#ifndef SH_DRAW_LOOP_H
#define SH_DRAW_LOOP_H

#include <vulkan/vulkan.h>

typedef struct ShVkCore ShVkCore;

typedef struct ShVkPipelineData ShVkPipelineData;

typedef struct ShMeshInfo ShMeshInfo;

typedef struct ShMesh ShMesh;

typedef struct ShVkFixedStates ShVkFixedStates;

extern void shFrameReset(const ShVkCore core);

extern void shFrameBegin(const ShVkCore core, uint32_t* swapchainImageIndex);

extern void shBindPipeline(const VkCommandBuffer graphicsCmdBuffer, const ShVkPipelineData pipeData);

extern void shPushConstants(const VkCommandBuffer graphicsCmdBuffer, const ShVkPipelineData pipeData, const void* pPushConstantsData);

extern void shBindVertexBuffer(const ShVkCore core, VkBuffer* p_vertex_buffer);

extern void shBindIndexBuffer(const ShVkCore core, VkBuffer* p_index_buffer);

extern void shBindDescriptorSets(const ShVkCore core, ShVkPipelineData pipeData);

extern void shDraw(const VkCommandBuffer graphicsCmdBuffer, const uint32_t count, const uint32_t stride, const uint8_t indexed);

extern void shFrameEnd(const ShVkCore core, const uint32_t swapchainImageIndex);

#endif//SH_VK_DRAW_LOOP_H