#ifndef SH_DRAW_LOOP_H
#define SH_DRAW_LOOP_H

#include <vulkan/vulkan.h>

typedef struct ShVkCore ShVkCore;

typedef struct ShVkPipelineData ShVkPipelineData;

typedef struct ShMeshInfo ShMeshInfo;

typedef struct ShMesh ShMesh;

typedef struct ShVkFixedStates ShVkFixedStates;

extern void shFrameReset(const ShVkCore core);

extern void shFrameBegin(const ShVkCore core, uint32_t* swapchain_image_index);

extern void shBindPipeline(const VkCommandBuffer graphics_cmd_buffer, const ShVkPipelineData pipe_data);

extern void shPushConstants(const VkCommandBuffer graphics_cmd_buffer, const ShVkPipelineData pipe_data, const void* p_push_constants_data);

extern void shBindVertexBuffer(const ShVkCore core, VkBuffer* p_vertex_buffer);

extern void shBindIndexBuffer(const ShVkCore core, VkBuffer* p_index_buffer);

extern void shBindDescriptorSets(const ShVkCore core, ShVkPipelineData pipe_data);

extern void shDraw(const VkCommandBuffer graphics_cmd_buffer, const uint32_t count_div_stride, const uint8_t indexed);

extern void shFrameEnd(const ShVkCore core, const uint32_t swapchain_image_index);

#endif//SH_VK_DRAW_LOOP_H