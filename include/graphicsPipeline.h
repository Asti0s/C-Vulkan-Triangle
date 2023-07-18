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
