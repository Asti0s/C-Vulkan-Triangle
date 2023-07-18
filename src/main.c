#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "constants.h"
#include "renderer.h"

/**
 * @brief enable the debug mode or not depending on the NDEBUG (NO debug) macro.
 * If the NDEBUG macro is defined, the debug mode is disabled, else he is enabled.
 * To define the NDEBUG macro, you can use the -DNDEBUG flag when compiling.
 *
 * @return true if the debug mode are enabled
 * @return false if the debug mode are not enabled
 */
#ifdef NDEBUG
    const bool EnableDebugMode = false;
#else
    const bool EnableDebugMode = true;
#endif

/**
 * @brief the vulkan validation layers to use if "EnableDebugMode" is set to true.
 */
const char *validationLayers[] = {
    "VK_LAYER_KHRONOS_validation"
};

int draw_frame (VKRenderer *renderer)
{
    // Acquire
    uint32_t image_index;
    if (vkAcquireNextImageKHR(renderer->logical_device, renderer->swapchain, UINT64_MAX, renderer->sm_image_available, VK_NULL_HANDLE, &image_index) != VK_SUCCESS)
        return CReturnFailure;


    // Submit
    VkSubmitInfo submit_info = {0};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &renderer->command_buffers[image_index];
    submit_info.pWaitSemaphores = &renderer->sm_image_available;
    submit_info.pWaitDstStageMask = (VkPipelineStageFlags[]) {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &renderer->sm_render_finished;

    if (vkQueueSubmit(renderer->graphics_queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
        return CReturnFailure;


    // Present
    VkPresentInfoKHR present_info = {0};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &renderer->sm_render_finished;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &renderer->swapchain;
    present_info.pImageIndices = &image_index;

    if (vkQueuePresentKHR(renderer->present_queue, &present_info) != VK_SUCCESS)
        return CReturnFailure;

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

int main ()
{
    VKRenderer renderer = {0};
    if (init_renderer(&renderer) != CReturnSuccess)
        return CReturnFailure;

    main_loop(&renderer);

    destroy_renderer(&renderer);

    return CReturnSuccess;
}
