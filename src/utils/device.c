#include <stdint.h>
#include <stdlib.h>

#include "constants.h"
#include "renderer.h"

uint32_t pick_queue_family_indice (VkPhysicalDevice physical_device, VkQueueFlagBits queueFlagBits)
{
    int indice = -1;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties *queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    if (queueFamilies == NULL)
        return CReturnFailure;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, queueFamilies);

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & queueFlagBits) {
            indice = i;
            break;
        }
    }

    free(queueFamilies);
    return indice;
}
