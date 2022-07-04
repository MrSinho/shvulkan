#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkPipelineData.h"
#include "shvulkan/shVkCheck.h"
#include "shvulkan/shVkMemoryInfo.h"

#include <stdlib.h>



uint8_t shDescriptorSetLayout(VkDevice device, const uint32_t binding, const VkDescriptorType descriptor_type, const VkShaderStageFlags shaderStageFlags, VkDescriptorSetLayoutBinding* p_binding, VkDescriptorSetLayout* p_descriptor_set_layout) {
	shVkError(p_binding == NULL, "invalid binding pointer", return 0);
	shVkError(p_descriptor_set_layout == NULL, "invalid descriptor set layout pointer", return 0);
	VkDescriptorSetLayoutBinding descriptor_set_layout_binding = {
		binding,																					//binding;
		//p_pipeline->dynamic_uniforms[descriptor_idx] ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//descriptorType;
		descriptor_type,
		1,																						//descriptorCount;
		shaderStageFlags,																		//stageFlags;
		NULL																					//pImmutableSamplers;
	};
	*p_binding = descriptor_set_layout_binding;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		1,														//bindingCount;
		p_binding												//pBindings;
	};
	
	shVkResultError(
		vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, NULL, p_descriptor_set_layout),
		"error creating descriptor set layout", return 0
	);

    return 1;
}

uint8_t shCreateDescriptorPool(VkDevice device, const VkDescriptorType descriptor_type, VkDescriptorPool* p_descriptor_pool) {
	shVkError(p_descriptor_pool == NULL, "invalid descriptor pool pointer", return 0);
	VkDescriptorPoolSize descriptor_pool_size = {
		descriptor_type,
		1									//descriptorCount;
	};

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		1,												//maxSets;
		1,												//poolSizeCount;
		&descriptor_pool_size							//pPoolSizes;
	};

	shVkResultError(
		vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, NULL, p_descriptor_pool),
		"error creating descriptor pool", return 0
	);

    return 1;
}

uint8_t shAllocateDescriptorSet(VkDevice device, const uint32_t binding, VkDescriptorType descriptor_type, VkDescriptorSetLayout* p_descriptor_set_layout, VkDescriptorPool descriptor_pool, VkDescriptorSet* p_descriptor_set, VkDescriptorBufferInfo* p_buffer_info, VkWriteDescriptorSet* p_write_descriptor_set) {
	shVkError(p_descriptor_set_layout == NULL, "invalid descriptor set layout pointer", return 0);
	shVkError(p_buffer_info == NULL, "invalid descriptor buffer info pointer", return 0);
	shVkError(p_descriptor_set == NULL, "invalid descriptor set pointer", return 0);
	shVkError(p_write_descriptor_set == NULL, "invalid write descriptor set pointer", return 0);
	
	VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,				//sType;
		NULL,														//pNext;
		descriptor_pool,											//descriptorPool;
		1,															//descriptorSetCount;
		(VkDescriptorSetLayout*)p_descriptor_set_layout				//pSetLayouts;
	};
	shVkResultError(
		vkAllocateDescriptorSets(device, &descriptor_set_allocate_info, p_descriptor_set),
		"error allocating descriptor set", return 0
	);

	VkWriteDescriptorSet write_descriptor_set = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,	//sType;
		NULL,									//pNext;
		*p_descriptor_set,						//dstSet;
		binding,								//dstBinding
		0,										//dstArrayElement;
		1,										//descriptorCount;
		descriptor_type,						//descriptorType
		NULL,									//pImageInfo;
		p_buffer_info,							//pBufferInfo;
		NULL									//pTexelBufferView;
	};
	*p_write_descriptor_set = write_descriptor_set;

    return 1;
}

uint8_t shSetPushConstants(const VkShaderStageFlags shader_stage_flags, const uint32_t offset, const uint32_t size, VkPushConstantRange* p_constant_range) {
	shVkError(p_constant_range == NULL, "invalid pipeline pointer", return 0);
	p_constant_range->offset		= offset;
	p_constant_range->size			= size;
	p_constant_range->stageFlags	= shader_stage_flags;

	return 1;
}

uint8_t shCreateDescriptorBuffer(VkDevice device, VkBufferUsageFlags usage, const uint32_t descriptor_idx, const uint32_t size, const uint32_t max_size, VkDescriptorBufferInfo* p_buffer_info, VkBuffer* p_buffer) {
	shVkError(device		== NULL, "invalid device handle",		return 0);
	shVkError(p_buffer_info == NULL, "invalid buffer info memory",	return 0);
	shVkError(p_buffer		== NULL, "invalid buffer pointer",		return 0)

	shVkError(
		shCreateBuffer(device, max_size, usage, p_buffer) == 0,
		"failed creating descriptor buffer",
		return 0
	);

	VkDescriptorBufferInfo buffer_info = {
		*p_buffer,	//buffer;
		0,			//offset;
		size,		//range;
	};
	*p_buffer_info = buffer_info;

	return 1;
}

//uint8_t shGraphicsAllocateDescriptorUniformBuffers(VkDevice device, VkPhysicalDevice physical_device, ShVkPipeline* p_pipeline) {
//	for (uint8_t i = 0; i < p_pipeline->descriptor_count; i++) {
//		shAllocateMemory(device, physical_device,
//			(p_pipeline)->descriptor_buffers[i],
//			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//			&(p_pipeline)->descriptor_buffers_memory[i]
//		);
//	}
//
//    return 1;
//}

uint8_t shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, VkShaderModule* p_shader_module) {
	shVkError(code == NULL, "invalid shader module code", return 0);
	shVkError(p_shader_module == NULL, "invalid shader module pointer", return 0);
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		size,											//codeSize;
		(const uint32_t*)(code)							//pCode;
	};

	shVkResultError(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, p_shader_module),
		"error creating shader module", return 0
	);

    return 1;
}

uint8_t shCreateShaderStage(const VkDevice device, const VkShaderModule shader_module, const VkShaderStageFlags shader_stage_flag, VkPipelineShaderStageCreateInfo* p_shader_stage) {
	shVkError(p_shader_stage == NULL, "invalid shader stage pointer", return 0);
	VkPipelineShaderStageCreateInfo shader_stage_create_info = {
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		shader_stage_flag,										//stage;
		shader_module,											//module;
		"main",													//pName;
		NULL,													//pSpecializationInfo;
	};

	*p_shader_stage = shader_stage_create_info;

    return 1;
}

uint8_t shSetVertexInputAttribute(const uint32_t location, VkFormat format, const uint32_t offset, const uint32_t size, ShVkFixedStates* p_fixed_states) {
	shVkError(p_fixed_states == NULL, "invalid fixed states pointer", return 0);
	VkVertexInputAttributeDescription vertex_input_attribute = {
		location,
		0,
		format,
		offset
	};
	p_fixed_states->vertex_input_attributes[location] = vertex_input_attribute;
	p_fixed_states->vertex_binding_description.stride += size;
	p_fixed_states->vertex_input_attribute_description_count++;

    return 1;
}

uint8_t shSetVertexInputRate(const VkVertexInputRate input_rate, const uint32_t binding, VkVertexInputBindingDescription* p_vertex_binding) {
	shVkError(p_vertex_binding == NULL, "invalid vertex input binding description pointer", return 0);
	
	p_vertex_binding->binding = binding;
	p_vertex_binding->inputRate = input_rate;

    return 1;
}

uint8_t shSetVertexInputState(VkVertexInputBindingDescription* p_vertex_binding, uint32_t vertex_input_attribute_count, VkVertexInputAttributeDescription* p_vertex_input_attributes, VkPipelineVertexInputStateCreateInfo* p_vertex_input_state) {
	shVkError(p_vertex_binding == NULL, "invalid vertex input binding description pointer", return 0);
	shVkError(p_vertex_input_attributes == NULL, "invalid vertex input attribute description pointer", return 0);
	shVkError(p_vertex_input_state == NULL, "invalid vertex input state info pointer", return 0);

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

    return 1;
}

uint8_t shCreateInputAssembly(const VkPrimitiveTopology primitive_topology, const VkBool32 primitive_restart_enable, VkPipelineInputAssemblyStateCreateInfo* p_input_assembly) {
	shVkError(p_input_assembly == NULL, "invalid input assembly state info pointer", return 0);
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,	//sType;
		NULL,															//pNext;
		0,																//flags;
		primitive_topology,												//topology;
		primitive_restart_enable										//primitiveRestartEnable;
	};																	
	*p_input_assembly = inputAssemblyStateCreateInfo;

    return 1;
}

uint8_t shCreateRasterizer(VkPolygonMode polygon_mode, VkPipelineRasterizationStateCreateInfo* p_rasterizer) {
	shVkError(p_rasterizer == NULL, "invalid rasterizer pointer", return 0);
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,	//sType;
		NULL,														//pNext;
		0,															//flags;
		VK_FALSE,													//depthClampEnable;
		VK_FALSE,													//rasterizerDiscardEnable;
		polygon_mode,												//polygonMode;
		VK_CULL_MODE_BACK_BIT,										//cullMode
		VK_FRONT_FACE_CLOCKWISE,									//frontFace
		VK_FALSE,													//depthBiasEnable 
		0.0f,														//depthBiasConstantFactor;
		0.0f,														//depthBiasClamp;
		0.0f,														//depthBiasSlopeFactor; 
		1.0f														//lineWidth;
	};

	*p_rasterizer = rasterizationStateCreateInfo;

    return 1;
}

uint8_t shSetMultisampleState(VkPipelineMultisampleStateCreateInfo * p_multisample_state) {
	shVkError(p_multisample_state == NULL, "invalid multisample state info pointer", return 0);
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

    return 1;
}

uint8_t shColorBlendSettings(VkPipelineColorBlendAttachmentState *p_color_blend_attachment, VkPipelineColorBlendStateCreateInfo* p_color_blend_state) {
	shVkError(p_color_blend_attachment == NULL, "invalid color blend attachment state pointer", return 0);
	shVkError(p_color_blend_state == NULL, "invalid color blend attachment state info pointer", return 0);
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

    return 1;
}

uint8_t shSetViewport(const uint32_t width, const uint32_t height, VkViewport* p_viewport, VkRect2D* p_scissors, VkPipelineViewportStateCreateInfo* p_viewport_state) {
	shVkError(p_viewport == NULL, "invalid viewport pointer", return 0);
	shVkError(p_scissors == NULL, "invalid scissors pointer", return 0);
	shVkError(p_viewport_state == NULL, "invalid viewport state pointer", return 0);
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

    return 1;
}

uint8_t shSetFixedStates(VkDevice device, const uint32_t surface_width, const uint32_t surface_height, VkPrimitiveTopology primitive, VkPolygonMode polygon_mode, ShVkFixedStates* p_fixed_states) {
	shVkError(p_fixed_states == NULL, "invalid fixed states pointer", return 0);
	
	shCreateInputAssembly(primitive, VK_FALSE, &p_fixed_states->input_assembly);

	shCreateRasterizer(polygon_mode, &p_fixed_states->rasterizer);

	shSetMultisampleState(&p_fixed_states->multisample_state_info);
	shColorBlendSettings(&p_fixed_states->color_blend_attachment, &p_fixed_states->color_blend_state);
	shSetViewport(surface_width, surface_height, &p_fixed_states->viewport, &p_fixed_states->scissor, &p_fixed_states->viewport_state);

    return 1;
}

uint8_t shSetupGraphicsPipeline(VkDevice device, VkRenderPass render_pass, const ShVkFixedStates fixed_states, ShVkPipeline* p_pipeline) {
	shVkError(p_pipeline == NULL, "invalid graphics pipeline pointer", return 0);

	VkPipelineLayoutCreateInfo mainPipelineLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,								//sType;
		NULL,																		//pNext;
		0,																			//flags;
		p_pipeline->descriptor_count,												//setLayoutCount;
		p_pipeline->descriptor_set_layouts,											//pSetLayouts;
		0,																			//pushConstantRangeCount;
		NULL																		//pPushConstantRanges;
	};

	if (p_pipeline->push_constant_range.size != 0) {
		mainPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		mainPipelineLayoutCreateInfo.pPushConstantRanges = &p_pipeline->push_constant_range;
	}
	
	shVkResultError(
		vkCreatePipelineLayout(device, &mainPipelineLayoutCreateInfo, NULL, &p_pipeline->pipeline_layout),
		"error creating main pipeline layout", return 0
	);

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

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,	//sType;
		NULL,												//pNext;
		0,													//flags;
		p_pipeline->shader_module_count,					//stageCount;
		p_pipeline->shader_stages,							//pStages;
		&fixed_states.vertex_input_state_info,				//pVertexInputState;
		&fixed_states.input_assembly,						//pInputAssemblyState;
		NULL,												//pTessellationState;
		&fixed_states.viewport_state,						//pViewportState;
		&fixed_states.rasterizer,							//pRasterizationState;
		&fixed_states.multisample_state_info,				//pMultisampleState;
		&depthStencilStateCreateInfo ,						//pDepthStencilState;
		&fixed_states.color_blend_state,					//pColorBlendState;
		NULL,												//pDynamicState;
		p_pipeline->pipeline_layout,						//layout;
		render_pass,										//renderPass;
		0,													//subpass;
		0,													//basePipelineHandle;
		0													//basePipelineIndex;
	};

	shVkResultError(
		vkCreateGraphicsPipelines(device, 0, 1, &graphicsPipelineCreateInfo, NULL, &p_pipeline->pipeline),
		"error creating graphics pipeline", return 0
	);

    return 1;
}

uint8_t shSetupComputePipeline(VkDevice device, ShVkPipeline* p_pipeline) {
	shVkError(p_pipeline == NULL, "invalid compute pipeline pointer", return 0);

	VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		NULL,
		0,
		p_pipeline->descriptor_count,
		p_pipeline->descriptor_set_layouts,
		0,
		NULL
	};

	if (p_pipeline->push_constant_range.size != 0) {
		pipeline_layout_create_info.pushConstantRangeCount = 1;
		pipeline_layout_create_info.pPushConstantRanges = &p_pipeline->push_constant_range;
	}

	shVkResultError(
		vkCreatePipelineLayout(device, &pipeline_layout_create_info, NULL, &p_pipeline->pipeline_layout),
		"error creating pipeline layout", return 0
	);

	VkComputePipelineCreateInfo pipeline_create_info = {
		VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		NULL,
		0,
		p_pipeline->shader_stages[0],
		p_pipeline->pipeline_layout,
	};

	shVkResultError(
		vkCreateComputePipelines(device, 0, 1, &pipeline_create_info, NULL, &p_pipeline->pipeline),
		"error creating pipeline layout", return 0
	);

    return 1;
}

uint8_t shEndPipeline(ShVkPipeline* p_pipeline) {
	shVkError(p_pipeline == NULL, "invalid graphics pipeline pointer", return 0);
	memset(p_pipeline->dynamic_descriptor_buffer_offsets, 0, sizeof(p_pipeline->dynamic_descriptor_buffer_offsets));

    return 1;
}

uint8_t shPipelineRelease(VkDevice device, ShVkPipeline* p_pipeline) {
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);
	vkDeviceWaitIdle(device);
	for (uint32_t i = 0; i < p_pipeline->descriptor_count; i++) {
		if (p_pipeline->descriptor_sets[i] != NULL) {
			vkDestroyDescriptorPool(device, p_pipeline->descriptor_pools[i], NULL);
			vkDestroyDescriptorSetLayout(device, p_pipeline->descriptor_set_layouts[i], NULL);
		}
	}
	vkDestroyPipelineLayout(device, p_pipeline->pipeline_layout, NULL);
	vkDestroyPipeline(device, p_pipeline->pipeline, NULL);
	vkDestroyShaderModule(device, p_pipeline->shader_modules[0], NULL);
	vkDestroyShaderModule(device, p_pipeline->shader_modules[1], NULL);
	//shFreeVkPipeline(pp_pipeline);

	return 1;
}																

uint8_t shPipelineLinkDescriptorBuffer(const uint32_t descriptor_idx, VkBuffer buffer, const uint32_t offset, const uint32_t size, ShVkPipeline* p_pipeline) {
	shVkError(p_pipeline == NULL, "invalid pipeline pointer", return 0);
	shVkError(buffer == NULL, "invalid buffer pointer", return 0);
	VkDescriptorBufferInfo buffer_info = {
		buffer,	//buffer;
		offset,	//offset;
		size,	//range;
	};
	p_pipeline->descriptor_buffer_infos[descriptor_idx] = buffer_info;
	p_pipeline->descriptor_count++;

    return 1;
}

uint8_t shPipelineCreateDescriptorBuffer(const VkDevice device, const VkBufferUsageFlags buffer_usage_flag, const uint32_t descriptor_idx, const uint32_t size, ShVkPipeline* p_pipeline) {
	shVkError(device == NULL, "invalid device handle", return 0);
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);
	shCreateDescriptorBuffer(device, buffer_usage_flag, descriptor_idx, size, size, &p_pipeline->descriptor_buffer_infos[descriptor_idx], &p_pipeline->descriptor_buffers[descriptor_idx]);
	p_pipeline->descriptor_count++;

	return 0;
}

uint8_t shPipelineCreateDynamicDescriptorBuffer(const VkDevice device, const VkBufferUsageFlags buffer_usage_flag, const uint32_t descriptor_idx, const uint32_t size, const uint32_t max_bindings, ShVkPipeline* p_pipeline) {
	shVkError(device == NULL, "invalid device handle", return 0);
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);
	shCreateDescriptorBuffer(device, buffer_usage_flag, descriptor_idx, size, size * max_bindings, &p_pipeline->descriptor_buffer_infos[descriptor_idx], &p_pipeline->descriptor_buffers[descriptor_idx]);
	p_pipeline->descriptor_count++;

	return 0;
}

uint8_t shPipelineAllocateDescriptorBuffersMemory(const VkDevice device, const VkPhysicalDevice physical_device, VkMemoryPropertyFlags flags, ShVkPipeline* p_pipeline) {
	shVkError(device == NULL, "invalid device handle", return 0);
	shVkError(physical_device == NULL, "invalid physical device device", return 0);
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return 0);

	for (uint32_t descriptor_idx = 0; descriptor_idx < p_pipeline->descriptor_count; descriptor_idx++) {
		shPipelineAllocateDescriptorBufferMemory(device, physical_device, flags, descriptor_idx, p_pipeline);
	}

	return 0;
}

uint8_t shPipelineCreateShaderStage(const VkDevice device, VkShaderStageFlags shader_stage_flag, ShVkPipeline* p_pipeline) {
	shCreateShaderStage(device, p_pipeline->shader_modules[p_pipeline->shader_module_count], shader_stage_flag, &p_pipeline->shader_stages[p_pipeline->shader_module_count]);
	p_pipeline->shader_module_count++;

    return 1;
}

uint8_t shPipelineBindDynamicDescriptorSet(const VkCommandBuffer cmd_buffer, const uint32_t descriptor_idx, const VkPipelineBindPoint bind_point, ShVkPipeline* p_pipeline) {
	vkCmdBindDescriptorSets(cmd_buffer,
		bind_point,
		p_pipeline->pipeline_layout, descriptor_idx, 1,
		&(p_pipeline)->descriptor_sets[descriptor_idx],
		1,
		&(p_pipeline)->dynamic_descriptor_buffer_offsets[descriptor_idx]
	);
	(p_pipeline)->dynamic_descriptor_buffer_offsets[descriptor_idx] += (uint32_t)p_pipeline->descriptor_buffer_infos[descriptor_idx].range;

    return 1;
}

uint8_t shPipelineBindDynamicDescriptorSets(const VkCommandBuffer cmd_buffer, const uint32_t first_descriptor, const uint32_t descriptor_count, const VkPipelineBindPoint bind_point, ShVkPipeline* p_pipeline) {
	vkCmdBindDescriptorSets(cmd_buffer,
		bind_point,
		p_pipeline->pipeline_layout, 
		first_descriptor,
		descriptor_count,
		&(p_pipeline)->descriptor_sets[first_descriptor],
		2,
		&(p_pipeline)->dynamic_descriptor_buffer_offsets[first_descriptor]
	);
	for (uint32_t i = 0; i < descriptor_count; i++) {
		(p_pipeline)->dynamic_descriptor_buffer_offsets[i] += (uint32_t)p_pipeline->descriptor_buffer_infos[i].range;
	}

	return 1;
}

#ifdef __cplusplus
}
#endif//__cplusplus