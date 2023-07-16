#include <stdbool.h>

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
    glfwDestroyWindow(renderer->window);
    glfwTerminate();

    vkDestroyDevice(renderer->logical_device, NULL);
    vkDestroyInstance(renderer->instance, NULL);
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