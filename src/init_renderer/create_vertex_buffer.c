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

int create_vertex_buffer (VKRenderer *renderer)
{
    // Create the vertex buffer
    VkBufferCreateInfo buffer_info = {0};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = sizeof(vertices);
    buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(renderer->logical_device, &buffer_info, NULL, &renderer->vertex_buffer) != VK_SUCCESS)
        return CReturnFailure;


    // Allocate the memory for the vertex buffer
    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(renderer->logical_device, renderer->vertex_buffer, &mem_requirements);

    VkMemoryAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = findMemoryType(renderer, mem_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(renderer->logical_device, &alloc_info, NULL, &renderer->vertex_buffer_memory) != VK_SUCCESS)
        return CReturnFailure;
    vkBindBufferMemory(renderer->logical_device, renderer->vertex_buffer, renderer->vertex_buffer_memory, 0);


    // Map the vertex buffer memory and copy the vertices into it
    void *data;
    vkMapMemory(renderer->logical_device, renderer->vertex_buffer_memory, 0, buffer_info.size, 0, &data);
    memcpy(data, vertices, buffer_info.size);
    vkUnmapMemory(renderer->logical_device, renderer->vertex_buffer_memory);

    return CReturnSuccess;
}
