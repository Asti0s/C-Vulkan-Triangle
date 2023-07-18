#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renderer.h"
#include "constants.h"

VkShaderModule create_shader_module (VKRenderer *renderer, const char *filename)
{
    // Read the shader file into a buffer
    FILE *file = fopen(filename, "rb");
    if (!file) {
        dprintf(2, "Error: Failed to open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *shader_buffer = malloc(length + 1);
    if (!shader_buffer)
        return NULL;

    size_t buffer_length = fread(shader_buffer, 1, length, file);
    if (buffer_length != (size_t) length) {
        fclose(file);
        free(shader_buffer);
        return NULL;
    }

    shader_buffer[buffer_length] = '\0';
    fclose(file);

    // Create the shader module
    VkShaderModuleCreateInfo create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = buffer_length;
    create_info.pCode = (uint32_t *) shader_buffer;

    VkShaderModule shader_module;
    if (vkCreateShaderModule(renderer->logical_device, &create_info, NULL, &shader_module) != VK_SUCCESS)
        return NULL;

    free(shader_buffer);

    return shader_module;
}

int create_graphics_pipeline(VKRenderer *renderer)
{
    // Shader stages
    VkShaderModule frag_shader = create_shader_module(renderer, "../shaders/frag.spv");
    VkShaderModule vert_shader = create_shader_module(renderer, "../shaders/vert.spv");
    if (vert_shader == NULL || frag_shader == NULL)
        return CReturnFailure;

    VkPipelineShaderStageCreateInfo vert_shader_stage_info = {0};
    vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_info.module = vert_shader;
    vert_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo frag_shader_stage_info = {0};
    frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_info.module = frag_shader;
    frag_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};


    // Vertex input
    VkPipelineVertexInputStateCreateInfo vertex_input_info = {0};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;


    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo input_assembly = {0};
    input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly.primitiveRestartEnable = VK_FALSE;


    // Viewport and scissor
    VkViewport viewport = {0};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) renderer->swapchain_extent.width;
    viewport.height = (float) renderer->swapchain_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {0};
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = renderer->swapchain_extent;

    VkPipelineViewportStateCreateInfo viewport_state = {0};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;


    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer = {0};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasClamp = VK_FALSE;


    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling = {0};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;


    // Color blending
    VkPipelineColorBlendAttachmentState color_blend_attachment = {0};
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
    VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo color_blending = {0};
    color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable = VK_FALSE;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &color_blend_attachment;


    // Pipeline layout
    VkPipelineLayoutCreateInfo pipeline_layout_info = {0};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    if (vkCreatePipelineLayout(renderer->logical_device, &pipeline_layout_info, NULL, &renderer->pipeline_layout) != VK_SUCCESS)
        return CReturnFailure;


    // Graphics pipeline
    VkGraphicsPipelineCreateInfo pipeline_info = {0};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.layout = renderer->pipeline_layout;
    pipeline_info.renderPass = renderer->render_pass;
    pipeline_info.subpass = 0;

    if (vkCreateGraphicsPipelines(renderer->logical_device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &renderer->graphics_pipeline) != VK_SUCCESS)
        return CReturnFailure;


    // Shader modules are no longer needed after the graphics pipeline has been created
    vkDestroyShaderModule(renderer->logical_device, frag_shader, NULL);
    vkDestroyShaderModule(renderer->logical_device, vert_shader, NULL);

    return CReturnSuccess;
}
