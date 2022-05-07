#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkPipelineData.h"
#include "shvulkan/shVkCheck.h"
#include "shvulkan/shVkMemoryInfo.h"

#include <stdlib.h>

#ifdef _MSC_VER
#pragma warning (disable: 6386)
#endif//_MSC_VER

void shDescriptorSetLayout(VkDevice device, const uint32_t descriptor_idx, const uint32_t binding, const VkDescriptorType descriptor_type, const VkShaderStageFlags shaderStageFlags, VkDescriptorSetLayoutBinding* p_binding, VkDescriptorSetLayout* p_descriptor_set_layout) {
	shVkAssert(p_binding != NULL, "invalid binding pointer");
	shVkAssert(p_descriptor_set_layout != NULL, "invalid descriptor set layout pointer");
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
	
	shVkAssertResult(
		vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, NULL, p_descriptor_set_layout),
		"error creating descriptor set layout"
	);
}

void shCreateDescriptorPool(VkDevice device, const uint32_t descriptor_idx, const uint32_t binding, const VkDescriptorType descriptor_type, VkDescriptorPool* p_descriptor_pool) {
	shVkAssert(p_descriptor_pool != NULL, "invalid descriptor pool pointer");
	VkDescriptorPoolSize descriptorPoolSize = {
		descriptor_type,
		1									//descriptorCount;
	};

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		1,												//maxSets;
		1,												//poolSizeCount;
		&descriptorPoolSize								//pPoolSizes;
	};

	shVkAssertResult(
		vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, NULL, p_descriptor_pool),
		"error creating descriptor pool"
	);
}

void shAllocateDescriptorSet(VkDevice device, const uint32_t descriptor_idx, const uint32_t binding, VkDescriptorType descriptor_type, VkDescriptorPool descriptor_pool, VkDescriptorSetLayout* p_descriptor_set_layout, VkDescriptorSet* p_descriptor_set, VkDescriptorBufferInfo* p_buffer_info, VkWriteDescriptorSet* p_write_descriptor_set) {
	shVkAssert(p_descriptor_set_layout != NULL, "invalid descriptor set layout pointer");
	shVkAssert(p_buffer_info != NULL, "invalid descriptor buffer info pointer");
	shVkAssert(p_descriptor_set != NULL, "invalid descriptor set pointer");
	shVkAssert(p_write_descriptor_set != NULL, "invalid write descriptor set pointer");
	
	VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,				//sType;
		NULL,														//pNext;
		descriptor_pool,											//descriptorPool;
		1,															//descriptorSetCount;
		(VkDescriptorSetLayout*)p_descriptor_set_layout				//pSetLayouts;
	};
	shVkAssertResult(
		vkAllocateDescriptorSets(device, &descriptor_set_allocate_info, p_descriptor_set),
		"error allocating descriptor set"
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
}

void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer ");
	p_pipeline->push_constant_range.offset		= offset;
	p_pipeline->push_constant_range.size		= size;
	p_pipeline->push_constant_range.stageFlags = shaderStageFlags;
}

void shCreateDescriptorBuffer(VkDevice device, VkBufferUsageFlagBits usage, const uint32_t descriptor_idx, const uint32_t size, const uint32_t max_size, VkDescriptorBufferInfo* p_buffer_info, VkBuffer* p_buffer) {
	shCreateBuffer(device, max_size, usage, p_buffer);
	VkDescriptorBufferInfo buffer_info = {
		*p_buffer,	//buffer;
		0,			//offset;
		size,		//range;
	};
	*p_buffer_info = buffer_info;
}

//void shGraphicsAllocateDescriptorUniformBuffers(VkDevice device, VkPhysicalDevice physical_device, ShVkPipeline* p_pipeline) {
//	for (uint8_t i = 0; i < p_pipeline->descriptor_count; i++) {
//		shAllocateMemory(device, physical_device,
//			(p_pipeline)->descriptor_buffers[i],
//			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//			&(p_pipeline)->descriptor_buffers_memory[i]
//		);
//	}
//}

void shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, VkShaderModule* p_shader_module) {
	shVkAssert(code != NULL, "invalid shader module code");
	shVkAssert(p_shader_module != NULL, "invalid shader module pointer");
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		size,											//codeSize;
		(const uint32_t*)(code)							//pCode;
	};

	shVkAssertResult(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, p_shader_module),
		"error creating shader module"
	);
}

void shCreateShaderStage(const VkDevice device, const VkShaderModule shader_module, const VkShaderStageFlagBits shader_stage_flag, VkPipelineShaderStageCreateInfo* p_shader_stage) {
	shVkAssert(p_shader_stage != NULL, "invalid shader stage pointer");
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

void shSetFixedStates(VkDevice device, const uint32_t surface_width, const uint32_t surface_height, ShFixedStateFlags flags, ShVkFixedStates* p_fixed_states) {
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
	shSetViewport(surface_width, surface_height, &p_fixed_states->viewport, &p_fixed_states->scissor, &p_fixed_states->viewport_state);
}

void shSetupGraphicsPipeline(VkDevice device, VkRenderPass render_pass, const ShVkFixedStates fStates, ShVkPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid graphics pipeline pointer");

	VkPipelineLayoutCreateInfo mainPipelineLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,								//sType;
		NULL,																		//pNext;
		0,																			//flags;
		p_pipeline->descriptor_count,													//setLayoutCount;
		p_pipeline->descriptor_set_layouts,											//pSetLayouts;
		0,																			//pushConstantRangeCount;
		NULL,																		//pPushConstantRanges;
	};

	if (p_pipeline->push_constant_range.size != 0) {
		mainPipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		mainPipelineLayoutCreateInfo.pPushConstantRanges = &p_pipeline->push_constant_range;
	}
	
	shVkAssertResult(
		vkCreatePipelineLayout(device, &mainPipelineLayoutCreateInfo, NULL, &p_pipeline->pipeline_layout),
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
		p_pipeline->shader_module_count,					//stageCount;
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
		p_pipeline->pipeline_layout,						//layout;
		render_pass,										//renderPass;
		0,													//subpass;
		0,													//basePipelineHandle;
		0													//basePipelineIndex;
	};

	shVkAssertResult(
		vkCreateGraphicsPipelines(device, 0, 1, &graphicsPipelineCreateInfo, NULL, &p_pipeline->pipeline),
		"error creating graphics pipeline"
	);
}

void shSetupComputePipeline(VkDevice device, ShVkPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid compute pipeline pointer");

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

	shVkAssertResult(
		vkCreatePipelineLayout(device, &pipeline_layout_create_info, NULL, &p_pipeline->pipeline_layout),
		"error creating pipeline layout"
	);

	VkComputePipelineCreateInfo pipeline_create_info = {
		VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		NULL,
		0,
		p_pipeline->shader_stages[0],
		p_pipeline->pipeline_layout,
	};

	shVkAssertResult(
		vkCreateComputePipelines(device, 0, 1, &pipeline_create_info, NULL, &p_pipeline->pipeline),
		"error creating pipeline layout"
	);
}

void shEndPipeline(ShVkPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid graphics pipeline pointer ");
	for (uint8_t descriptor_idx = 0; descriptor_idx < 32; descriptor_idx++) {
		p_pipeline->dynamic_descriptor_buffer_offsets[descriptor_idx] = 0;
	}
}

void shPipelineRelease(VkDevice device, ShVkPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid graphics pipeline pointer ");
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
}																

#ifdef __cplusplus
}
#endif//__cplusplus