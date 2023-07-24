#include "constants.h"
#include "renderer.h"
#include "window.h"
#include "instance.h"
#include "device.h"
#include "swapchain.h"
#include "graphicsPipeline.h"

int init_renderer (VKRenderer *renderer)
{
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
    if (create_vertex_buffer(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_command_pool(renderer) != CReturnSuccess) return CReturnFailure;
    if (allocate_command_buffers(renderer) != CReturnSuccess) return CReturnFailure;
    if (record_command_buffers(renderer) != CReturnSuccess) return CReturnFailure;
    if (create_sync_objects(renderer) != CReturnSuccess) return CReturnFailure;

    return CReturnSuccess;
}
