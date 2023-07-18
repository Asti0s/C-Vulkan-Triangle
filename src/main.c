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

void destroy_renderer (VKRenderer *renderer)
{
    for (uint32_t i = 0; i < renderer->swapchain_images_count; i++) {
        vkDestroyFramebuffer(renderer->logical_device, renderer->swapchain_framebuffers[i], NULL);
        vkDestroyImageView(renderer->logical_device, renderer->swapchain_image_views[i], NULL);
    }
    free(renderer->swapchain_image_views);
    free(renderer->swapchain_framebuffers);
    free(renderer->command_buffers);
    free(renderer->swapchain_images);

    vkDestroyCommandPool(renderer->logical_device, renderer->command_pool, NULL);
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

void main_loop (VKRenderer *renderer)
{
    while (!glfwWindowShouldClose(renderer->window)) {
        glfwPollEvents();
    }
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
