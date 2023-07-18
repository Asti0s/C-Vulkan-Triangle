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
 * @brief Struct that holds the swapchain support details for the application.
 */
typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR *formats;
    VkPresentModeKHR *present_modes;
    uint32_t formats_count;
    uint32_t present_modes_count;
} swapchain_support_details;

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
    swapchain_support_details swapchain_support_details;
    VkSwapchainKHR swapchain;
    VkImage *swapchain_images;
    uint32_t swapchain_images_count;
    VkFormat swapchain_image_format;
    VkExtent2D swapchain_extent;
    VkImageView *swapchain_image_views;
    VkPipelineLayout pipeline_layout;
    VkRenderPass render_pass;
    VkPipeline graphics_pipeline;
    VkFramebuffer *swapchain_framebuffers;
    VkCommandPool command_pool;
    VkCommandBuffer *command_buffers;
} VKRenderer;

/**
 * @brief Fully initializes the renderer by creating all of the necessary
 * objects for vulkan and setting up the window.
 *
 * @param renderer pointer to the empty renderer struct to initialize.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int init_renderer (VKRenderer *renderer);
