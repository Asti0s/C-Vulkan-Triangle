#include "renderer.h"
#include "constants.h"

#include <stdlib.h>
#include <vulkan/vulkan_core.h>

int create_command_pool (VKRenderer *renderer)
{
    VkCommandPoolCreateInfo pool_info = {0};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = renderer->queue_family_indices.graphics;
    pool_info.flags = 0;

    if (vkCreateCommandPool(renderer->logical_device, &pool_info, NULL, &renderer->command_pool) != VK_SUCCESS)
        return CReturnFailure;

    return CReturnSuccess;
}

int allocate_command_buffers (VKRenderer *renderer)
{
    renderer->command_buffers = malloc(sizeof(VkCommandBuffer) * renderer->swapchain_images_count);
    if (renderer->command_buffers == NULL)
        return CReturnFailure;

    VkCommandBufferAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = renderer->command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = renderer->swapchain_images_count;

    if (vkAllocateCommandBuffers(renderer->logical_device, &alloc_info, renderer->command_buffers) != VK_SUCCESS)
        return CReturnFailure;

    return CReturnSuccess;
}

int record_command_buffers (VKRenderer *renderer)
{
    VkClearValue clear_color = {{{0, 0, 0, 1}}};

    for (uint32_t i = 0; i < renderer->swapchain_images_count; i++) {
        VkCommandBufferBeginInfo begin_info = {0};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(renderer->command_buffers[i], &begin_info) != VK_SUCCESS)
            return CReturnFailure;

        VkRenderPassBeginInfo render_pass_info = {0};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = renderer->render_pass;
        render_pass_info.framebuffer = renderer->swapchain_framebuffers[i];
        render_pass_info.renderArea.offset.x = 0;
        render_pass_info.renderArea.offset.y = 0;
        render_pass_info.renderArea.extent = renderer->swapchain_extent;
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_color;
        vkCmdBeginRenderPass(renderer->command_buffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(renderer->command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->graphics_pipeline);

        vkCmdDraw(renderer->command_buffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(renderer->command_buffers[i]);

        if (vkEndCommandBuffer(renderer->command_buffers[i]) != VK_SUCCESS)
            return CReturnFailure;
    }

    return CReturnSuccess;
}
