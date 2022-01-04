#ifndef SH_PIPELINE_DATA_H
#define SH_PIPELINE_DATA_H

#include <vulkan/vulkan.h>

typedef enum ShFixedStateFlags {
	SH_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT = 0x01,
	SH_FIXED_STATES_POLYGON_MODE_FACE_BIT = 0x02,
	SH_FIXED_STATES_POLYGON_MODE_POINTS_BIT = 0x04,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = 0x08,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST = 0x10,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST = 0x20,
	SH_FIXED_STATES_VERTEX_POSITIONS_BIT = 0x40,
	SH_FIXED_STATES_VERTEX_NORMALS_BIT = 0x80,
	SH_FIXED_STATES_VERTEX_TCOORDS_BIT = 0x100
} ShFixedStateFlags;

typedef struct ShVkFixedStates {
	/*Shader inputs*/
	VkVertexInputBindingDescription			vertex_binding_description;
	uint32_t								vertex_input_attribute_description_count;
	VkVertexInputAttributeDescription*		p_vertex_input_assembly_descriptions;
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

typedef struct ShUniformBuffer {
	VkBuffer						uniform_buffer;
	uint32_t						uniform_buffer_size;
	VkDeviceMemory					uniform_buffer_memory;
	VkDescriptorSetLayoutBinding	descriptor_set_layout_binding;
	VkDescriptorSetLayout			descriptor_set_layout;
	VkDescriptorBufferInfo			descriptor_buffer_info;
	VkDescriptorPool				descriptor_pool;
} ShUniformBuffer;

typedef struct ShVkPipelineData {
	uint32_t vertexStride;
	/*Shaders*/
	uint32_t							shaderStageCount;
	VkPipelineShaderStageCreateInfo*	p_shader_stages;
	uint32_t							shader_module_count;
	VkShaderModule*						p_shader_modules;
	/*Push constants*/
	VkPushConstantRange					push_constant_range;
	/*Uniform buffers*/	
	uint32_t							uniform_buffer_count;
	ShUniformBuffer*					p_uniform_buffers;
	VkWriteDescriptorSet*				p_write_descriptor_sets;
	VkDescriptorSet*					p_descriptor_sets;
	/*Pipeline*/
	VkPipelineLayout					main_pipeline_layout;
	VkPipeline							pipeline;
} ShVkPipelineData;


extern void shCreateRasterizer(VkPipelineRasterizationStateCreateInfo* p_rasterizer);

extern void shSetMultisampleState(VkPipelineMultisampleStateCreateInfo* p_multisample_state);

extern void shColorBlendSettings(VkPipelineColorBlendAttachmentState* p_color_blend_attachment, VkPipelineColorBlendStateCreateInfo* p_color_blend_state);

extern void shSetViewport(const uint32_t width, const uint32_t height, VkViewport *p_viewport, VkRect2D* p_scissors, VkPipelineViewportStateCreateInfo* p_viewport_state);

extern void shSetFixedStates(const ShVkCore core, ShFixedStateFlags flags, ShVkFixedStates* p_fixed_states);

extern void shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, VkShaderModule* p_shader_module);

extern void shCreateShaderStage(const VkDevice device, const VkShaderModule shModule, const VkShaderStageFlagBits stageFlag, VkPipelineShaderStageCreateInfo* p_shader_stage);

extern void shSetVertexInputState(const ShFixedStateFlags flags, VkVertexInputBindingDescription* p_vertex_binding, uint32_t* p_vertex_input_attribute_count, VkVertexInputAttributeDescription* p_vertex_input_attributes, VkPipelineVertexInputStateCreateInfo* p_vertex_input_state);

extern void shCreateInputAssembly(const VkPrimitiveTopology primitive_topology, const VkBool32 primitive_restart_enable, VkPipelineInputAssemblyStateCreateInfo* p_input_assembly);

extern void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkPipelineData* p_pipe_data);

extern void shAllocateUniformBufferData(const ShVkCore core, const uint32_t bufferSize, ShUniformBuffer* p_uniform);

extern void shDescriptorSetLayout(const ShVkCore core, const uint32_t binding, const VkShaderStageFlags shaderStageFlags, ShUniformBuffer* p_uniform);

extern void shCreateDescriptorPool(const ShVkCore core, ShUniformBuffer* p_uniform);

extern void shAllocateDescriptorSets(const ShVkCore core, const uint32_t uniform_idx, ShVkPipelineData* p_pipe_data);

extern void shSetupGraphicsPipeline(const ShVkCore core, const ShVkFixedStates fStates, ShVkPipelineData* p_pipe_data);

extern void shDestroyPipeline(const ShVkCore core, ShVkPipelineData* p_pipe_data);

#endif//SH_PIPELINE_DATA_H