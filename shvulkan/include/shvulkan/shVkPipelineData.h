#ifndef SH_PIPELINE_DATA_H
#define SH_PIPELINE_DATA_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>

typedef enum ShFixedStateFlags {
	SH_FIXED_STATES_POLYGON_MODE_WIREFRAME				= 0b000000001,
	SH_FIXED_STATES_POLYGON_MODE_FACE					= 0b000000010,
	SH_FIXED_STATES_POLYGON_MODE_POINTS					= 0b000000100,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST	= 0b000001000,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST		= 0b000010000,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST		= 0b000100000,
} ShFixedStateFlags;

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
	ShFixedStateFlags						fixed_state_flags;
} ShVkFixedStates;

#define SH_UNIFORM_BUFFER_DYNAMIC 0b01000000000000000000000000000000

typedef struct ShVkGraphicsPipeline {
	/*Shaders*/
	uint32_t							shader_stage_count;
	VkPipelineShaderStageCreateInfo		shader_stages[2];
	uint32_t							shader_module_count;
	VkShaderModule						shader_modules[2];
	/*Push constants*/
	VkPushConstantRange					push_constant_range;
	/*Uniform buffers*/
	uint32_t							uniform_count;
	uint32_t							uniform_buffers_size[32];
	uint8_t								dynamic_uniforms[32];
	uint32_t							uniform_buffers_offsets[32];
	VkDescriptorSetLayout				descriptor_set_layouts[32];
	VkDescriptorSet						descriptor_sets[32];
	VkWriteDescriptorSet				write_descriptor_sets[32];
	VkBuffer							uniform_buffers[32];
	VkDeviceMemory						uniform_buffers_memory[32];
	VkDescriptorSetLayoutBinding		descriptor_set_layout_bindings[32];
	VkDescriptorBufferInfo				descriptor_buffer_infos[32];
	VkDescriptorPool					descriptor_pools[32];
	/*Pipeline*/
	VkPipelineLayout					main_pipeline_layout;
	VkPipeline							pipeline;
} ShVkGraphicsPipeline;

#define shWriteUniformBufferMemory(device, uniform_idx, p_uniform_buffer_data, p_pipeline)\
	shMapMemory(device,\
		(p_pipeline)->uniform_buffers_memory[uniform_idx],\
		0,\
		(p_pipeline)->uniform_buffers_size[uniform_idx],\
		p_uniform_buffer_data)

#define shWriteDynamicUniformBufferMemory(device, uniform_idx, p_uniform_buffer_data, p_pipeline)\
	shMapMemory(device,\
		(p_pipeline)->uniform_buffers_memory[uniform_idx],\
		(p_pipeline)->uniform_buffers_offsets[uniform_idx],\
		(p_pipeline)->uniform_buffers_size[uniform_idx],\
		p_uniform_buffer_data)

#define shUpdateUniformBuffer(device, uniform_idx, p_pipeline)\
	vkUpdateDescriptorSets(device,\
		1, &(p_pipeline)->write_descriptor_sets[uniform_idx],\
		0, NULL\
	)

#define shPushConstants(graphics_cmd_buffer, p_push_constants_data, p_pipeline)\
	vkCmdPushConstants(graphics_cmd_buffer, (p_pipeline)->main_pipeline_layout, (p_pipeline)->push_constant_range.stageFlags, (p_pipeline)->push_constant_range.offset, (p_pipeline)->push_constant_range.size, p_push_constants_data)


#define shBindGraphicsPipeline(graphics_cmd_buffer, p_pipeline)\
	vkCmdBindPipeline(graphics_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, (p_pipeline)->pipeline)

#define shUpdateUniformBuffers(device, p_pipeline)\
	vkUpdateDescriptorSets(device,\
		(p_pipeline)->uniform_count, (p_pipeline)->write_descriptor_sets,\
		0, NULL\
	)

#define shBindUniformBuffer(graphics_cmd_buffer, uniform_idx, p_pipeline)\
	vkCmdBindDescriptorSets(graphics_cmd_buffer,\
		VK_PIPELINE_BIND_POINT_GRAPHICS,\
		(p_pipeline)->main_pipeline_layout, uniform_idx, 1,\
		&(p_pipeline)->descriptor_sets[uniform_idx],\
		0,\
		NULL\
	)

static void shBindDynamicUniformBuffer(VkCommandBuffer graphics_cmd_buffer, const uint32_t uniform_idx, ShVkGraphicsPipeline* p_pipeline) {
	vkCmdBindDescriptorSets(graphics_cmd_buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		p_pipeline->main_pipeline_layout, uniform_idx, 1,
		&(p_pipeline)->descriptor_sets[uniform_idx],
		1,
		&(p_pipeline)->uniform_buffers_offsets[uniform_idx]
	);
	(p_pipeline)->uniform_buffers_offsets[uniform_idx] += (p_pipeline)->uniform_buffers_size[uniform_idx];
}

#define SH_UNIFORM_BUFFER  0
#define SH_DYNAMIC_UNIFORM_BUFFER 1

extern void shCreateRasterizer(VkPipelineRasterizationStateCreateInfo* p_rasterizer);

extern void shSetMultisampleState(VkPipelineMultisampleStateCreateInfo* p_multisample_state);

extern void shColorBlendSettings(VkPipelineColorBlendAttachmentState* p_color_blend_attachment, VkPipelineColorBlendStateCreateInfo* p_color_blend_state);

extern void shSetViewport(const uint32_t width, const uint32_t height, VkViewport *p_viewport, VkRect2D* p_scissors, VkPipelineViewportStateCreateInfo* p_viewport_state);

extern void shSetFixedStates(VkDevice device, const uint32_t surface_width, const uint32_t surface_height, ShFixedStateFlags flags, ShVkFixedStates* p_fixed_states);

extern void shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, ShVkGraphicsPipeline* p_pipeline);

extern void shCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const VkShaderStageFlagBits stageFlag, ShVkGraphicsPipeline* p_pipeline);

extern void shSetVertexInputAttribute(const uint32_t location, VkFormat format, const uint32_t offset, const uint32_t size, ShVkFixedStates* p_fixed_states);

extern void shSetVertexInputState(VkVertexInputBindingDescription* p_vertex_binding, uint32_t vertex_input_attribute_count, VkVertexInputAttributeDescription* p_vertex_input_attributes, VkPipelineVertexInputStateCreateInfo* p_vertex_input_state);

extern void shCreateInputAssembly(const VkPrimitiveTopology primitive_topology, const VkBool32 primitive_restart_enable, VkPipelineInputAssemblyStateCreateInfo* p_input_assembly);

extern void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkGraphicsPipeline* p_pipeline);

extern void shCreateUniformBuffer(VkDevice device, const uint32_t uniform_idx, const uint32_t size, ShVkGraphicsPipeline* p_pipeline);

extern void shCreateDynamicUniformBuffer(VkDevice device, const uint32_t uniform_idx, const uint32_t size, ShVkGraphicsPipeline* p_pipeline);

extern void shAllocateUniformBuffers(VkDevice device, VkPhysicalDevice physical_device, ShVkGraphicsPipeline* p_pipeline);

extern void shCreateDescriptorPool(VkDevice device, const uint32_t swapchain_image_count, const uint32_t uniform_idx, ShVkGraphicsPipeline* p_pipeline);

extern void shDescriptorSetLayout(VkDevice device, const uint32_t uniform_idx, const VkShaderStageFlags shaderStageFlags, ShVkGraphicsPipeline* p_pipeline);

extern void shAllocateDescriptorSet(VkDevice device, const uint32_t uniform_idx, ShVkGraphicsPipeline* p_pipeline);

extern void shCreateDescriptorPools(VkDevice device, const uint32_t swapchain_image_count, ShVkGraphicsPipeline* p_pipeline);

extern void shAllocateDescriptorSets(VkDevice device, ShVkGraphicsPipeline* p_pipeline);

extern void shSetupGraphicsPipeline(VkDevice device, VkRenderPass render_pass, const ShVkFixedStates fStates, ShVkGraphicsPipeline* p_pipeline);

extern void shEndPipeline(ShVkGraphicsPipeline* p_pipeline);

extern void shDestroyPipeline(VkDevice device, ShVkGraphicsPipeline* p_pipeline);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_PIPELINE_DATA_H