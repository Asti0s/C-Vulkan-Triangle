#include "renderer.h"
#include "constants.h"

#include <stdlib.h>

extern const int MAX_FRAMES_IN_FLIGHT;

int create_sync_objects (VKRenderer *renderer)
{
    renderer->sm_image_available = malloc(sizeof(VkSemaphore) * MAX_FRAMES_IN_FLIGHT);
    renderer->sm_render_finished = malloc(sizeof(VkSemaphore) * MAX_FRAMES_IN_FLIGHT);
    renderer->in_flight_fences = malloc(sizeof(VkFence) * MAX_FRAMES_IN_FLIGHT);
    if (renderer->sm_image_available == NULL || renderer->sm_render_finished == NULL || renderer->in_flight_fences == NULL)
        return CReturnFailure;

    VkSemaphoreCreateInfo semaphore_info = {0};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info = {0};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(renderer->logical_device, &semaphore_info, NULL, &renderer->sm_image_available[i]) != VK_SUCCESS)
            return CReturnFailure;
        if (vkCreateSemaphore(renderer->logical_device, &semaphore_info, NULL, &renderer->sm_render_finished[i]) != VK_SUCCESS)
            return CReturnFailure;
        if (vkCreateFence(renderer->logical_device, &fence_info, NULL, &renderer->in_flight_fences[i]) != VK_SUCCESS)
            return CReturnFailure;
    }

    return CReturnSuccess;
}
