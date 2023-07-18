#include "constants.h"
#include "renderer.h"
#include "window.h"
#include "instance.h"
#include "device.h"
#include "swapchain.h"
#include "graphicsPipeline.h"

int init_renderer (VKRenderer *renderer)
{
    VkSemaphoreCreateInfo semaphore_info = {0};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (create_window(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_instance(renderer) != CReturnSuccess) return CReturnFailure;
    if (glfwCreateWindowSurface(renderer->instance, renderer->window, NULL, &renderer->surface) != VK_SUCCESS) return CReturnFailure;
    if (pick_physical_device(renderer) != CReturnSuccess) return CReturnFailure;
    if (pick_queue_family_indices(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_logical_device(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_swapchain(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_image_views(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_render_pass(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_graphics_pipeline(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_framebuffers(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_command_pool(renderer) != CReturnSuccess) return CReturnFailure;
    if (allocate_command_buffers(renderer) != CReturnSuccess) return CReturnFailure;
    if (record_command_buffers(renderer) != CReturnSuccess) return CReturnFailure;
    if (vkCreateSemaphore(renderer->logical_device, &semaphore_info, NULL, &renderer->sm_image_available) != VK_SUCCESS) return CReturnFailure;
    if (vkCreateSemaphore(renderer->logical_device, &semaphore_info, NULL, &renderer->sm_render_finished) != VK_SUCCESS) return CReturnFailure;

    return CReturnSuccess;
}
