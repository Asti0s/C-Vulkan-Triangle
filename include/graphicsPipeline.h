#pragma once

#include "renderer.h"

/**
 * @brief Create a render pass object and store it in the renderer struct.
 *
 * @param renderer The renderer to store the render pass in.
 * @return int CReturnSuccess if the render pass was created successfully.
 * CReturnFailure otherwise.
 */
int create_render_pass (VKRenderer *renderer);

/**
 * @brief Create a graphics pipeline object and store it in the renderer.
 *
 * @param renderer The renderer to store the graphics pipeline in.
 * @return int CReturnSuccess if the graphics pipeline was created successfully.
 * CReturnFailure otherwise.
 */
int create_graphics_pipeline (VKRenderer *renderer);

/**
 * @brief Create a framebuffers needed to render to the swapchain images.
 *
 * @param renderer The renderer to store the framebuffers in.
 * @return int CReturnSuccess if the framebuffers were created successfully.
 * CReturnFailure otherwise.
 */
int create_framebuffers (VKRenderer *renderer);

/**
 * @brief Create a command pool object and store it in the renderer struct.
 *
 * @param renderer The renderer to store the command pool in.
 * @return int CReturnSuccess if the command pool was created successfully.
 * CReturnFailure otherwise.
 */
int create_command_pool (VKRenderer *renderer);

/**
 * @brief Create a command buffer for each swapchain image and store them
 * in the renderer struct.
 *
 * @param renderer The renderer to store the command buffers in.
 * @return int CReturnSuccess if the command buffers were created successfully.
 * CReturnFailure otherwise.
 */
int allocate_command_buffers (VKRenderer *renderer);

/**
 * @brief Record the command buffers to render to the swapchain images.
 *
 * @param renderer The renderer to record the command buffers in.
 * @return int CReturnSuccess if the command buffers were recorded successfully.
 * CReturnFailure otherwise.
 */
int record_command_buffers (VKRenderer *renderer);
