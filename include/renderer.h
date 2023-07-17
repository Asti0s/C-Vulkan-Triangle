#pragma once

#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"

/**
 * @brief Struct that holds the queue family indices for the application.
 */
typedef struct {
    int graphics;
    int present;
} queue_family_indices;

/**
 * @brief Struct that holds all of the necessary objects for the renderer.
 */
typedef struct {
    GLFWwindow *window;
    VkInstance instance;
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkSurfaceKHR surface;
    queue_family_indices queue_family_indices;
} VKRenderer;

/**
 * @brief Fully initializes the renderer by creating all of the necessary
 * objects for vulkan and setting up the window.
 *
 * @param renderer pointer to the empty renderer struct to initialize.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int init_renderer (VKRenderer *renderer);