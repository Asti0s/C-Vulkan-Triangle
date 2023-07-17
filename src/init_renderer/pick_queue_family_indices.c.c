#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "constants.h"
#include "renderer.h"

extern const bool EnableDebugMode;

int pick_queue_family_indices (VKRenderer *renderer)
{
    renderer->queue_family_indices.graphics = -1;
    renderer->queue_family_indices.present = -1;

    // Get queue family properties
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(renderer->physical_device, &queueFamilyCount, NULL);
    VkQueueFamilyProperties *queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    if (queueFamilies == NULL)
        return CReturnFailure;
    vkGetPhysicalDeviceQueueFamilyProperties(renderer->physical_device, &queueFamilyCount, queueFamilies);

    // Find queue family indices
    for (int i = queueFamilyCount - 1; i >= 0; i--) {
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(renderer->physical_device, i, renderer->surface, &present_support);

        if (present_support)
            renderer->queue_family_indices.present = i;

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            renderer->queue_family_indices.graphics = i;
    }

    // Print debug info if enabled
    if (EnableDebugMode) {
        for (uint32_t i = 0; i < queueFamilyCount; i++) {
            printf("\nQueue family %d:\n", i);
            printf("- Count: %d\n", queueFamilies[i].queueCount);
            printf("- Flags:\n");
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                printf(". VK_QUEUE_GRAPHICS_BIT\n");
            if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
                printf(". VK_QUEUE_COMPUTE_BIT\n");
            if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
                printf(". VK_QUEUE_TRANSFER_BIT\n");
            if (queueFamilies[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
                printf(". VK_QUEUE_SPARSE_BINDING_BIT\n");
            if (queueFamilies[i].queueFlags & VK_QUEUE_PROTECTED_BIT)
                printf(". VK_QUEUE_PROTECTED_BIT\n");
        }
    }

    free(queueFamilies);
    return CReturnSuccess;
}
