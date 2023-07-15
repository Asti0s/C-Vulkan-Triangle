#include "constants.h"
#include "renderer.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

extern const bool EnableDebugMode;

int rate_physical_device (VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(device, &device_properties);

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER ||
    device_features.geometryShader == VK_FALSE)
        return 0;

    int score = device_properties.limits.maxImageDimension2D;

    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        score += score * 2;
    else if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
        score += score;
    else if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
        score += score / 2;
    else if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
        score += score / 4;

    return score;
}

int pick_physical_device (VKRenderer *renderer)
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(renderer->instance, &device_count, NULL);
    if (device_count == 0)
        return CReturnFailure;

    VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * device_count);
    if (devices == NULL)
        return CReturnFailure;
    vkEnumeratePhysicalDevices(renderer->instance, &device_count, devices);

    int best_score = 0;
    VkPhysicalDevice best_device = VK_NULL_HANDLE;

    for (uint32_t i = 0; i < device_count; i++) {
        int score = rate_physical_device(devices[i]);
        if (score > best_score) {
            best_score = score;
            best_device = devices[i];
        }
    }

    renderer->physical_device = best_device;
    free(devices);

    if (best_score == 0) {
        dprintf(2, "Failed to find a suitable GPU.\n");
        return CReturnFailure;
    }

    if (EnableDebugMode == true) {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(best_device, &device_properties);
        printf("Best device found : %s\n", device_properties.deviceName);
    }

    return CReturnSuccess;
}
