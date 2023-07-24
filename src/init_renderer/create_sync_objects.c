#include "renderer.h"
#include "constants.h"

#include <stdlib.h>

int create_sync_objects (VKRenderer *renderer)
{
    renderer->sm_image_available = malloc(sizeof(VkSemaphore) * renderer->max_frames_in_flight);
    renderer->sm_render_finished = malloc(sizeof(VkSemaphore) * renderer->max_frames_in_flight);
    renderer->in_flight_fences = malloc(sizeof(VkFence) * renderer->max_frames_in_flight);
    if (renderer->sm_image_available == NULL || renderer->sm_render_finished == NULL || renderer->in_flight_fences == NULL)
        return CReturnFailure;

    VkSemaphoreCreateInfo semaphore_info = {0};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info = {0};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < renderer->max_frames_in_flight; i++) {
        if (vkCreateSemaphore(renderer->logical_device, &semaphore_info, NULL, &renderer->sm_image_available[i]) != VK_SUCCESS)
            return CReturnFailure;
        if (vkCreateSemaphore(renderer->logical_device, &semaphore_info, NULL, &renderer->sm_render_finished[i]) != VK_SUCCESS)
            return CReturnFailure;
        if (vkCreateFence(renderer->logical_device, &fence_info, NULL, &renderer->in_flight_fences[i]) != VK_SUCCESS)
            return CReturnFailure;
    }

    return CReturnSuccess;
}
