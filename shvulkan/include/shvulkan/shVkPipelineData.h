#ifndef SH_PIPELINE_DATA_H
#define SH_PIPELINE_DATA_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>
#include <shvulkan/shVkCheck.h>

#include <stdlib.h>



typedef struct ShVkFixedStates {
	/*Shader inputs*/
	VkVertexInputBindingDescription			vertex_binding_description;
	uint32_t								vertex_input_attribute_description_count;
	VkVertexInputAttributeDescription		vertex_input_attributes[32];
	VkPipelineVertexInputStateCreateInfo	vertex_input_state_info;
	VkPipelineInputAssemblyStateCreateInfo	input_assembly;
	/*Viewport*/
	VkViewport								viewport;
	VkRect2D								scissor;
	VkPipelineViewportStateCreateInfo		viewport_state;
	/*Rasterizer*/
	VkPipelineRasterizationStateCreateInfo	rasterizer;
	/**/
	VkPipelineColorBlendAttachmentState		color_blend_attachment;
	VkPipelineColorBlendStateCreateInfo		color_blend_state;
	/**/
	VkPipelineMultisampleStateCreateInfo	multisample_state_info;
} ShVkFixedStates;

typedef struct ShVkPipeline {
	/*Shaders*/
	uint32_t							shader_module_count;
	VkShaderModule						shader_modules[16];
	VkPipelineShaderStageCreateInfo		shader_stages[16];
	/*Push constants*/
	VkPushConstantRange					push_constant_range;
	/*Descriptors*/
	uint32_t							descriptor_count;
	VkBuffer							descriptor_buffers[32];
	VkDeviceMemory						descriptor_buffers_memory[32];
	VkDescriptorSetLayout				descriptor_set_layouts[32];
	VkDescriptorSet						descriptor_sets[32];
	VkWriteDescriptorSet				write_descriptor_sets[32];
	VkDescriptorSetLayoutBinding		descriptor_set_layout_bindings[32];
	VkDescriptorBufferInfo				descriptor_buffer_infos[32];
	VkDescriptorPool					descriptor_pools[32];
	uint32_t							dynamic_descriptor_buffer_offsets[32];
	/*Pipeline*/
	VkPipelineLayout					pipeline_layout;
	VkPipeline							pipeline;
} ShVkPipeline;

#define SH_UNIFORM_BUFFER  0
#define SH_DYNAMIC_UNIFORM_BUFFER 1


static ShVkPipeline* shAllocateShVkPipeline() {
	ShVkPipeline* p_pipeline = (ShVkPipeline*)calloc(1, sizeof(ShVkPipeline));
	shVkError(p_pipeline == NULL, "invalid pipeline memory", return NULL);
	return p_pipeline;
}

static uint8_t shFreeVkPipeline(ShVkPipeline** pp_pipeline) {
	shVkError(pp_pipeline == NULL, "invalid pipeline memory", return 0);
	free(*pp_pipeline);
	*pp_pipeline = NULL;
	return 1;
}


extern uint8_t shCreateRasterizer(VkPolygonMode polygon_mode, VkPipelineRasterizationStateCreateInfo* p_rasterizer);

extern uint8_t shSetMultisampleState(VkPipelineMultisampleStateCreateInfo* p_multisample_state);

extern uint8_t shColorBlendSettings(VkPipelineColorBlendAttachmentState* p_color_blend_attachment, VkPipelineColorBlendStateCreateInfo* p_color_blend_state);

extern uint8_t shSetViewport(const uint32_t width, const uint32_t height, VkViewport *p_viewport, VkRect2D* p_scissors, VkPipelineViewportStateCreateInfo* p_viewport_state);

extern uint8_t shSetFixedStates(VkDevice device, const uint32_t surface_width, const uint32_t surface_height, VkPrimitiveTopology primitive, VkPolygonMode polygon_mode, ShVkFixedStates* p_fixed_states);

extern uint8_t shSetVertexInputAttribute(const uint32_t location, VkFormat format, const uint32_t offset, const uint32_t size, ShVkFixedStates* p_fixed_states);

extern uint8_t shSetVertexInputRate(const VkVertexInputRate input_rate, const uint32_t binding, VkVertexInputBindingDescription* p_vertex_binding);

extern uint8_t shSetVertexInputState(VkVertexInputBindingDescription* p_vertex_binding, uint32_t vertex_input_attribute_count, VkVertexInputAttributeDescription* p_vertex_input_attributes, VkPipelineVertexInputStateCreateInfo* p_vertex_input_state);

extern uint8_t shCreateInputAssembly(const VkPrimitiveTopology primitive_topology, const VkBool32 primitive_restart_enable, VkPipelineInputAssemblyStateCreateInfo* p_input_assembly);

extern uint8_t shSetPushConstants(const VkShaderStageFlags shader_stage_flags, const uint32_t offset, const uint32_t size, VkPushConstantRange* p_constant_range);

extern uint8_t shCreateDescriptorBuffer(const VkDevice device, const VkBufferUsageFlags usage, const uint32_t descriptor_idx, const uint32_t size, const uint32_t max_size, VkDescriptorBufferInfo* p_buffer_info, VkBuffer* p_buffer);

extern uint8_t shDescriptorSetLayout(const VkDevice device, const uint32_t binding, const VkDescriptorType descriptor_type, const VkShaderStageFlags shaderStageFlags, VkDescriptorSetLayoutBinding* p_binding, VkDescriptorSetLayout* p_descriptor_set_layout);

extern uint8_t shCreateDescriptorPool(VkDevice device, const VkDescriptorType descriptor_type, VkDescriptorPool* p_descriptor_pool);

extern uint8_t shAllocateDescriptorSet(VkDevice device, const uint32_t binding, VkDescriptorType descriptor_type, VkDescriptorSetLayout* p_descriptor_set_layout, VkDescriptorPool descriptor_pool, VkDescriptorSet* p_descriptor_set, VkDescriptorBufferInfo* p_buffer_info, VkWriteDescriptorSet* p_write_descriptor_set);

extern uint8_t shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, VkShaderModule* p_shader_module);

extern uint8_t shCreateShaderStage(const VkDevice device, const VkShaderModule shader_module, const VkShaderStageFlags shader_stage_flag, VkPipelineShaderStageCreateInfo* p_shader_stage);



extern uint8_t shSetupGraphicsPipeline(VkDevice device, VkRenderPass render_pass, const ShVkFixedStates fixed_states, ShVkPipeline* p_pipeline);

extern uint8_t shSetupComputePipeline(VkDevice device, ShVkPipeline* p_pipeline);

extern uint8_t shEndPipeline(ShVkPipeline* p_pipeline);

extern uint8_t shPipelineRelease(VkDevice device, ShVkPipeline* p_pipeline);

extern uint8_t shFixedStatesRelease(ShVkFixedStates* p_fixed_states);

#define shFixedStatesSetVertexInputRate(input_rate, binding, p_fixed_states)\
	shSetVertexInputRate(input_rate, binding, &(p_fixed_states)->vertex_binding_description)

#define shFixedStatesSetVertexInputState(p_fixed_states)\
	shSetVertexInputState(&(p_fixed_states)->vertex_binding_description, (p_fixed_states)->vertex_input_attribute_description_count, (p_fixed_states)->vertex_input_attributes, &(p_fixed_states)->vertex_input_state_info)

extern uint8_t shPipelineCreateDescriptorBuffer(const VkDevice device, const VkBufferUsageFlags buffer_usage_flag, const uint32_t descriptor_idx, const uint32_t size, ShVkPipeline* p_pipeline);

extern uint8_t shPipelineCreateDynamicDescriptorBuffer(const VkDevice device, const VkBufferUsageFlags buffer_usage_flag, const uint32_t descriptor_idx, const uint32_t size, const uint32_t max_bindings, ShVkPipeline* p_pipeline);

extern uint8_t shPipelineAllocateDescriptorBuffersMemory(const VkDevice device, const VkPhysicalDevice physical_device, VkMemoryPropertyFlags flags, ShVkPipeline* p_pipeline);


#define shPipelineDescriptorSetLayout(device, descriptor_idx, binding, descriptor_type, shader_stage, p_pipeline)\
	shDescriptorSetLayout(device,\
		binding,\
		descriptor_type,\
		shader_stage,\
		&(p_pipeline)->descriptor_set_layout_bindings[descriptor_idx],\
		&(p_pipeline)->descriptor_set_layouts[descriptor_idx]\
	)

#define shPipelineCreateDescriptorPool(device, descriptor_idx, p_pipeline)\
		shCreateDescriptorPool(device,\
			(p_pipeline)->descriptor_set_layout_bindings[descriptor_idx].descriptorType,\
			&(p_pipeline)->descriptor_pools[descriptor_idx]\
		)

#define shPipelineAllocateDescriptorSet(device, descriptor_idx, p_pipeline)\
	shAllocateDescriptorSet(device,\
		(p_pipeline)->descriptor_set_layout_bindings[descriptor_idx].binding,\
		(p_pipeline)->descriptor_set_layout_bindings[descriptor_idx].descriptorType,\
		&(p_pipeline)->descriptor_set_layouts[descriptor_idx],\
		(p_pipeline)->descriptor_pools[descriptor_idx],\
		&(p_pipeline)->descriptor_sets[descriptor_idx],\
		&(p_pipeline)->descriptor_buffer_infos[descriptor_idx],\
		&(p_pipeline)->write_descriptor_sets[descriptor_idx]\
	)

#define shPipelineCreateShaderModule(device, size, code, p_pipeline)\
	shCreateShaderModule(device, size, code, &(p_pipeline)->shader_modules[(p_pipeline)->shader_module_count])

extern uint8_t shPipelineCreateShaderStage(const VkDevice device, VkShaderStageFlags shader_stage_flag, ShVkPipeline* p_pipeline);

#define shPipelineWriteDescriptorBufferMemory(device, descriptor_idx, offset, size, p_descriptor_buffer_data, p_pipeline)\
	shWriteMemory(device,\
		(p_pipeline)->descriptor_buffers_memory[descriptor_idx],\
		offset,\
		(size == 0) ? (uint32_t)(p_pipeline)->descriptor_buffer_infos[descriptor_idx].range : size,\
		p_descriptor_buffer_data\
	)

#define shPipelineWriteDynamicDescriptorBufferMemory(device, descriptor_idx, p_descriptor_buffer_data, p_pipeline)\
	shWriteMemory(device,\
		(p_pipeline)->descriptor_buffers_memory[descriptor_idx],\
		(p_pipeline)->dynamic_descriptor_buffer_offsets[descriptor_idx],\
		(uint32_t)(p_pipeline)->descriptor_buffer_infos[descriptor_idx].range,\
		p_descriptor_buffer_data\
	)

#define shPipelinePushConstants(cmd_buffer, p_push_constant_data, p_pipeline)\
	vkCmdPushConstants(cmd_buffer,\
		(p_pipeline)->pipeline_layout,\
		(p_pipeline)->push_constant_range.stageFlags,\
		(p_pipeline)->push_constant_range.offset,\
		(p_pipeline)->push_constant_range.size,\
		p_push_constant_data\
	)

#define shBindPipeline(cmd_buffer, bind_point, p_pipeline)\
	vkCmdBindPipeline(cmd_buffer, bind_point, (p_pipeline)->pipeline)

#define shPipelineUpdateDescriptorSets(device, p_pipeline)\
	vkUpdateDescriptorSets(device,\
		(p_pipeline)->descriptor_count,\
		(p_pipeline)->write_descriptor_sets,\
		0, NULL\
	)

#define shPipelineUpdateDescriptorSet(device, descriptor_idx, p_pipeline)\
	vkUpdateDescriptorSets(device,\
		1,\
		&(p_pipeline)->write_descriptor_sets[descriptor_idx],\
		0,\
		NULL\
	)

#define shPipelineBindDescriptorSet(cmd_buffer, descriptor_idx, bind_point, p_pipeline)\
	vkCmdBindDescriptorSets(cmd_buffer,\
		bind_point,\
		(p_pipeline)->pipeline_layout,\
		descriptor_idx,\
		1,\
		&(p_pipeline)->descriptor_sets[descriptor_idx],\
		0,\
		NULL\
	)

#define shPipelineBindDescriptorSets(cmd_buffer, first_descriptor, descriptor_count, bind_point, p_pipeline)\
	vkCmdBindDescriptorSets(cmd_buffer,\
		bind_point,\
		(p_pipeline)->pipeline_layout,\
		first_descriptor,\
		descriptor_count,\
		&(p_pipeline)->descriptor_sets[first_descriptor],\
		0,\
		NULL\
	)

extern uint8_t shPipelineBindDynamicDescriptorSet(const VkCommandBuffer cmd_buffer, const uint32_t descriptor_idx, const VkPipelineBindPoint bind_point, ShVkPipeline* p_pipeline);

extern uint8_t shPipelineBindDynamicDescriptorSets(const VkCommandBuffer cmd_buffer, const uint32_t first_descriptor, const uint32_t descriptor_count, const VkPipelineBindPoint bind_point, ShVkPipeline* p_pipeline);


#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_PIPELINE_DATA_H