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
    VkSemaphore *sm_image_available;
    VkSemaphore *sm_render_finished;
    VkFence *in_flight_fences;
    int max_frames_in_flight;
} VKRenderer;

/**
 * @brief Fully initializes the renderer by creating all of the necessary
 * objects for vulkan and setting up the window.
 *
 * @param renderer pointer to the empty renderer struct to initialize.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int init_renderer (VKRenderer *renderer);

/**
 * @brief Frees all of the memory allocated by the renderer.
 *
 * @param renderer the renderer to free.
 */
void destroy_renderer (VKRenderer *renderer);

/**
 * @brief Create the synchronization objects for the application.
 *
 * @param renderer the renderer to use.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int create_sync_objects (VKRenderer *renderer);

/**
 * @brief Executes the main loop of the application.
 *
 * @param renderer the renderer to use.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int main_loop (VKRenderer *renderer);
