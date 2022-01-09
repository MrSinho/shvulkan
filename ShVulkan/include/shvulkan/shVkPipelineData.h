#ifndef SH_PIPELINE_DATA_H
#define SH_PIPELINE_DATA_H

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
	uint32_t							uniforms_idx;
	uint32_t							dynamic_uniforms_idx;
	uint32_t							uniform_buffers_size[2][32];
	uint32_t							uniform_buffers_offsets[32];
	VkDescriptorSetLayout				descriptor_set_layouts[64];
	VkDescriptorSet						descriptor_sets[2][32];
	VkWriteDescriptorSet				write_descriptor_sets[2][32];
	VkBuffer							uniform_buffers[2][32];
	VkDeviceMemory						uniform_buffers_memory[2][32];
	VkDescriptorSetLayoutBinding		descriptor_set_layout_bindings[2][32];
	VkDescriptorBufferInfo				descriptor_buffer_infos[2][32];
	VkDescriptorPool					descriptor_pools[2][32];
	/*Pipeline*/
	VkPipelineLayout					main_pipeline_layout;
	VkPipeline							pipeline;
} ShVkGraphicsPipeline;

#define shWriteUniformBufferMemory(p_core, uniform_idx, p_uniform_buffer_data, p_pipeline)\
	shMapMemory((p_core)->device,\
		(p_pipeline)->uniform_buffers_memory[0][uniform_idx],\
		0,\
		(p_pipeline)->uniform_buffers_size[0][uniform_idx],\
		p_uniform_buffer_data)

#define shWriteDynamicUniformBufferMemory(p_core, uniform_idx, p_uniform_buffer_data, p_pipeline)\
	shMapMemory((p_core)->device,\
		(p_pipeline)->uniform_buffers_memory[1][uniform_idx],\
		(p_pipeline)->uniform_buffers_offsets[uniform_idx],\
		(p_pipeline)->uniform_buffers_size[1][uniform_idx],\
		p_uniform_buffer_data)

#define SH_UNIFORM_BUFFER  0
#define SH_DYNAMIC_UNIFORM_BUFFER 1

extern void shCreateRasterizer(VkPipelineRasterizationStateCreateInfo* p_rasterizer);

extern void shSetMultisampleState(VkPipelineMultisampleStateCreateInfo* p_multisample_state);

extern void shColorBlendSettings(VkPipelineColorBlendAttachmentState* p_color_blend_attachment, VkPipelineColorBlendStateCreateInfo* p_color_blend_state);

extern void shSetViewport(const uint32_t width, const uint32_t height, VkViewport *p_viewport, VkRect2D* p_scissors, VkPipelineViewportStateCreateInfo* p_viewport_state);

extern void shSetFixedStates(ShVkCore* p_core, ShFixedStateFlags flags, ShVkFixedStates* p_fixed_states);

extern void shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, ShVkGraphicsPipeline* p_pipeline);

extern void shCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const VkShaderStageFlagBits stageFlag, ShVkGraphicsPipeline* p_pipeline);

extern void shSetVertexInputAttribute(const uint32_t location, VkFormat format, const uint32_t offset, const uint32_t size, ShVkFixedStates* p_fixed_states);

extern void shSetVertexInputState(VkVertexInputBindingDescription* p_vertex_binding, uint32_t vertex_input_attribute_count, VkVertexInputAttributeDescription* p_vertex_input_attributes, VkPipelineVertexInputStateCreateInfo* p_vertex_input_state);

extern void shCreateInputAssembly(const VkPrimitiveTopology primitive_topology, const VkBool32 primitive_restart_enable, VkPipelineInputAssemblyStateCreateInfo* p_input_assembly);

extern void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkGraphicsPipeline* p_pipeline);

extern void shDescriptorSetLayout(ShVkCore* p_core, const uint32_t uniform_idx, const uint8_t type, const VkShaderStageFlags shaderStageFlags, ShVkGraphicsPipeline* p_pipeline);

extern void shCreateDescriptorPool(ShVkCore* p_core, const uint32_t uniform_idx, const uint8_t type, ShVkGraphicsPipeline* p_pipeline);

extern void shAllocateDescriptorSet(ShVkCore* p_core, const uint32_t uniform_idx, const uint8_t type, ShVkGraphicsPipeline* p_pipeline);

extern void shSetupGraphicsPipeline(ShVkCore* p_core, const ShVkFixedStates fStates, ShVkGraphicsPipeline* p_pipeline);

extern void shEndPipeline(ShVkGraphicsPipeline* p_pipeline);

extern void shDestroyPipeline(ShVkCore* p_core, ShVkGraphicsPipeline* p_pipeline);

#endif//SH_PIPELINE_DATA_H