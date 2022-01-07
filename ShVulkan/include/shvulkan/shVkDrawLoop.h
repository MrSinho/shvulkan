#ifndef SH_DRAW_LOOP_H
#define SH_DRAW_LOOP_H

#include <vulkan/vulkan.h>

typedef struct ShVkCore ShVkCore;

typedef struct ShVkPipelineData ShVkPipelineData;

typedef struct ShMeshInfo ShMeshInfo;

typedef struct ShMesh ShMesh;

typedef struct ShVkFixedStates ShVkFixedStates;

extern void shFrameReset(ShVkCore* p_core);

extern void shFrameBegin(ShVkCore* p_core, uint32_t* swapchain_image_index);

extern void shBindPipeline(ShVkCore* p_core, ShVkPipelineData* p_pipe_data);

extern void shPushConstants(ShVkCore* p_core, const void* p_push_constants_data, ShVkPipelineData* p_pipe_data);

extern void shBindVertexBuffer(ShVkCore* p_core, VkBuffer* p_vertex_buffer);

extern void shBindIndexBuffer(ShVkCore* p_core, VkBuffer* p_index_buffer);

extern void shBindDescriptorSets(ShVkCore* p_core, ShVkPipelineData* p_pipe_data);

extern void shDraw(ShVkCore* p_core, const uint32_t count_div_stride, const uint8_t indexed);

extern void shFrameEnd(ShVkCore* p_core, const uint32_t swapchain_image_index);

#endif//SH_VK_DRAW_LOOP_H