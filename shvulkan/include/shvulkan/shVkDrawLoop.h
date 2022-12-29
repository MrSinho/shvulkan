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



extern void shAcquireNextImage(ShVkCore* p_core, VkSemaphore semaphore, uint32_t* p_swapchain_image_idx);

extern void shBeginRenderPass(ShVkCore* p_core, VkCommandBuffer cmd_buffer, VkClearColorValue clear_color, uint32_t swapchain_image_idx);

#define shEndRenderPass(cmd_buffer)\
	vkCmdEndRenderPass(cmd_buffer)

#define shDraw(graphics_cmd_buffer, vertex_count_div_stride, first_vertex, instance_count, first_instance)\
	vkCmdDraw(graphics_cmd_buffer, vertex_count_div_stride, instance_count, first_vertex, first_instance)

#define shDrawIndexed(graphics_cmd_buffer, index_count, vertex_offset, first_index, instance_count, first_instance)\
	vkCmdDrawIndexed(graphics_cmd_buffer, index_count, instance_count, first_index, vertex_offset, first_instance)//vertex offset in bytes

void shGraphicsQueueSubmit(ShVkCore* p_core, VkCommandBuffer cmd_buffer, VkSemaphore semaphore, VkFence fence);

void shPresentImage(ShVkCore* p_core, VkSemaphore semaphore, const uint32_t swapchain_image_idx);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VK_DRAW_LOOP_H