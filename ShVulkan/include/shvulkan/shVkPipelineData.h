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

typedef struct ShVkPipelineData {
	/*Shaders*/
	uint32_t							shader_stage_count;
	VkPipelineShaderStageCreateInfo		shader_stages[2];
	uint32_t							shader_module_count;
	VkShaderModule						shader_modules[2];
	/*Push constants*/
	VkPushConstantRange					push_constant_range;
	/*Uniform buffers*/	
	uint32_t							uniform_buffer_count;
	uint32_t							uniform_buffers_size[32];
	VkDescriptorSet						descriptor_sets[32];
	VkWriteDescriptorSet				write_descriptor_sets[32];
	uint32_t							dynamic_uniform_buffers_size[32];
	uint32_t							uniform_buffers_offsets[32];
	VkDescriptorSet						dynamic_descriptor_sets[32];
	VkWriteDescriptorSet				dynamic_write_descriptor_sets[32];
	VkBuffer							uniform_buffers[32];
	VkDeviceMemory						uniform_buffers_memory[32];
	VkDescriptorSetLayoutBinding		descriptor_set_layout_bindings[32];
	VkDescriptorSetLayout				descriptor_set_layouts[32];
	VkDescriptorBufferInfo				descriptor_buffer_infos[32];
	VkDescriptorPool					descriptor_pools[32];

	/*Pipeline*/
	VkPipelineLayout					main_pipeline_layout;
	VkPipeline							pipeline;
} ShVkPipelineData;

extern void shCreateRasterizer(VkPipelineRasterizationStateCreateInfo* p_rasterizer);

extern void shSetMultisampleState(VkPipelineMultisampleStateCreateInfo* p_multisample_state);

extern void shColorBlendSettings(VkPipelineColorBlendAttachmentState* p_color_blend_attachment, VkPipelineColorBlendStateCreateInfo* p_color_blend_state);

extern void shSetViewport(const uint32_t width, const uint32_t height, VkViewport *p_viewport, VkRect2D* p_scissors, VkPipelineViewportStateCreateInfo* p_viewport_state);

extern void shSetFixedStates(ShVkCore* p_core, ShFixedStateFlags flags, ShVkFixedStates* p_fixed_states);

extern void shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, ShVkPipelineData* p_pipeline);

extern void shCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const VkShaderStageFlagBits stageFlag, ShVkPipelineData* p_pipeline);

extern void shSetVertexInputAttribute(const uint32_t location, VkFormat format, const uint32_t offset, const uint32_t size, ShVkFixedStates* p_fixed_states);

extern void shSetVertexInputState(VkVertexInputBindingDescription* p_vertex_binding, uint32_t vertex_input_attribute_count, VkVertexInputAttributeDescription* p_vertex_input_attributes, VkPipelineVertexInputStateCreateInfo* p_vertex_input_state);

extern void shCreateInputAssembly(const VkPrimitiveTopology primitive_topology, const VkBool32 primitive_restart_enable, VkPipelineInputAssemblyStateCreateInfo* p_input_assembly);

extern void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkPipelineData* p_pipeline);

extern void shDescriptorSetLayout(ShVkCore* p_core, const uint32_t uniform_idx, const VkShaderStageFlags shaderStageFlags, ShVkPipelineData* p_pipeline);

extern void shCreateDescriptorPool(ShVkCore* p_core, const uint32_t uniform_idx, ShVkPipelineData* p_pipeline);

extern void shAllocateDescriptorSet(ShVkCore* p_core, const uint32_t uniform_idx, ShVkPipelineData* p_pipeline);

extern void shSetupGraphicsPipeline(ShVkCore* p_core, const ShVkFixedStates fStates, ShVkPipelineData* p_pipeline);

extern void shWriteUniformBufferMemory(ShVkCore* p_core, const uint32_t uniform_idx, void* p_uniform_buffer_data, ShVkPipelineData* p_pipeline);

extern void shEndPipeline(ShVkPipelineData* p_pipeline);

extern void shDestroyPipeline(ShVkCore* p_core, ShVkPipelineData* p_pipeline);

#endif//SH_PIPELINE_DATA_H