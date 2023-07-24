#include <stdlib.h>
#include <vulkan/vulkan_core.h>

#include "renderer.h"

void destroy_renderer (VKRenderer *renderer)
{
    vkDestroyCommandPool(renderer->logical_device, renderer->command_pool, NULL);

    for (uint32_t i = 0; i < renderer->swapchain_images_count; i++) {
        vkDestroyFramebuffer(renderer->logical_device, renderer->swapchain_framebuffers[i], NULL);
        vkDestroyImageView(renderer->logical_device, renderer->swapchain_image_views[i], NULL);
    }
    free(renderer->swapchain_image_views);
    free(renderer->swapchain_framebuffers);
    free(renderer->command_buffers);
    free(renderer->swapchain_images);

    for (int i = 0; i < renderer->max_frames_in_flight; i++) {
        vkDestroySemaphore(renderer->logical_device, renderer->sm_image_available[i], NULL);
        vkDestroySemaphore(renderer->logical_device, renderer->sm_render_finished[i], NULL);
        vkDestroyFence(renderer->logical_device, renderer->in_flight_fences[i], NULL);
    }
    free(renderer->sm_image_available);
    free(renderer->sm_render_finished);
    free(renderer->in_flight_fences);

    vkDestroyBuffer(renderer->logical_device, renderer->vertex_buffer, NULL);
    vkFreeMemory(renderer->logical_device, renderer->vertex_buffer_memory, NULL);
    vkDestroyPipelineLayout(renderer->logical_device, renderer->pipeline_layout, NULL);
    vkDestroyRenderPass(renderer->logical_device, renderer->render_pass, NULL);
    vkDestroyPipeline(renderer->logical_device, renderer->graphics_pipeline, NULL);
    vkDestroySwapchainKHR(renderer->logical_device, renderer->swapchain, NULL);
    vkDestroyDevice(renderer->logical_device, NULL);
    vkDestroySurfaceKHR(renderer->instance, renderer->surface, NULL);
    vkDestroyInstance(renderer->instance, NULL);

    glfwDestroyWindow(renderer->window);
    glfwTerminate();
}
