#ifndef SH_DRAW_LOOP_H
#define SH_DRAW_LOOP_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>

typedef struct ShVkCore ShVkCore;

typedef struct ShVkPipeline ShVkPipeline;

typedef struct ShMeshInfo ShMeshInfo;

typedef struct ShMesh ShMesh;

typedef struct ShVkFixedStates ShVkFixedStates;



extern void shFrameReset(ShVkCore* p_core, uint32_t thread_idx);

extern void shFrameBegin(ShVkCore* p_core, const uint32_t thread_idx, uint32_t* p_swapchain_image_idx);

#define shDrawIndexed(graphics_cmd_buffer, index_count)\
	vkCmdDrawIndexed(graphics_cmd_buffer, index_count, 1, 0, 0, 0)

#define shDraw(graphics_cmd_buffer, vertex_count_div_stride)\
	vkCmdDraw(graphics_cmd_buffer, vertex_count_div_stride, 1, 0, 0)

extern void shFrameEnd(ShVkCore* p_core, const uint32_t thread_idx, const uint32_t swapchain_image_idx);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VK_DRAW_LOOP_H