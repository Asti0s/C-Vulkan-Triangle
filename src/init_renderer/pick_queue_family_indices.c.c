#include <stdlib.h>
#include <stdbool.h>

#include "constants.h"
#include "renderer.h"

int pick_queue_family_indices (VKRenderer *renderer)
{
    renderer->queue_family_indices.graphics = -1;
    renderer->queue_family_indices.present = -1;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(renderer->physical_device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties *queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    if (queueFamilies == NULL)
        return CReturnFailure;
    vkGetPhysicalDeviceQueueFamilyProperties(renderer->physical_device, &queueFamilyCount, queueFamilies);

    for (int i = queueFamilyCount - 1; i >= 0; i--) {

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(renderer->physical_device, i, renderer->surface, &present_support);

        if (present_support)
            renderer->queue_family_indices.present = i;

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            renderer->queue_family_indices.graphics = i;

    }

    free(queueFamilies);
    return CReturnSuccess;
}
