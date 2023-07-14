#include "GLFW/glfw3.h"
#include "constants.h"
#include "renderer.h"

int create_window (VKRenderer *renderer)
{
    if (!glfwInit())
        return CReturnFailure;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_NO_API);

    renderer->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", NULL, NULL);
    if (!renderer->window)
        return CReturnFailure;

    return CReturnSuccess;
}