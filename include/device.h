#pragma once

#include <vulkan/vulkan_core.h>

#include "renderer.h"

/**
 * @brief Rates a physical device based on its properties and features.
 *
 * @param device the physical device to rate.
 * @return int the score of the device.
 */
int rate_physical_device (VkPhysicalDevice device);

/**
 * @brief Picks the best physical device for the application and stores
 * it in the renderer struct.
 *
 * @param renderer pointer to the renderer struct where the physical
 * device will be stored.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int pick_physical_device (VKRenderer *renderer);
