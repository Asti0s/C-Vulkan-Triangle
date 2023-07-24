#include <string.h>

#include "constants.h"
#include "graphicsPipeline.h"

extern const vertex vertices[3];

uint32_t findMemoryType (VKRenderer *renderer, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(renderer->physical_device, &mem_properties);

    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
        if (typeFilter & (1 << i) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;

    return CReturnFailure;
}

int create_buffer (VKRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory)
{
    // Create the vertex buffer
    VkBufferCreateInfo buffer_info = {0};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(renderer->logical_device, &buffer_info, NULL, buffer) != VK_SUCCESS)
        return CReturnFailure;


    // Allocate the memory for the vertex buffer
    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(renderer->logical_device, *buffer, &mem_requirements);

    VkMemoryAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = findMemoryType(renderer, mem_requirements.memoryTypeBits, properties);

    if (vkAllocateMemory(renderer->logical_device, &alloc_info, NULL, buffer_memory) != VK_SUCCESS)
        return CReturnFailure;
    vkBindBufferMemory(renderer->logical_device, *buffer, *buffer_memory, 0);

    return CReturnSuccess;
}

int copy_buffer (VKRenderer *renderer, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size)
{
    // Create command buffer
    VkCommandBufferAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandBufferCount = 1;
    alloc_info.commandPool = renderer->command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer command_buffer;
    if (vkAllocateCommandBuffers(renderer->logical_device, &alloc_info, &command_buffer) != VK_SUCCESS)
        return CReturnFailure;

    VkCommandBufferBeginInfo begin_info = {0};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);


    // Record command buffer
    VkBufferCopy copy_region = {0};
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

    vkEndCommandBuffer(command_buffer);


    // Submit to queue
    VkSubmitInfo submit_info = {0};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    vkQueueSubmit(renderer->graphics_queue, 1, &submit_info, 0);
    vkQueueWaitIdle(renderer->graphics_queue);


    return CReturnSuccess;
}

int create_vertex_buffer (VKRenderer *renderer)
{
    // Staging buffer creation
    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;

    if (create_buffer(renderer, sizeof(vertices), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &staging_buffer, &staging_buffer_memory) != CReturnSuccess)
        return CReturnFailure;

    void *data;
    vkMapMemory(renderer->logical_device, staging_buffer_memory, 0, sizeof(vertices), 0, &data);
    memcpy(data, vertices, sizeof(vertices));
    vkUnmapMemory(renderer->logical_device, staging_buffer_memory);


    // Vertex buffer creation
    if (create_buffer(renderer, sizeof(vertices), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &renderer->vertex_buffer, &renderer->vertex_buffer_memory) != CReturnSuccess)
        return CReturnFailure;

    if (copy_buffer(renderer, staging_buffer, renderer->vertex_buffer, sizeof(vertices)) != CReturnSuccess)
        return CReturnFailure;


    // Staging buffer destruction
    vkFreeMemory(renderer->logical_device, staging_buffer_memory, NULL);
    vkDestroyBuffer(renderer->logical_device, staging_buffer, NULL);

    return CReturnSuccess;
}
