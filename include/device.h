#pragma once

#include <stdint.h>
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

/**
 * @brief Pick a queue family indice based on the given flag bits.
 *
 * @param physical_device the physical device to look for the queue family.
 * device is stored.
 * @param flagBits the flag bits to look for. Can be :
 * VK_QUEUE_GRAPHICS_BIT,
 * VK_QUEUE_COMPUTE_BIT,
 * VK_QUEUE_TRANSFER_BIT,
 * VK_QUEUE_SPARSE_BINDING_BIT,
 * VK_QUEUE_PROTECTED_BIT.
 * @return uint32_t the indice of the queue family if successful, -1 otherwise.
 */
uint32_t pick_queue_family_indice (VkPhysicalDevice physical_device, VkQueueFlagBits queueFlagBits);

/**
 * @brief Create a logical device object and put it in the renderer struct.
 *
 * @param renderer pointer to the renderer struct where the logical
 * device will be stored.
 * @return int CReturnSuccess if successful, CReturnFailure otherwise.
 */
int create_logical_device (VKRenderer *renderer);
