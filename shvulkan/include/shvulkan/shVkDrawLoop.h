#ifndef SH_DRAW_LOOP_H
#define SH_DRAW_LOOP_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>

typedef struct ShVkCore ShVkCore;

typedef struct ShVkGraphicsPipeline ShVkGraphicsPipeline;

typedef struct ShMeshInfo ShMeshInfo;

typedef struct ShMesh ShMesh;

typedef struct ShVkFixedStates ShVkFixedStates;



extern void shFrameReset(ShVkCore* p_core);

extern void shFrameBegin(ShVkCore* p_core, uint32_t* swapchain_image_index);

#define shDrawIndexed(graphics_cmd_buffer, index_count)\
	vkCmdDrawIndexed(graphics_cmd_buffer, index_count, 1, 0, 0, 0)

#define shDraw(graphics_cmd_buffer, vertex_count_div_stride)\
	vkCmdDraw(graphics_cmd_buffer, vertex_count_div_stride, 1, 0, 0)

extern void shFrameEnd(ShVkCore* p_core, const uint32_t swapchain_image_index);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VK_DRAW_LOOP_H