#pragma once

#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"

typedef struct {
    GLFWwindow *window;
    VkInstance instance;
    VkPhysicalDevice physical_device;
} VKRenderer;

/**
 * @brief Fully initializes the renderer by creating all of the necessary
 * objects for vulkan and setting up the window.
 *
 * @param renderer pointer to the empty renderer struct to initialize.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int init_renderer (VKRenderer *renderer);