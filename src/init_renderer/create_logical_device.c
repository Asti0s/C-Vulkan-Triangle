#include "constants.h"
#include "device.h"

extern const char *validationLayers[];
extern const bool EnableDebugMode;

void fill_queue_create_info (VKRenderer *renderer, VkDeviceQueueCreateInfo *queue_create_info,
float *queue_priority)
{
    queue_create_info[0] = (VkDeviceQueueCreateInfo){0};
    queue_create_info[1] = (VkDeviceQueueCreateInfo){0};

    queue_create_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info[0].queueFamilyIndex = renderer->queue_family_indices.graphics;
    queue_create_info[0].queueCount = 1;
    queue_create_info[0].pQueuePriorities = queue_priority;

    queue_create_info[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info[1].queueFamilyIndex = renderer->queue_family_indices.present;
    queue_create_info[1].queueCount = 1;
    queue_create_info[1].pQueuePriorities = queue_priority;
}

void fill_device_create_info (VKRenderer *renderer, VkDeviceCreateInfo *device_create_info,
VkDeviceQueueCreateInfo *queue_create_info, const char *const *extension_names, VkPhysicalDeviceFeatures *device_features)
{
    device_create_info->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info->pQueueCreateInfos = queue_create_info;

    if (renderer->queue_family_indices.graphics == renderer->queue_family_indices.present)
        device_create_info->queueCreateInfoCount = 1;
    else
        device_create_info->queueCreateInfoCount = 2;

    device_create_info->pEnabledFeatures = device_features;
    device_create_info->enabledExtensionCount = 1;
    device_create_info->ppEnabledExtensionNames = extension_names;

    if (EnableDebugMode) {
        device_create_info->ppEnabledLayerNames = validationLayers;
        for (int i = 0; validationLayers[i] != NULL; i++)
            device_create_info->enabledLayerCount += 1;
    }
}

void create_queues (VKRenderer *renderer)
{
    vkGetDeviceQueue(renderer->logical_device, renderer->queue_family_indices.graphics, 0, &renderer->graphics_queue);
    if (renderer->queue_family_indices.present != renderer->queue_family_indices.graphics)
        vkGetDeviceQueue(renderer->logical_device, renderer->queue_family_indices.present, 0, &renderer->present_queue);
    else
        renderer->present_queue = renderer->graphics_queue;
}

int create_logical_device (VKRenderer *renderer)
{
    VkPhysicalDeviceFeatures device_features = {0};
    VkDeviceQueueCreateInfo queue_create_info[2] = {0};
    VkDeviceCreateInfo device_create_info = {0};
    float queue_priority = 1.0f;

    fill_queue_create_info(renderer, queue_create_info, &queue_priority);
    fill_device_create_info(renderer, &device_create_info, queue_create_info,
        &(const char *const){VK_KHR_SWAPCHAIN_EXTENSION_NAME}, &device_features);

    if (vkCreateDevice(renderer->physical_device, &device_create_info, NULL, &renderer->logical_device) != VK_SUCCESS)
        return CReturnFailure;

    create_queues(renderer);

    return CReturnSuccess;
}
