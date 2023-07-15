#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "constants.h"
#include "renderer.h"

extern const char *validationLayers[];
extern const bool EnableDebugMode;

int check_for_validation_layers_support ()
{
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);
    VkLayerProperties *available_layers = malloc(sizeof(VkLayerProperties) * layer_count);
    if (available_layers == NULL)
        return CReturnFailure;
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

    for (int i = 0; validationLayers[i] != 0; i++) {
        int found = 0;
        for (uint32_t j = 0; j < layer_count; j++) {
            if (strcmp(validationLayers[i], available_layers[j].layerName) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            dprintf(2, "Missing validation layer : \"%s\"\n", validationLayers[i]);
            free(available_layers);
            return CReturnFailure;
        }
    }

    free(available_layers);
    return CReturnSuccess;
}

int check_for_required_extentions_support ()
{
    uint32_t required_extension_count = 0;
    const char **required_extensions = glfwGetRequiredInstanceExtensions(&required_extension_count);

    uint32_t available_extension_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &available_extension_count, NULL);
    VkExtensionProperties *available_extensions =
        malloc(sizeof(VkExtensionProperties) * available_extension_count);
    if (available_extensions == NULL)
        return CReturnFailure;
    vkEnumerateInstanceExtensionProperties(NULL, &available_extension_count, available_extensions);

    for (uint32_t i = 0; i < required_extension_count; i++) {
        int found = 0;
        for (uint32_t j = 0; j < available_extension_count; j++) {
            if (strcmp(required_extensions[i], available_extensions[j].extensionName) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            dprintf(2, "Failed to create vulkan instance : required extension \"%s\" not found.\n", required_extensions[i]);
            free(available_extensions);
            return CReturnFailure;
        }
    }

    free(available_extensions);
    return CReturnSuccess;
}

int create_instance (VKRenderer *renderer)
{
    if (check_for_required_extentions_support() != CReturnSuccess)
        return CReturnFailure;

    VkApplicationInfo app_info = {0};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Vulkan Renderer";
    app_info.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instance_info = {0};
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &app_info;
    instance_info.ppEnabledExtensionNames =
        glfwGetRequiredInstanceExtensions(&instance_info.enabledExtensionCount);

    if (EnableDebugMode == true && check_for_validation_layers_support() == CReturnSuccess) {
        for (instance_info.enabledLayerCount = 0;
            validationLayers[instance_info.enabledLayerCount] != 0;
            instance_info.enabledLayerCount++
        );

        instance_info.ppEnabledLayerNames = validationLayers;
    }

    if (vkCreateInstance(&instance_info, NULL, &renderer->instance) != VK_SUCCESS)
        return CReturnFailure;

    return CReturnSuccess;
}
