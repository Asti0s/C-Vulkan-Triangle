#include <stdlib.h>

#include "renderer.h"
#include "constants.h"

int create_framebuffers (VKRenderer *renderer)
{
    renderer->swapchain_framebuffers = malloc(sizeof(VkFramebuffer) * renderer->swapchain_images_count);
    if (renderer->swapchain_framebuffers == NULL)
        return CReturnFailure;

    for (uint32_t i = 0; i < renderer->swapchain_images_count; i++) {
        VkImageView attachments[] = {renderer->swapchain_image_views[i]};

        VkFramebufferCreateInfo framebuffer_info = {0};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = renderer->render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = renderer->swapchain_extent.width;
        framebuffer_info.height = renderer->swapchain_extent.height;
        framebuffer_info.layers = 1;

        if (vkCreateFramebuffer(renderer->logical_device, &framebuffer_info, NULL, &renderer->swapchain_framebuffers[i]) != VK_SUCCESS)
            return CReturnSuccess;
    }

    return CReturnSuccess;
}
