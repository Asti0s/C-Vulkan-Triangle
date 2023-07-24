#include "renderer.h"
#include "constants.h"

int render_image (VKRenderer *renderer, uint32_t image_index, int current_frame)
{
    VkSubmitInfo submit_info = {0};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &renderer->command_buffers[image_index];
    submit_info.pWaitSemaphores = &renderer->sm_image_available[current_frame];
    submit_info.pWaitDstStageMask = (VkPipelineStageFlags[]) {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &renderer->sm_render_finished[current_frame];

    if (vkQueueSubmit(renderer->graphics_queue, 1, &submit_info, renderer->in_flight_fences[current_frame]) != VK_SUCCESS)
        return CReturnFailure;

    return CReturnSuccess;
}

int present_image (VKRenderer *renderer, uint32_t image_index, int current_frame)
{
    VkPresentInfoKHR present_info = {0};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &renderer->sm_render_finished[current_frame];
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &renderer->swapchain;
    present_info.pImageIndices = &image_index;

    if (vkQueuePresentKHR(renderer->present_queue, &present_info) != VK_SUCCESS)
        return CReturnFailure;

    return CReturnSuccess;
}

int draw_frame (VKRenderer *renderer)
{
    static int current_frame = 0;
    vkWaitForFences(renderer->logical_device, 1, &renderer->in_flight_fences[current_frame], VK_FALSE, UINT64_MAX);
    vkResetFences(renderer->logical_device, 1, &renderer->in_flight_fences[current_frame]);

    uint32_t image_index;
    if (vkAcquireNextImageKHR(renderer->logical_device, renderer->swapchain, UINT64_MAX, renderer->sm_image_available[current_frame], VK_NULL_HANDLE, &image_index) != VK_SUCCESS)
        return CReturnFailure;

    if (render_image(renderer, image_index, current_frame) != CReturnSuccess)
        return CReturnFailure;

    if (present_image(renderer, image_index, current_frame) != CReturnSuccess)
        return CReturnFailure;

    current_frame = (current_frame + 1) % renderer->max_frames_in_flight;

    return CReturnSuccess;
}

int main_loop (VKRenderer *renderer)
{
    while (!glfwWindowShouldClose(renderer->window)) {
        glfwPollEvents();

        if (draw_frame(renderer) == CReturnFailure)
            return CReturnFailure;
    }

    vkDeviceWaitIdle(renderer->logical_device);

    return CReturnSuccess;
}
