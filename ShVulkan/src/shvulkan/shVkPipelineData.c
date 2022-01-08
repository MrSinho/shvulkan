#include "shvulkan/shVkCore.h"
#include "shvulkan/shVkPipelineData.h"
#include "shvulkan/shVkCheck.h"
#include "shvulkan/shVkMemoryInfo.h"

#include <stdlib.h>
#include <assert.h>

#ifdef _MSC_VER
#pragma warning (disable: 6386)
#endif//_MSC_VER

void shDescriptorSetLayout(ShVkCore* p_core, const uint32_t uniform_idx, const VkShaderStageFlags shaderStageFlags, ShVkPipelineData* p_pipe_data) {
	assert(p_pipe_data != NULL);
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {
		p_pipe_data->dynamic_uniform_buffers_size[uniform_idx] ? 1 : 0,	//binding;
		p_pipe_data->dynamic_uniform_buffers_size[uniform_idx] ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//descriptorType;
		1,											//descriptorCount;
		shaderStageFlags,							//stageFlags;
		NULL										//pImmutableSamplers;
	};
	
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		1,														//bindingCount;
		&descriptorSetLayoutBinding								//pBindings;
	};
	p_pipe_data->descriptor_set_layout_bindings[uniform_idx] = descriptorSetLayoutBinding;

	shCheckVkResult(
		vkCreateDescriptorSetLayout(p_core->device, &descriptorSetLayoutCreateInfo, NULL, &p_pipe_data->descriptor_set_layouts[uniform_idx]),
		"error creating descriptor set layout"
	);
}

extern void shCreateDescriptorPool(ShVkCore* p_core, const uint32_t uniform_idx, ShVkPipelineData* p_pipe_data) {
	assert(p_pipe_data != NULL);
	VkDescriptorPoolSize descriptorPoolSize = {
		p_pipe_data->dynamic_uniform_buffers_size[uniform_idx] ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//type;
		p_core->swapchain_image_count				//descriptorCount;
	};

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		p_core->swapchain_image_count,					//maxSets;
		1,												//poolSizeCount;
		&descriptorPoolSize								//pPoolSizes;
	};

	shCheckVkResult(
		vkCreateDescriptorPool(p_core->device, &descriptorPoolCreateInfo, NULL, &p_pipe_data->descriptor_pools[uniform_idx]),
		"error creating descriptor pool"
	);
}

void shAllocateDescriptorSet(ShVkCore* p_core, const uint32_t uniform_idx, ShVkPipelineData* p_pipe_data) {
	assert(p_pipe_data != NULL);
	VkDescriptorSetAllocateInfo allocateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,				//sType;
		NULL,														//pNext;
		p_pipe_data->descriptor_pools[uniform_idx],					//descriptorPool;
		1,															//descriptorSetCount;
		& p_pipe_data->descriptor_set_layouts[uniform_idx]			//pSetLayouts;
	};

	shCheckVkResult(
		vkAllocateDescriptorSets(p_core->device, &allocateInfo, p_pipe_data->dynamic_uniform_buffers_size[uniform_idx] ? &p_pipe_data->dynamic_descriptor_sets[uniform_idx] : &p_pipe_data->descriptor_sets[uniform_idx]),
		"error allocating descriptor set"
	);

	VkDescriptorBufferInfo descriptorBufferInfo = {
		p_pipe_data->uniform_buffers[uniform_idx],		//buffer;
		0,												//offset;
		p_pipe_data->dynamic_uniform_buffers_size[uniform_idx] ? p_pipe_data->dynamic_uniform_buffers_size[uniform_idx] : p_pipe_data->uniform_buffers_size[uniform_idx],	//range;
	};

	p_pipe_data->descriptor_buffer_infos[uniform_idx] = descriptorBufferInfo;

	VkWriteDescriptorSet writeDescriptorSet = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,															//sType;
		NULL,																							//pNext;
		p_pipe_data->dynamic_uniform_buffers_size[uniform_idx] ? p_pipe_data->dynamic_descriptor_sets[uniform_idx] : p_pipe_data->descriptor_sets[uniform_idx],														//dstSet;
		p_pipe_data->dynamic_uniform_buffers_size[uniform_idx] ? 1 : 0,									//dstBinding;
		0,																								//dstArrayElement;
		1,																								//descriptorCount;
		p_pipe_data->dynamic_uniform_buffers_size[uniform_idx] ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//descriptorType;
		NULL,																							//pImageInfo;
		&p_pipe_data->descriptor_buffer_infos[uniform_idx],												//pBufferInfo;
		NULL																							//pTexelBufferView;
	};
	if (p_pipe_data->uniform_buffers_size[uniform_idx]) {
		p_pipe_data->write_descriptor_sets[uniform_idx] = writeDescriptorSet;
		p_pipe_data->write_descriptor_sets[uniform_idx].pBufferInfo = &p_pipe_data->descriptor_buffer_infos[uniform_idx];
	}
	if (p_pipe_data->dynamic_uniform_buffers_size[uniform_idx]) {
		p_pipe_data->dynamic_write_descriptor_sets[uniform_idx] = writeDescriptorSet;
		p_pipe_data->dynamic_write_descriptor_sets[uniform_idx].pBufferInfo = &p_pipe_data->descriptor_buffer_infos[uniform_idx];
	}
}

void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkPipelineData* p_pipe_data) {
	assert(p_pipe_data != NULL);
	p_pipe_data->push_constant_range.offset		= offset;
	p_pipe_data->push_constant_range.size		= size;
	p_pipe_data->push_constant_range.stageFlags = shaderStageFlags;
}

void shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, VkShaderModule* p_shader_module) {
	assert(code != NULL && p_shader_module != NULL);
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		size,											//codeSize;
		(const uint32_t*)(code)							//pCode;
	};

	shCheckVkResult(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, p_shader_module),
		"error creating shader module"
	);
}

void shCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const VkShaderStageFlagBits stageFlag, VkPipelineShaderStageCreateInfo* p_shader_stage) {
	assert(p_shader_stage != NULL);
	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		stageFlag,												//stage;
		shModule,												//module;
		"main",													//pName;
		NULL,													//pSpecializationInfo;
	};

	*p_shader_stage = shaderStageCreateInfo;

}

void shSetVertexInputAttribute(const uint32_t location, VkFormat format, const uint32_t offset, const uint32_t size, ShVkFixedStates* p_fixed_states) {
	assert(p_fixed_states != NULL);
	VkVertexInputAttributeDescription vertex_input_attribute = {
		location,
		0,
		format,
		offset
	};
	p_fixed_states->vertex_input_attributes[location] = vertex_input_attribute;
	p_fixed_states->vertex_binding_description.stride += size;
	p_fixed_states->vertex_input_attribute_description_count++;
}

void shSetVertexInputState(VkVertexInputBindingDescription* p_vertex_binding, uint32_t vertex_input_attribute_count, VkVertexInputAttributeDescription* p_vertex_input_attributes, VkPipelineVertexInputStateCreateInfo* p_vertex_input_state) {
	assert(p_vertex_binding != NULL && p_vertex_input_state);

	p_vertex_binding->binding = 0;
	p_vertex_binding->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkPipelineVertexInputStateCreateInfo vertexInput = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,//sType;
		NULL,									//pNext;
		0,										//flags;
		1,										//vertexBindingDescriptionCount;
		p_vertex_binding,						//pVertexBindingDescriptions;
		vertex_input_attribute_count,			//vertexAttributeDescriptionCount;
		p_vertex_input_attributes,				//pVertexAttributeDescriptions;
	};
	*p_vertex_input_state = vertexInput;
}

void shCreateInputAssembly(const VkPrimitiveTopology primitive_topology, const VkBool32 primitive_restart_enable, VkPipelineInputAssemblyStateCreateInfo* p_input_assembly) {
	assert(p_input_assembly != NULL);
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,	//sType;
		NULL,															//pNext;
		0,																//flags;
		primitive_topology,												//topology;
		primitive_restart_enable										//primitiveRestartEnable;
	};																	
	*p_input_assembly = inputAssemblyStateCreateInfo;
}

void shCreateRasterizer(VkPipelineRasterizationStateCreateInfo * p_rasterizer) {
	assert(p_rasterizer != NULL);
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,	//sType;
		NULL,														//pNext;
		0,															//flags;
		VK_FALSE,													//depthClampEnable;
		VK_FALSE,													//rasterizerDiscardEnable;
		VK_POLYGON_MODE_FILL,										//polygonMode;
		VK_CULL_MODE_BACK_BIT,										//cullMode
		VK_FRONT_FACE_CLOCKWISE,									//frontFace
		VK_FALSE,													//depthBiasEnable 
		0.0f,														//depthBiasConstantFactor;
		0.0f,														//depthBiasClamp;
		0.0f,														//depthBiasSlopeFactor; 
		1.0f														//lineWidth;
	};

	*p_rasterizer = rasterizationStateCreateInfo;
}

void shSetMultisampleState(VkPipelineMultisampleStateCreateInfo * p_multisample_state) {
	assert(p_multisample_state != NULL);
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,	//sType;
		NULL,														//pNext;
		0,															//flags;
		VK_SAMPLE_COUNT_1_BIT,										//rasterizationSamples;
		VK_FALSE,													//sampleShadingEnable;
		1.0f,														//minSampleShading;
		NULL,														//pSampleMask;
		VK_FALSE,													//alphaToCoverageEnable;
		VK_FALSE													//alphaToOneEnable;
	};
	*p_multisample_state = multisampleStateCreateInfo;
}

void shColorBlendSettings(VkPipelineColorBlendAttachmentState *p_color_blend_attachment, VkPipelineColorBlendStateCreateInfo* p_color_blend_state) {
	assert(p_color_blend_attachment != NULL && p_color_blend_state != NULL);
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
		VK_FALSE,							//blendEnable;
		0.0f,								//srcColorBlendFactor;
		0.0f,								//dstColorBlendFactor;
		0.0f,								//colorBlendOp;
		0.0f,								//srcAlphaBlendFactor;
		0.0f,								//dstAlphaBlendFactor;
		VK_FALSE,							//alphaBlendOp;
		VK_COLOR_COMPONENT_R_BIT | 			
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT 			//colorWriteMask;
	};
	*p_color_blend_attachment = colorBlendAttachmentState;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		NULL,
		0,
		VK_FALSE,
		VK_LOGIC_OP_COPY,
		1,
		p_color_blend_attachment,
		{0.0f, 0.0f, 0.0f}
	};
	*p_color_blend_state = colorBlendStateCreateInfo;
}

void shSetViewport(const uint32_t width, const uint32_t height, VkViewport* p_viewport, VkRect2D* p_scissors, VkPipelineViewportStateCreateInfo* p_viewport_state) {
	assert(p_viewport != NULL && p_scissors != NULL && p_viewport_state != NULL);
	VkViewport viewport = {
		0.0f,					//x; 
		0.0f,					//y;
		(float)width,			//width;
		(float)height,			//height;
		0.0f, 					//minDepth;
		1.0f					//maxDepth;
	};
	*p_viewport = viewport;

	VkRect2D scissor = {
		{0, 0},							//offset
		{width, height}	//extent
	};
	*p_scissors = scissor;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		1, 														//viewportCount;
		p_viewport,												//pViewports;
		1,														//scissorCount;
		p_scissors												//pScissors;
	};
	*p_viewport_state = viewportStateCreateInfo;
}

void shSetFixedStates(ShVkCore* p_core, ShFixedStateFlags flags, ShVkFixedStates* p_fixed_states) {
	assert(p_fixed_states != NULL);
	shSetVertexInputState(&p_fixed_states->vertex_binding_description, p_fixed_states->vertex_input_attribute_description_count, p_fixed_states->vertex_input_attributes, &p_fixed_states->vertex_input_state_info);
	
	shCreateInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE, &p_fixed_states->input_assembly);
	if (flags & SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST) {
		p_fixed_states->input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	}
	if (flags & SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST) {
		p_fixed_states->input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	}

	shCreateRasterizer(&p_fixed_states->rasterizer);
	if (flags & SH_FIXED_STATES_POLYGON_MODE_WIREFRAME) {
		p_fixed_states->rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
	}
	if (flags & SH_FIXED_STATES_POLYGON_MODE_POINTS) {
		p_fixed_states->rasterizer.polygonMode = VK_POLYGON_MODE_POINT;
	}

	shSetMultisampleState(&p_fixed_states->multisample_state_info);
	shColorBlendSettings(&p_fixed_states->color_blend_attachment, &p_fixed_states->color_blend_state);
	shSetViewport(p_core->surface.width, p_core->surface.height, &p_fixed_states->viewport, &p_fixed_states->scissor, &p_fixed_states->viewport_state);
}

void shSetupGraphicsPipeline(ShVkCore* p_core, const ShVkFixedStates fStates, ShVkPipelineData* p_pipe_data) {
	assert(p_pipe_data != NULL);

	VkPipelineLayoutCreateInfo mainPipelineLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		p_pipe_data->uniform_buffer_count+ p_pipe_data->dynamic_uniform_buffer_count,				//setLayoutCount;
		p_pipe_data->descriptor_set_layouts,			//pSetLayouts;
		0,												//pushConstantRangeCount;
		NULL,											//pPushConstantRanges;
	};

	if (p_pipe_data->push_constant_range.size != 0) {
		mainPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		mainPipelineLayoutCreateInfo.pPushConstantRanges = &p_pipe_data->push_constant_range;
	}
	

	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,	//sType;
		NULL,														//pNext;
		0,															//flags;
		VK_TRUE,													//depthTestEnable;
		VK_TRUE,													//depthWriteEnable;
		VK_COMPARE_OP_LESS,											//depthCompareOp;
		VK_FALSE,													//depthBoundsTestEnable;
		VK_FALSE,													//stencilTestEnable;
		0, 															//front;
		0,															//back;
		0.0f,														//minDepthBounds;
		1.0f														//maxDepthBounds;
	};


	shCheckVkResult(
		vkCreatePipelineLayout(p_core->device, &mainPipelineLayoutCreateInfo, NULL, &p_pipe_data->main_pipeline_layout),
		"error creating main pipeline layout"
	);

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,	//sType;
		NULL,												//pNext;
		0,													//flags;
		p_pipe_data->shader_stage_count,					//stageCount;
		p_pipe_data->shader_stages,							//pStages;
		&fStates.vertex_input_state_info,					//pVertexInputState;
		&fStates.input_assembly,							//pInputAssemblyState;
		NULL,												//pTessellationState;
		&fStates.viewport_state,							//pViewportState;
		&fStates.rasterizer,								//pRasterizationState;
		&fStates.multisample_state_info,					//pMultisampleState;
		&depthStencilStateCreateInfo ,						//pDepthStencilState;
		&fStates.color_blend_state,							//pColorBlendState;
		NULL,												//pDynamicState;
		p_pipe_data->main_pipeline_layout,					//layout;
		p_core->render_pass,									//renderPass;
		0,													//subpass;
		0,													//basePipelineHandle;
		0													//basePipelineIndex;
	};

	shCheckVkResult(
		vkCreateGraphicsPipelines(p_core->device, 0, 1, &graphicsPipelineCreateInfo, NULL, &p_pipe_data->pipeline),
		"error creating graphics pipeline"
	);
}

void shEndPipeline(ShVkPipelineData* p_pipe_data) {
	for (uint8_t i = 0; i < 32; i++) {
		p_pipe_data->dynamic_uniform_buffers_offsets[i] = 0;
	}
}

void shDestroyPipeline(ShVkCore* p_core, ShVkPipelineData* p_pipe_data) {
	assert(p_pipe_data != NULL);
	vkDeviceWaitIdle(p_core->device);
	for (uint32_t i = 0; i < p_pipe_data->uniform_buffer_count + p_pipe_data->dynamic_uniform_buffer_count; i++) {
		vkDestroyDescriptorPool(p_core->device, p_pipe_data->descriptor_pools[i], NULL);
		vkDestroyDescriptorSetLayout(p_core->device, p_pipe_data->descriptor_set_layouts[i], NULL);
	}
	vkDestroyPipelineLayout(p_core->device, p_pipe_data->main_pipeline_layout, NULL);
	vkDestroyPipeline(p_core->device, p_pipe_data->pipeline, NULL);
	vkDestroyShaderModule(p_core->device, p_pipe_data->shader_modules[0], NULL);
	vkDestroyShaderModule(p_core->device, p_pipe_data->shader_modules[1], NULL);
}																