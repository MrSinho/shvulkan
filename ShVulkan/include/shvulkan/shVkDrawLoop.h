#ifndef SH_DRAW_LOOP_H
#define SH_DRAW_LOOP_H

#include <vulkan/vulkan.h>

typedef struct ShVkCore ShVkCore;

typedef struct ShVkPipelineData ShVkPipelineData;

typedef struct ShMeshInfo ShMeshInfo;

typedef struct ShMesh ShMesh;

typedef struct ShVkFixedStates ShVkFixedStates;

#define shDrawIndexed(p_core, index_count)\
	vkCmdDrawIndexed((p_core)->graphics_cmd_buffer, index_count, 1, 0, 0, 0)

#define shDraw(p_core, vertex_count_div_stride)\
	vkCmdDraw((p_core)->graphics_cmd_buffer, vertex_count_div_stride, 1, 0, 0)


extern void shFrameReset(ShVkCore* p_core);

extern void shFrameBegin(ShVkCore* p_core, uint32_t* swapchain_image_index);

extern void shBeginPipeline(ShVkCore* p_core, ShVkPipelineData* p_pipeline);

extern void shBindPipeline(ShVkCore* p_core, ShVkPipelineData* p_pipeline);

extern void shPushConstants(ShVkCore* p_core, const void* p_push_constants_data, ShVkPipelineData* p_pipeline);

extern void shBindVertexBuffer(ShVkCore* p_core, VkBuffer* p_vertex_buffer);

extern void shBindIndexBuffer(ShVkCore* p_core, VkBuffer* p_index_buffer);

extern void shBindUniformBuffer(ShVkCore* p_core, const uint32_t uniform_idx, ShVkPipelineData* p_pipeline);

extern void shFrameEnd(ShVkCore* p_core, const uint32_t swapchain_image_index);

#endif//SH_VK_DRAW_LOOP_H