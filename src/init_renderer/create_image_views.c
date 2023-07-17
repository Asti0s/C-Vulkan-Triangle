#include "constants.h"
#include "window.h"

#include <stdlib.h>

int create_image_views (VKRenderer *renderer)
{
    renderer->swapchain_image_views = malloc(sizeof(VkImageView) * renderer->swapchain_images_count);
    if (renderer->swapchain_image_views == NULL)
        return CReturnFailure;

    for (uint32_t i = 0; i < renderer->swapchain_images_count; i++) {
        VkImageViewCreateInfo image_view_create_info = {0};
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image = renderer->swapchain_images[i];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = renderer->swapchain_image_format;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(renderer->logical_device, &image_view_create_info, NULL, &renderer->swapchain_image_views[i]) != VK_SUCCESS)
            return CReturnFailure;

    }

    return CReturnSuccess;
}
