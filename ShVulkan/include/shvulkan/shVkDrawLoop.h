#ifndef SH_DRAW_LOOP_H
#define SH_DRAW_LOOP_H

#include <vulkan/vulkan.h>

typedef struct ShVkCore ShVkCore;

typedef struct ShVkGraphicsPipeline ShVkGraphicsPipeline;

typedef struct ShMeshInfo ShMeshInfo;

typedef struct ShMesh ShMesh;

typedef struct ShVkFixedStates ShVkFixedStates;

#define shDrawIndexed(p_core, index_count)\
	vkCmdDrawIndexed((p_core)->graphics_cmd_buffer, index_count, 1, 0, 0, 0)

#define shDraw(p_core, vertex_count_div_stride)\
	vkCmdDraw((p_core)->graphics_cmd_buffer, vertex_count_div_stride, 1, 0, 0)

#define shUpdateUniformBuffer(p_core, uniform_idx, p_pipeline)\
	vkUpdateDescriptorSets((p_core)->device,\
		1, &(p_pipeline)->write_descriptor_sets[uniform_idx],\
		0, NULL\
	)

#define shPushConstants(p_core, p_push_constants_data, p_pipeline)\
	vkCmdPushConstants((p_core)->graphics_cmd_buffer, (p_pipeline)->main_pipeline_layout, (p_pipeline)->push_constant_range.stageFlags, (p_pipeline)->push_constant_range.offset, (p_pipeline)->push_constant_range.size, p_push_constants_data)

extern void shFrameReset(ShVkCore* p_core);

extern void shFrameBegin(ShVkCore* p_core, uint32_t* swapchain_image_index);

extern void shBindPipeline(ShVkCore* p_core, ShVkGraphicsPipeline* p_pipeline);

extern void shBindVertexBuffer(ShVkCore* p_core, VkBuffer* p_vertex_buffer);

extern void shBindIndexBuffer(ShVkCore* p_core, VkBuffer* p_index_buffer);

extern void shUpdateUniformBuffers(ShVkCore* p_core, ShVkGraphicsPipeline* p_pipeline);

extern void shBindUniformBuffer(ShVkCore* p_core, const uint32_t uniform_idx, ShVkGraphicsPipeline* p_pipeline);

extern void shBindDynamicUniformBuffer(ShVkCore* p_core, const uint32_t uniform_idx, ShVkGraphicsPipeline* p_pipeline);

extern void shFrameEnd(ShVkCore* p_core, const uint32_t swapchain_image_index);

#endif//SH_VK_DRAW_LOOP_H