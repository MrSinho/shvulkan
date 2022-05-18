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

#if 0
typedef struct ShVkPipeline {
	uint32_t						shader_module_count;
	VkShaderModule					shader_modules[1];
	VkPipelineShaderStageCreateInfo shader_stages[1];
	/*Push constant*/
	VkPushConstantRange				push_constant_range;
	/*Descriptors*/
	uint32_t						descriptor_count;
	uint8_t							descriptor_bindings[32];
	VkDescriptorSetLayoutBinding	descriptor_set_layout_bindings[32];
	VkDescriptorSetLayout			descriptor_set_layouts[32];
	VkDescriptorSet					descriptor_sets[32];
	VkDescriptorPool				descriptor_pools[32];
	/*Pipeline*/
	VkPipelineLayout				pipeline_layout;
	VkPipeline						pipeline;
} ShVkPipeline;
#endif//0


extern void shCreateRasterizer(VkPipelineRasterizationStateCreateInfo* p_rasterizer);

extern void shSetMultisampleState(VkPipelineMultisampleStateCreateInfo* p_multisample_state);

extern void shColorBlendSettings(VkPipelineColorBlendAttachmentState* p_color_blend_attachment, VkPipelineColorBlendStateCreateInfo* p_color_blend_state);

extern void shSetViewport(const uint32_t width, const uint32_t height, VkViewport *p_viewport, VkRect2D* p_scissors, VkPipelineViewportStateCreateInfo* p_viewport_state);

extern void shSetFixedStates(VkDevice device, const uint32_t surface_width, const uint32_t surface_height, ShFixedStateFlags flags, ShVkFixedStates* p_fixed_states);

extern void shSetVertexInputAttribute(const uint32_t location, VkFormat format, const uint32_t offset, const uint32_t size, ShVkFixedStates* p_fixed_states);

extern void shSetVertexInputState(VkVertexInputBindingDescription* p_vertex_binding, uint32_t vertex_input_attribute_count, VkVertexInputAttributeDescription* p_vertex_input_attributes, VkPipelineVertexInputStateCreateInfo* p_vertex_input_state);

extern void shCreateInputAssembly(const VkPrimitiveTopology primitive_topology, const VkBool32 primitive_restart_enable, VkPipelineInputAssemblyStateCreateInfo* p_input_assembly);

extern void shSetPushConstants(const VkShaderStageFlags shaderStageFlags, const uint32_t offset, const uint32_t size, ShVkPipeline* p_pipeline);

extern void shCreateDescriptorBuffer(const VkDevice device, const VkBufferUsageFlagBits usage, const uint32_t descriptor_idx, const uint32_t size, const uint32_t max_size, VkDescriptorBufferInfo* p_buffer_info, VkBuffer* p_buffer);

extern void shAllocateDescriptorBuffers(const VkDevice device, const VkPhysicalDevice physical_device, ShVkPipeline* p_pipeline);

extern void shCreateDescriptorPool(VkDevice device, const uint32_t descriptor_idx, const uint32_t binding, const VkDescriptorType descriptor_type, VkDescriptorPool* p_descriptor_pool);

extern void shDescriptorSetLayout(const VkDevice device, const uint32_t descriptor_idx, const uint32_t binding, const VkDescriptorType descriptor_type, const VkShaderStageFlags shaderStageFlags, VkDescriptorSetLayoutBinding* p_binding, VkDescriptorSetLayout* p_descriptor_set_layout);

extern void shAllocateDescriptorSet(VkDevice device, const uint32_t descriptor_idx, const uint32_t binding, VkDescriptorType descriptor_type, VkDescriptorPool descriptor_pool, VkDescriptorSetLayout* p_descriptor_set_layout, VkDescriptorSet* p_descriptor_set, VkDescriptorBufferInfo* p_buffer_info, VkWriteDescriptorSet* p_write_descriptor_set);

extern void shSetupGraphicsPipeline(VkDevice device, VkRenderPass render_pass, const ShVkFixedStates fStates, ShVkPipeline* p_pipeline);

extern void shCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, VkShaderModule* p_shader_module);

extern void shCreateShaderStage(const VkDevice device, const VkShaderModule shader_module, const VkShaderStageFlagBits shader_stage_flag, VkPipelineShaderStageCreateInfo* p_shader_stage);

extern void shEndPipeline(ShVkPipeline* p_pipeline);

extern void shPipelineRelease(VkDevice device, ShVkPipeline* p_pipeline);



extern void shSetupComputePipeline(VkDevice device, ShVkPipeline* p_pipeline);

extern void shComputeCreateShaderModule(const VkDevice device, const uint32_t size, const char* code, ShVkPipeline* p_pipeline);

extern void shComputeCreateShaderStage(const VkDevice device, ShVkPipeline* p_pipeline);


#include "shvulkan/shVkMemoryInfo.h"
#include "shvulkan/shVkCheck.h"

#define SH_VULKAN_MAKE_DESCRIPTOR_STRUCT_UTILS(STRUCT)\
typedef struct STRUCT##DescriptorStructureHandle {\
	uint32_t	structure_count;\
	uint32_t	structure_size;\
	STRUCT**	pp_##STRUCT;\
	void*		p_##STRUCT##_map;\
} STRUCT##DescriptorStructureHandle;\
static uint32_t shVkGet ## STRUCT ## DescriptorStructureSize(const VkPhysicalDeviceProperties physical_device_properties) {\
	return sizeof(STRUCT) > (uint32_t)physical_device_properties.limits.minUniformBufferOffsetAlignment ? (uint32_t)(sizeof(STRUCT) + (sizeof(STRUCT) % (uint32_t)physical_device_properties.limits.minUniformBufferOffsetAlignment)) : (uint32_t)physical_device_properties.limits.minUniformBufferOffsetAlignment;\
}\
static STRUCT##DescriptorStructureHandle shVkCreate ## STRUCT ## DescriptorStructures(const VkPhysicalDeviceProperties physical_device_properties, const uint32_t structure_count) {\
	STRUCT##DescriptorStructureHandle handle = { structure_count };\
	handle.pp_##STRUCT = calloc(structure_count, sizeof(STRUCT*));\
	for (uint32_t i = 0; i < handle.structure_count; i++) {\
		handle.pp_##STRUCT[i] = (STRUCT*)calloc(1, sizeof(STRUCT));\
		shVkAssert(handle.pp_##STRUCT[i] != NULL, "invalid input structure pointer");\
	}\
	shVkAssert(handle.pp_##STRUCT != NULL, "invalid input structure ppointer");\
	handle.structure_size = shVkGet## STRUCT ##DescriptorStructureSize(physical_device_properties);\
	handle.p_##STRUCT##_map = (STRUCT*)calloc(structure_count, handle.structure_size);\
	shVkAssert(handle.p_##STRUCT##_map != NULL, "invalid input structure map pointer");\
	return handle;\
}\
static STRUCT* shVkGet ## STRUCT ## DescriptorStructure(const STRUCT##DescriptorStructureHandle handle, const uint32_t idx, const uint8_t mapped) {\
	return mapped ? (STRUCT*)(&((char*)handle.p_##STRUCT##_map)[idx * handle.structure_size]) : handle.pp_##STRUCT[idx];\
}\
static void shVkMap ## STRUCT ## DecriptorStructures(STRUCT##DescriptorStructureHandle* p_handle) {\
	for (uint32_t i = 0; i < p_handle->structure_count; i++) {\
		memcpy(&((char*)p_handle->p_##STRUCT##_map)[i * p_handle->structure_size], p_handle->pp_##STRUCT[i], sizeof(STRUCT));\
	}\
}\

static void shPipelineCreateDescriptorBuffer(const VkDevice device, const VkPhysicalDeviceProperties physical_device_properties, const VkBufferUsageFlagBits buffer_usage_flag, const uint32_t descriptor_idx, const uint32_t size, ShVkPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer");
	shCreateDescriptorBuffer(device, buffer_usage_flag, descriptor_idx, size, size, &p_pipeline->descriptor_buffer_infos[descriptor_idx], &p_pipeline->descriptor_buffers[descriptor_idx]);
	p_pipeline->descriptor_count++;
}

static void shPipelineCreateDynamicDescriptorBuffer(const VkDevice device, const VkPhysicalDeviceProperties properties, const VkBufferUsageFlagBits buffer_usage_flag, const uint32_t descriptor_idx, const uint32_t size, const uint32_t max_bindings, ShVkPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer");
	shCreateDescriptorBuffer(device, buffer_usage_flag, descriptor_idx, size, size * max_bindings, &p_pipeline->descriptor_buffer_infos[descriptor_idx], &p_pipeline->descriptor_buffers[descriptor_idx]);
	p_pipeline->descriptor_count++;
}

static void shPipelineAllocateDescriptorBuffersMemory(const VkDevice device, const VkPhysicalDevice physical_device, ShVkPipeline* p_pipeline) {
	for (uint32_t descriptor_idx = 0; descriptor_idx < p_pipeline->descriptor_count; descriptor_idx++) {
		shPipelineAllocateDescriptorBufferMemory(device, physical_device, descriptor_idx, p_pipeline);
	}
}


static void shPipelineDescriptorSetLayout(const VkDevice device, const uint32_t descriptor_idx, const uint32_t binding, const VkDescriptorType descriptor_type, const VkShaderStageFlags shader_stage, ShVkPipeline* p_pipeline) {
	shVkAssert(p_pipeline != NULL, "invalid pipeline pointer");
	shDescriptorSetLayout(device, descriptor_idx, binding, descriptor_type, shader_stage, &(p_pipeline)->descriptor_set_layout_bindings[descriptor_idx], &(p_pipeline)->descriptor_set_layouts[descriptor_idx]);
	if (descriptor_type & VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC || descriptor_type & VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC) {

	}
}

#define shPipelineCreateDescriptorPool(device, descriptor_idx, p_pipeline)\
		shCreateDescriptorPool(device, descriptor_idx, (p_pipeline)->descriptor_set_layout_bindings[descriptor_idx].binding, (p_pipeline)->descriptor_set_layout_bindings[descriptor_idx].descriptorType, &(p_pipeline)->descriptor_pools[descriptor_idx])\

#define shPipelineAllocateDescriptorSet(device, descriptor_idx, p_pipeline)\
	shAllocateDescriptorSet(device, descriptor_idx, (p_pipeline)->descriptor_set_layout_bindings[descriptor_idx].binding, (p_pipeline)->descriptor_set_layout_bindings[descriptor_idx].descriptorType, (p_pipeline)->descriptor_pools[descriptor_idx], &(p_pipeline)->descriptor_set_layouts[descriptor_idx], &(p_pipeline)->descriptor_sets[descriptor_idx], &(p_pipeline)->descriptor_buffer_infos[descriptor_idx], &(p_pipeline)->write_descriptor_sets[descriptor_idx])\

static void shPipelineCreateShaderStage(const VkDevice device, VkShaderStageFlagBits shader_stage_flag, ShVkPipeline* p_pipeline) {
	shCreateShaderStage(device, p_pipeline->shader_modules[p_pipeline->shader_module_count], shader_stage_flag, &p_pipeline->shader_stages[p_pipeline->shader_module_count]);
	p_pipeline->shader_module_count++;
}

#define shPipelineCreateShaderModule(device, size, code, p_pipeline)\
	shCreateShaderModule(device, size, code, &(p_pipeline)->shader_modules[(p_pipeline)->shader_module_count])


#define shPipelineWriteDescriptorBufferMemory(device, descriptor_idx, p_descriptor_buffer_data, p_pipeline)\
	shWriteMemory(device,\
		(p_pipeline)->descriptor_buffers_memory[descriptor_idx],\
		0,\
		(uint32_t)(p_pipeline)->descriptor_buffer_infos[descriptor_idx].range,\
		p_descriptor_buffer_data)

static void shPipelineWriteDynamicDescriptorBufferMemory(const VkDevice device, const uint32_t descriptor_idx, void* p_descriptor_buffer_data, ShVkPipeline* p_pipeline) {
	shWriteMemory(device,
		p_pipeline->descriptor_buffers_memory[descriptor_idx],
		p_pipeline->dynamic_descriptor_buffer_offsets[descriptor_idx],
		(uint32_t)(p_pipeline)->descriptor_buffer_infos[descriptor_idx].range,
		p_descriptor_buffer_data
	);
}

#define shPipelinePushConstants(cmd_buffer, p_push_constants_data, p_pipeline)\
	vkCmdPushConstants(cmd_buffer, (p_pipeline)->pipeline_layout, (p_pipeline)->push_constant_range.stageFlags, (p_pipeline)->push_constant_range.offset, (p_pipeline)->push_constant_range.size, p_push_constants_data)


#define shBindPipeline(cmd_buffer, bind_point, p_pipeline)\
	vkCmdBindPipeline(cmd_buffer, bind_point, (p_pipeline)->pipeline)

#define shPipelineUpdateDescriptorSets(device, p_pipeline)\
	vkUpdateDescriptorSets(device,\
		(p_pipeline)->descriptor_count, (p_pipeline)->write_descriptor_sets,\
		0, NULL\
	)

#define shPipelineUpdateDescriptorSet(device, descriptor_idx, p_pipeline)\
	vkUpdateDescriptorSets(device,\
		1, &(p_pipeline)->write_descriptor_sets[descriptor_idx],\
		0, NULL\
	)

#define shPipelineBindDescriptorSet(cmd_buffer, descriptor_idx, bind_point, p_pipeline)\
	vkCmdBindDescriptorSets(cmd_buffer,\
		bind_point,\
		(p_pipeline)->pipeline_layout, descriptor_idx, 1,\
		&(p_pipeline)->descriptor_sets[descriptor_idx],\
		0,\
		NULL\
	)

static void shPipelineBindDynamicDescriptorSet(const VkCommandBuffer cmd_buffer, const uint32_t descriptor_idx, const VkPipelineBindPoint bind_point, ShVkPipeline* p_pipeline) {
	vkCmdBindDescriptorSets(cmd_buffer,
		bind_point,
		p_pipeline->pipeline_layout, descriptor_idx, 1,
		&(p_pipeline)->descriptor_sets[descriptor_idx],
		1,
		&(p_pipeline)->dynamic_descriptor_buffer_offsets[descriptor_idx]
	);
	(p_pipeline)->dynamic_descriptor_buffer_offsets[descriptor_idx] += (uint32_t)p_pipeline->descriptor_buffer_infos[descriptor_idx].range;
}


#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_PIPELINE_DATA_H