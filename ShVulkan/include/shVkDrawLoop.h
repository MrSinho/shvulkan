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

extern void shBindVertexBuffers(const ShVkCore core, const ShMesh mesh);

extern void shBindIndexBuffers(const ShVkCore core, const ShMesh mesh);

extern void shBindDescriptorSets(const ShVkCore core, ShVkPipelineData pipeData);

extern void shDraw(const VkCommandBuffer graphicsCmdBuffer, const uint32_t count, const uint32_t stride, const ShMesh mesh);

extern void shFrameEnd(const ShVkCore core, const uint32_t swapchainImageIndex);

#endif//SH_VK_DRAW_LOOP_H