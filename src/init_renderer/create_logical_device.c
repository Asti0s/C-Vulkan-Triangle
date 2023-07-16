#include <stdbool.h>
#include <vulkan/vulkan_core.h>

#include "constants.h"
#include "device.h"

extern const char *validationLayers[];
extern const bool EnableDebugMode;

int create_logical_device (VKRenderer *renderer)
{
    uint32_t graphics_queue_family_indice = pick_queue_family_indice(renderer->physical_device, VK_QUEUE_GRAPHICS_BIT);
    if ((int) graphics_queue_family_indice == CReturnFailure)
        return CReturnFailure;
    float queue_priority = 1.0f;

    VkDeviceQueueCreateInfo queue_create_info = {0};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = graphics_queue_family_indice;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features = {0};

    VkDeviceCreateInfo device_create_info = {0};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pEnabledFeatures = &device_features;
    device_create_info.enabledExtensionCount = 0;

    if (EnableDebugMode) {
        device_create_info.ppEnabledLayerNames = validationLayers;
        for (int i = 0; validationLayers[i] != NULL; i++)
            device_create_info.enabledLayerCount += 1;
    }

    if (vkCreateDevice(renderer->physical_device, &device_create_info, NULL, &renderer->logical_device) != VK_SUCCESS)
        return CReturnFailure;

    vkGetDeviceQueue(renderer->logical_device, graphics_queue_family_indice, 0, &renderer->graphics_queue);

    return CReturnSuccess;
}
