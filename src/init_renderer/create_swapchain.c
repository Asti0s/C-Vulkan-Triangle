#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "constants.h"
#include "renderer.h"

extern const bool EnableDebugMode;

int querry_swapchain_support_details (swapchain_support_details *swapchain_support_details, VkSurfaceKHR surface, VkPhysicalDevice device)
{
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapchain_support_details->capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &swapchain_support_details->formats_count, NULL);
    swapchain_support_details->formats = malloc(sizeof(VkSurfaceFormatKHR) * swapchain_support_details->formats_count);
    if (swapchain_support_details->formats == NULL)
        return CReturnFailure;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &swapchain_support_details->formats_count, swapchain_support_details->formats);

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &swapchain_support_details->present_modes_count, NULL);
    swapchain_support_details->present_modes = malloc(sizeof(VkPresentModeKHR) * swapchain_support_details->present_modes_count);
    if (swapchain_support_details->present_modes == NULL)
        return CReturnFailure;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &swapchain_support_details->present_modes_count, swapchain_support_details->present_modes);

    return CReturnSuccess;
}

VkSurfaceFormatKHR choose_surface_format (VkSurfaceFormatKHR *formats, uint32_t formats_count)
{
    for (uint32_t i = 0; i < formats_count; i++) {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            // Debug info if enabled
            if (EnableDebugMode)
                printf("\nSwapchain ideal format found:\n- VK_FORMAT_B8G8R8A8_SRGB,\n- VK_COLOR_SPACE_SRGB_NONLINEAR_KHR");

            return formats[i];
        }
    }

    // Debug info if enabled
    if (EnableDebugMode)
        printf("\nSwapchain ideal format not found: using first available format.");

    return formats[0];
}

VkPresentModeKHR choose_present_mode (VkPresentModeKHR *present_modes, uint32_t present_modes_count)
{
    // To avoid overconsumption of resources
    return VK_PRESENT_MODE_FIFO_KHR;

    for (uint32_t i = 0; i < present_modes_count; i++) {
        if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            // Debug info if enabled
            if (EnableDebugMode)
                printf("\nSwapchain present mode: VK_PRESENT_MODE_MAILBOX_KHR");

            return present_modes[i];
        }
    }

    // Debug info if enabled
    if (EnableDebugMode)
        printf("\nSwapchain present mode: VK_PRESENT_MODE_FIFO_KHR");

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D choose_swap_extent (VkSurfaceCapabilitiesKHR capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX) {
        if (EnableDebugMode)
            printf("\nSwapchain extent was already defined to: %dx%d", capabilities.currentExtent.width, capabilities.currentExtent.height);
        return capabilities.currentExtent;
    } else {
        VkExtent2D actual_extent = {WINDOW_WIDTH, WINDOW_HEIGHT};

        if (actual_extent.width > capabilities.maxImageExtent.width)
            actual_extent.width = capabilities.maxImageExtent.width;
        else if (actual_extent.width < capabilities.minImageExtent.width)
            actual_extent.width = capabilities.minImageExtent.width;

        if (actual_extent.height > capabilities.maxImageExtent.height)
            actual_extent.height = capabilities.maxImageExtent.height;
        else if (actual_extent.height < capabilities.minImageExtent.height)
            actual_extent.height = capabilities.minImageExtent.height;

        printf("\nSwapchain extent: %dx%d", capabilities.currentExtent.width, capabilities.currentExtent.height);
        return actual_extent;
    }
}

int get_swapchain_images (VKRenderer *renderer)
{
    vkGetSwapchainImagesKHR(renderer->logical_device, renderer->swapchain, &renderer->swapchain_images_count, NULL);

    renderer->swapchain_images = malloc(sizeof(VkImage) * renderer->swapchain_images_count);
    if (renderer->swapchain_images == NULL)
        return CReturnFailure;
    vkGetSwapchainImagesKHR(renderer->logical_device, renderer->swapchain, &renderer->swapchain_images_count, renderer->swapchain_images);

    return CReturnSuccess;
}

int create_swapchain (VKRenderer *renderer)
{
    swapchain_support_details swapchain_support_details;
    if (querry_swapchain_support_details(&swapchain_support_details, renderer->surface, renderer->physical_device) == CReturnFailure)
        return CReturnFailure;

    VkSurfaceFormatKHR surface_format = choose_surface_format(swapchain_support_details.formats, swapchain_support_details.formats_count);
    VkPresentModeKHR present_mode = choose_present_mode(swapchain_support_details.present_modes, swapchain_support_details.present_modes_count);
    VkExtent2D extent = choose_swap_extent(swapchain_support_details.capabilities);
    renderer->swapchain_images_count = swapchain_support_details.capabilities.minImageCount + 1;
    if (swapchain_support_details.capabilities.maxImageCount > 0 && renderer->swapchain_images_count > swapchain_support_details.capabilities.maxImageCount)
        renderer->swapchain_images_count = swapchain_support_details.capabilities.maxImageCount;

    renderer->swapchain_image_format = surface_format.format;
    renderer->swapchain_extent = extent;

    renderer->max_frames_in_flight = 3;
    if (renderer->max_frames_in_flight > (int) renderer->swapchain_images_count)
        renderer->max_frames_in_flight = renderer->swapchain_images_count;

    if (EnableDebugMode) {
        printf("\nSwapchain image count: %d\n", renderer->swapchain_images_count);
        printf("Max frames in flight: %d\n", renderer->max_frames_in_flight);
    }

    VkSwapchainCreateInfoKHR swapchain_create_info = {0};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.surface = renderer->surface;
    swapchain_create_info.presentMode = present_mode;
    swapchain_create_info.imageExtent = extent;
    swapchain_create_info.minImageCount = renderer->swapchain_images_count;
    swapchain_create_info.imageFormat = surface_format.format;
    swapchain_create_info.imageColorSpace = surface_format.colorSpace;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.preTransform = swapchain_support_details.capabilities.currentTransform;
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.clipped = VK_TRUE;
    swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

    // Define swapchain image sharing mode
    uint32_t queue_family_indices[] = {renderer->queue_family_indices.graphics, renderer->queue_family_indices.present};
    if (renderer->queue_family_indices.graphics != renderer->queue_family_indices.present) {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_create_info.queueFamilyIndexCount = 0;
        swapchain_create_info.pQueueFamilyIndices = NULL;
    }

    if (vkCreateSwapchainKHR(renderer->logical_device, &swapchain_create_info, NULL, &renderer->swapchain) != VK_SUCCESS)
        return CReturnFailure;

    if (get_swapchain_images(renderer) == CReturnFailure)
        return CReturnFailure;

    free(swapchain_support_details.present_modes);
    free(swapchain_support_details.formats);

    return CReturnSuccess;
}
