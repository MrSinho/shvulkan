#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkCore.h"
#include "shvulkan/shVkPipelineData.h"
#include "shvulkan/shVkCheck.h"
#include "shvulkan/shVkMemoryInfo.h"

#include <stdlib.h>

#ifdef _MSC_VER
#pragma warning (disable: 6386 6011)
#endif//_MSC_VER

void shDescriptorSetLayout(ShVkCore* p_core, const uint32_t uniform_idx, const VkShaderStageFlags shaderStageFlags, ShVkGraphicsPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid arguments");
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {
		p_pipeline->dynamic_uniforms[uniform_idx],																					//binding;
		p_pipeline->dynamic_uniforms[uniform_idx] ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//descriptorType;
		1,																						//descriptorCount;
		shaderStageFlags,																		//stageFlags;
		NULL																					//pImmutableSamplers;
	};
	
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		1,														//bindingCount;
		&descriptorSetLayoutBinding								//pBindings;
	};
	p_pipeline->descriptor_set_layout_bindings[uniform_idx] = descriptorSetLayoutBinding;
	shVkAssertResult(
		vkCreateDescriptorSetLayout(p_core->device, &descriptorSetLayoutCreateInfo, NULL, &p_pipeline->descriptor_set_layouts[uniform_idx]),
		"error creating descriptor set layout"
	);
}

extern void shCreateDescriptorPool(ShVkCore* p_core, const uint32_t uniform_idx, ShVkGraphicsPipeline* p_pipeline) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer ");
	VkDescriptorPoolSize descriptorPoolSize = {
		p_pipeline->dynamic_uniforms[uniform_idx] ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,	//p_pipeline->dynamic_uniforms[uniform_idx];
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

	shVkAssertResult(
		vkCreateDescriptorPool(p_core->device, &descriptorPoolCreateInfo, NULL, &p_pipeline->descriptor_pools[uniform_idx]),
		"error creating descriptor pool"
	);
}

void shAllocateDescriptorSet(ShVkCore* p_core, const uint32_t uniform_idx, ShVkGraphicsPipeline* p_pipeline) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer ");
	VkDescriptorSetAllocateInfo allocateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,				//sType;
		NULL,														//pNext;
		p_pipeline->descriptor_pools[uniform_idx],					//descriptorPool;
		1,															//descriptorSetCount;
		&p_pipeline->descriptor_set_layouts[uniform_idx]			//pSetLayouts;
	};
	shVkAssertResult(
		vkAllocateDescriptorSets(p_core->device, &allocateInfo, &p_pipeline->descriptor_sets[uniform_idx]),
		"error allocating descriptor set"
	);

	VkDescriptorBufferInfo descriptorBufferInfo = {
		p_pipeline->uniform_buffers[uniform_idx],		//buffer;
		0,														//offset;
		p_pipeline->uniform_buffers_size[uniform_idx],	//range;
	};
	p_pipeline->descriptor_buffer_infos[uniform_idx] = descriptorBufferInfo;


	VkWriteDescriptorSet writeDescriptorSet = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,													//sType;
		NULL,																					//pNext;
		p_pipeline->descriptor_sets[uniform_idx],												//dstSet;
		p_pipeline->dynamic_uniforms[uniform_idx],												//dstBinding;
		0,																						//dstArrayElement;
		1,																						//descriptorCount;
		p_pipeline->dynamic_uniforms[uniform_idx] ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,		//descriptorType;
		NULL,																					//pImageInfo;
		&p_pipeline->descriptor_buffer_infos[uniform_idx],										//pBufferInfo;
		NULL																					//pTexelBufferView;
	};
	p_pipeline->write_descriptor_sets[uniform_idx] = writeDescriptorSet;
}

void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkGraphicsPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer ");
	p_pipeline->push_constant_range.offset		= offset;
	p_pipeline->push_constant_range.size		= size;
	p_pipeline->push_constant_range.stageFlags = shaderStageFlags;
}

void shCreateUniformBuffer(ShVkCore* p_core, const uint32_t uniform_idx, const uint32_t size, ShVkGraphicsPipeline* p_pipeline) {
	shCreateBuffer((p_core)->device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &(p_pipeline)->uniform_buffers[uniform_idx]);
	p_pipeline->uniform_buffers_size[uniform_idx] = size;
	p_pipeline->uniform_count++;
}

void shCreateDynamicUniformBuffer(ShVkCore* p_core, const uint32_t uniform_idx, const uint32_t size, ShVkGraphicsPipeline* p_pipeline) {
	shCreateBuffer((p_core)->device, SH_MAX_UNIFORM_BUFFER_SIZE, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &(p_pipeline)->uniform_buffers[uniform_idx]);
	p_pipeline->uniform_buffers_size[uniform_idx] = size;
	p_pipeline->dynamic_uniforms[uniform_idx] = 1;
	p_pipeline->uniform_count++;
}

void shAllocateUniformBuffers(ShVkCore* p_core, ShVkGraphicsPipeline* p_pipeline) {
	for (uint8_t i = 0; i < p_pipeline->uniform_count; i++) {
		shAllocateMemory((p_core)->device, (p_core)->physical_device,
			(p_pipeline)->uniform_buffers[i],
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&(p_pipeline)->uniform_buffers_memory[i]
		);
	}
}

void shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, ShVkGraphicsPipeline* p_pipeline) {
	shVkAssert(code != NULL, "invalid shader module code ");
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer ");
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		size,											//codeSize;
		(const uint32_t*)(code)							//pCode;
	};

	shVkAssertResult(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, &p_pipeline->shader_modules[p_pipeline->shader_module_count]),
		"error creating shader module"
	);
	p_pipeline->shader_module_count++;
}

void shCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const VkShaderStageFlagBits stageFlag, ShVkGraphicsPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer ");
	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,	//sType;
		NULL,													//pNext;
		0,														//flags;
		stageFlag,												//stage;
		shModule,												//module;
		"main",													//pName;
		NULL,													//pSpecializationInfo;
	};

	p_pipeline->shader_stages[p_pipeline->shader_stage_count] = shaderStageCreateInfo;
	p_pipeline->shader_stage_count++;
}

void shSetVertexInputAttribute(const uint32_t location, VkFormat format, const uint32_t offset, const uint32_t size, ShVkFixedStates* p_fixed_states) {
	shVkAssert(p_fixed_states != NULL, "invalid fixed states pointer ");
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
	shVkAssert(p_vertex_binding != NULL, "invalid vertex input binding description pointer ");
	shVkAssert(p_vertex_input_attributes != NULL, "invalid vertex input attribute description pointer ");
	shVkAssert(p_vertex_input_state != NULL, "invalid vertex input state info pointer ");

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
	shVkAssert(p_input_assembly != NULL, "invalid input assembly state info pointer ");
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
	shVkAssert(p_rasterizer != NULL, "invalid rasterizer pointer ");
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
	shVkAssert(p_multisample_state != NULL, "invalid multisample state info pointer ");
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
	shVkAssert(p_color_blend_attachment != NULL, "invalid color blend attachment state pointer ");
	shVkAssert(p_color_blend_state != NULL, "invalid color blend attachment state info pointer ");
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
	shVkAssert(p_viewport != NULL, "invalid viewport pointer ");
	shVkAssert(p_scissors != NULL, "invalid scissors pointer ");
	shVkAssert(p_viewport_state != NULL, "invalid viewport state pointer ");
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
	shVkAssert(p_core != NULL, "invalid core pointer");
	shVkAssert(p_fixed_states != NULL, "invalid fixed states pointer ");
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

void shCreateDescriptorPools(ShVkCore* p_core, ShVkGraphicsPipeline* p_pipeline) {
	for (uint8_t i = 0; i < p_pipeline->uniform_count; i++) {
		shCreateDescriptorPool(p_core, i, p_pipeline);
	}
}

void shAllocateDescriptorSets(ShVkCore* p_core, ShVkGraphicsPipeline* p_pipeline) {
	for (uint8_t i = 0; i < p_pipeline->uniform_count; i++) {
		shAllocateDescriptorSet(p_core, i, p_pipeline);
	}
}

void shSetupGraphicsPipeline(ShVkCore* p_core, const ShVkFixedStates fStates, ShVkGraphicsPipeline* p_pipeline) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer");

	VkPipelineLayoutCreateInfo mainPipelineLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,								//sType;
		NULL,																		//pNext;
		0,																			//flags;
		p_pipeline->uniform_count,													//setLayoutCount;
		p_pipeline->descriptor_set_layouts,											//pSetLayouts;
		0,																			//pushConstantRangeCount;
		NULL,																		//pPushConstantRanges;
	};

	if (p_pipeline->push_constant_range.size != 0) {
		mainPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		mainPipelineLayoutCreateInfo.pPushConstantRanges = &p_pipeline->push_constant_range;
	}
	
	shVkAssertResult(
		vkCreatePipelineLayout(p_core->device, &mainPipelineLayoutCreateInfo, NULL, &p_pipeline->main_pipeline_layout),
		"error creating main pipeline layout"
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
		p_pipeline->shader_stage_count,						//stageCount;
		p_pipeline->shader_stages,							//pStages;
		&fStates.vertex_input_state_info,					//pVertexInputState;
		&fStates.input_assembly,							//pInputAssemblyState;
		NULL,												//pTessellationState;
		&fStates.viewport_state,							//pViewportState;
		&fStates.rasterizer,								//pRasterizationState;
		&fStates.multisample_state_info,					//pMultisampleState;
		&depthStencilStateCreateInfo ,						//pDepthStencilState;
		&fStates.color_blend_state,							//pColorBlendState;
		NULL,												//pDynamicState;
		p_pipeline->main_pipeline_layout,					//layout;
		p_core->render_pass,								//renderPass;
		0,													//subpass;
		0,													//basePipelineHandle;
		0													//basePipelineIndex;
	};

	shVkAssertResult(
		vkCreateGraphicsPipelines(p_core->device, 0, 1, &graphicsPipelineCreateInfo, NULL, &p_pipeline->pipeline),
		"error creating graphics pipeline"
	);
}

void shEndPipeline(ShVkGraphicsPipeline* p_pipeline) {
	for (uint8_t i = 0; i < 32; i++) {
		p_pipeline->uniform_buffers_offsets[i] = 0;
	}
}

void shDestroyPipeline(ShVkCore* p_core, ShVkGraphicsPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer ");
	vkDeviceWaitIdle(p_core->device);
	for (uint32_t i = 0; i < p_pipeline->uniform_count; i++) {
		if (p_pipeline->descriptor_sets[i] != NULL) {
			vkDestroyDescriptorPool(p_core->device, p_pipeline->descriptor_pools[i], NULL);
			vkDestroyDescriptorSetLayout(p_core->device, p_pipeline->descriptor_set_layouts[i], NULL);
		}
	}
	vkDestroyPipelineLayout(p_core->device, p_pipeline->main_pipeline_layout, NULL);
	vkDestroyPipeline(p_core->device, p_pipeline->pipeline, NULL);
	vkDestroyShaderModule(p_core->device, p_pipeline->shader_modules[0], NULL);
	vkDestroyShaderModule(p_core->device, p_pipeline->shader_modules[1], NULL);
}																

#ifdef __cplusplus
}
#endif//__cplusplus