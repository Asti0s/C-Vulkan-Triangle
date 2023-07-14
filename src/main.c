#include "GLFW/glfw3.h"
#include "constants.h"
#include "renderer.h"

void destroy_renderer (VKRenderer *renderer)
{
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