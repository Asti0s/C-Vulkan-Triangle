#include "constants.h"
#include "renderer.h"
#include "swapchain.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

extern const bool EnableDebugMode;

bool is_device_suitable (VKRenderer *renderer, VkPhysicalDevice device, VkPhysicalDeviceProperties device_properties, VkPhysicalDeviceFeatures device_features)
{
    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER ||
    device_features.geometryShader == VK_FALSE)
        return false;

    // Check if the device support the swapchain extension
    swapchain_support_details swapchain_support_details;
    if (querry_swapchain_support_details(&swapchain_support_details, renderer->surface, device) == CReturnFailure)
        return false;
    if (swapchain_support_details.formats_count == 0 || swapchain_support_details.present_modes_count == 0)
        return false;
    free(swapchain_support_details.formats);
    free(swapchain_support_details.present_modes);

    // Check if the device support the glfw's required extensions
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);

    VkExtensionProperties *available_extensions = malloc(sizeof(VkExtensionProperties) * extension_count);
    if (available_extensions == NULL)
        return 0;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, available_extensions);

    for (uint32_t i = 0; i < extension_count; i++) {
        if (strcmp(available_extensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
            free(available_extensions);
            return true;
        }
    }

    return false;
}

int rate_physical_device (VKRenderer *renderer, VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    if (!is_device_suitable(renderer, device, device_properties, device_features))
        return 0;

    int score = device_properties.limits.maxImageDimension2D;

    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        score += score * 2;
    else if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
        score += score;
    else if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
        score += score / 2;
    else if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
        score += score / 4;

    return score;
}

int pick_physical_device (VKRenderer *renderer)
{
    // Get all of the physical devices available
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(renderer->instance, &device_count, NULL);
    VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * device_count);
    if (devices == NULL)
        return CReturnFailure;
    vkEnumeratePhysicalDevices(renderer->instance, &device_count, devices);

    // Select the best device available based on the score
    int best_score = 0;
    VkPhysicalDevice best_device = VK_NULL_HANDLE;
    for (uint32_t i = 0; i < device_count; i++) {
        int score = rate_physical_device(renderer, devices[i]);
        if (score > best_score) {
            best_score = score;
            best_device = devices[i];
        }
    }

    renderer->physical_device = best_device;
    free(devices);

    if (best_score == 0) {
        dprintf(2, "Failed to find a suitable GPU.\n");
        return CReturnFailure;
    }

    // Print the best device found if debug mode is enabled
    if (EnableDebugMode == true) {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(best_device, &device_properties);
        printf("Best device found : %s\n", device_properties.deviceName);
    }

    return CReturnSuccess;
}
