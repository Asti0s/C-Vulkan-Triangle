#pragma once

#include "renderer.h"

/**
 * @brief Get the swap chain support details.
 *
 * @param swapchain_support_details The struct to store the swap chain support details.
 * @param surface The surface to query the swap chain support details.
 * @param device The physical device to query the swap chain support details.
 * @return int CReturnSuccess if the swap chain support details were queried
 * successfully, CReturnFailure otherwise.
 */
int querry_swapchain_support_details (swapchain_support_details *swapchain_support_details,
VkSurfaceKHR surface, VkPhysicalDevice device);

/**
 * @brief Create a swap chain object and store it in the renderer.
 *
 * @param renderer The renderer struct to store the swap chain object.
 * @return int CReturnSuccess if the swap chain object was created
 * successfully, CReturnFailure otherwise.
 */
int create_swapchain (VKRenderer *renderer);
