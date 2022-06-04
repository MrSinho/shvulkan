#ifndef SH_PIPELINE_DATA_H
#define SH_PIPELINE_DATA_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>

typedef enum ShVkFixedStateFlags  {
	SH_FIXED_STATES_POLYGON_MODE_WIREFRAME				= 0b000000001,
	SH_FIXED_STATES_POLYGON_MODE_FACE					= 0b000000010,
	SH_FIXED_STATES_POLYGON_MODE_POINTS					= 0b000000100,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST	= 0b000001000,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST		= 0b000010000,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST		= 0b000100000,
} ShVkFixedStateFlags;

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
	/**/
	ShVkFixedStateFlags 					fixed_state_flags;
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
	VkDescriptorSetLayout				descriptor_set_layouts[32];
	VkDescriptorSet						descriptor_sets[32];
	VkWriteDescriptorSet				write_descriptor_sets[32];
	VkDeviceMemory						descriptor_buffers_memory[32];
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



extern void shCreateRasterizer(VkPipelineRasterizationStateCreateInfo* p_rasterizer);

extern void shSetMultisampleState(VkPipelineMultisampleStateCreateInfo* p_multisample_state);

extern void shColorBlendSettings(VkPipelineColorBlendAttachmentState* p_color_blend_attachment, VkPipelineColorBlendStateCreateInfo* p_color_blend_state);

extern void shSetViewport(const uint32_t width, const uint32_t height, VkViewport *p_viewport, VkRect2D* p_scissors, VkPipelineViewportStateCreateInfo* p_viewport_state);

extern void shSetFixedStates(VkDevice device, const uint32_t surface_width, const uint32_t surface_height, ShVkFixedStateFlags  flags, ShVkFixedStates* p_fixed_states);

extern void shSetVertexInputAttribute(const uint32_t location, VkFormat format, const uint32_t offset, const uint32_t size, ShVkFixedStates* p_fixed_states);

extern void shSetVertexInputState(VkVertexInputBindingDescription* p_vertex_binding, uint32_t vertex_input_attribute_count, VkVertexInputAttributeDescription* p_vertex_input_attributes, VkPipelineVertexInputStateCreateInfo* p_vertex_input_state);

extern void shCreateInputAssembly(const VkPrimitiveTopology primitive_topology, const VkBool32 primitive_restart_enable, VkPipelineInputAssemblyStateCreateInfo* p_input_assembly);

extern void shSetPushConstants(const VkShaderStageFlags shader_stage_flags, const uint32_t offset, const uint32_t size, VkPushConstantRange* p_constant_range);

extern void shCreateDescriptorBuffer(const VkDevice device, const VkBufferUsageFlags usage, const uint32_t descriptor_idx, const uint32_t size, const uint32_t max_size, VkDescriptorBufferInfo* p_buffer_info, VkBuffer* p_buffer);

extern void shDescriptorSetLayout(const VkDevice device, const uint32_t binding, const VkDescriptorType descriptor_type, const VkShaderStageFlags shaderStageFlags, VkDescriptorSetLayoutBinding* p_binding, VkDescriptorSetLayout* p_descriptor_set_layout);

extern void shCreateDescriptorPool(VkDevice device, const VkDescriptorType descriptor_type, VkDescriptorPool* p_descriptor_pool);

extern void shAllocateDescriptorSet(VkDevice device, const uint32_t binding, VkDescriptorType descriptor_type, VkDescriptorSetLayout* p_descriptor_set_layout, VkDescriptorPool descriptor_pool, VkDescriptorSet* p_descriptor_set, VkDescriptorBufferInfo* p_buffer_info, VkWriteDescriptorSet* p_write_descriptor_set);

extern void shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, VkShaderModule* p_shader_module);

extern void shCreateShaderStage(const VkDevice device, const VkShaderModule shader_module, const VkShaderStageFlags shader_stage_flag, VkPipelineShaderStageCreateInfo* p_shader_stage);



extern void shSetupGraphicsPipeline(VkDevice device, VkRenderPass render_pass, const ShVkFixedStates fixed_states, ShVkPipeline* p_pipeline);

extern void shSetupComputePipeline(VkDevice device, ShVkPipeline* p_pipeline);

extern void shEndPipeline(ShVkPipeline* p_pipeline);

extern void shPipelineRelease(VkDevice device, ShVkPipeline* p_pipeline);



extern void shPipelineCreateDescriptorBuffer(const VkDevice device, const VkBufferUsageFlags buffer_usage_flag, const uint32_t descriptor_idx, const uint32_t size, ShVkPipeline* p_pipeline);

extern void shPipelineCreateDynamicDescriptorBuffer(const VkDevice device, const VkBufferUsageFlags buffer_usage_flag, const uint32_t descriptor_idx, const uint32_t size, const uint32_t max_bindings, ShVkPipeline* p_pipeline);

extern void shPipelineAllocateDescriptorBuffersMemory(const VkDevice device, const VkPhysicalDevice physical_device, ShVkPipeline* p_pipeline);

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

extern void shPipelineCreateShaderStage(const VkDevice device, VkShaderStageFlags shader_stage_flag, ShVkPipeline* p_pipeline);

#define shPipelineWriteDescriptorBufferMemory(device, descriptor_idx, p_descriptor_buffer_data, p_pipeline)\
	shWriteMemory(device,\
		(p_pipeline)->descriptor_buffers_memory[descriptor_idx],\
		0,\
		(uint32_t)(p_pipeline)->descriptor_buffer_infos[descriptor_idx].range,\
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

extern void shPipelineBindDynamicDescriptorSet(const VkCommandBuffer cmd_buffer, const uint32_t descriptor_idx, const VkPipelineBindPoint bind_point, ShVkPipeline* p_pipeline);

extern void shPipelineBindDynamicDescriptorSets(const VkCommandBuffer cmd_buffer, const uint32_t first_descriptor, const uint32_t descriptor_count, const VkPipelineBindPoint bind_point, ShVkPipeline* p_pipeline);


#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_PIPELINE_DATA_H