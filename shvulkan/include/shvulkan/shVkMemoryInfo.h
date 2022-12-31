#ifndef SH_MEMORY_INFO_H
#define SH_MEMORY_INFO_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>
#include <vulkan/vulkan.h>
#include <memory.h>

#include "shvulkan/shVkCheck.h"

#define SH_VEC1_SIGNED_FLOAT		VK_FORMAT_R32_SFLOAT
#define SH_VEC2_SIGNED_FLOAT		VK_FORMAT_R32G32_SFLOAT
#define SH_VEC3_SIGNED_FLOAT		VK_FORMAT_R32G32B32_SFLOAT
#define SH_VEC4_SIGNED_FLOAT		VK_FORMAT_R32G32B32A32_SFLOAT

#define SH_VEC1_SIGNED_DOUBLE		VK_FORMAT_R64_SFLOAT
#define SH_VEC2_SIGNED_DOUBLE		VK_FORMAT_R64G64_SFLOAT
#define SH_VEC3_SIGNED_DOUBLE		VK_FORMAT_R64G64B64_SFLOAT
#define SH_VEC4_SIGNED_DOUBLE		VK_FORMAT_R64G64B64A64_SFLOAT

#define SH_VEC1_SIGNED_INT			VK_FORMAT_R32_SINT
#define SH_VEC2_SIGNED_INT			VK_FORMAT_R32G32_SINT
#define SH_VEC3_SIGNED_INT			VK_FORMAT_R32G32B32_SINT
#define SH_VEC4_SIGNED_INT			VK_FORMAT_R32G32B32A32_SINT

#define SH_VEC1_UNSIGNED_INT		VK_FORMAT_R32_UINT
#define SH_VEC2_UNSIGNED_INT		VK_FORMAT_R32G32_UINT
#define SH_VEC3_UNSIGNED_INT		VK_FORMAT_R32G32B32_UINT
#define SH_VEC4_UNSIGNED_INT		VK_FORMAT_R32G32B32A32_UINT

#define SH_VEC1_UNSIGNED_LONG		VK_FORMAT_R64_UINT
#define SH_VEC2_UNSIGNED_LONG		VK_FORMAT_R64G64_UINT
#define SH_VEC3_UNSIGNED_LONG		VK_FORMAT_R64G64B64_UINT
#define SH_VEC4_UNSIGNED_LONG		VK_FORMAT_R64G64B64A64_UINT



extern uint8_t shCreateBuffer(const VkDevice device, const uint32_t size, VkBufferUsageFlags usage_flags, VkBuffer* p_buffer);

extern uint8_t shGetMemoryType(const VkDevice device, const VkPhysicalDevice physical_device, const VkMemoryPropertyFlags property_flags, uint32_t *p_memory_type_index);

extern uint8_t shAllocateMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const VkMemoryPropertyFlags property_flags, VkDeviceMemory* p_memory);

extern uint8_t shCopyBuffer(VkCommandBuffer transfer_cmd_buffer, VkBuffer src_buffer, const uint32_t src_offset, const uint32_t dst_offset, const uint32_t size, VkBuffer dst_buffer);

extern uint8_t shBindMemory(const VkDevice device, VkBuffer buffer, const uint32_t offset, const VkDeviceMemory buffer_memory);

extern uint8_t shReadMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t offset, const uint32_t data_size, void* p_data);

extern uint8_t shWriteMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t offset, const uint32_t data_size, const void* p_data);

extern uint8_t shClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory);

extern uint8_t shCreateImage(const VkDevice device, VkImageType type, uint32_t x, uint32_t y, uint32_t z, VkFormat format, uint32_t mip_levels, VkSampleCountFlagBits sample_count, VkImageUsageFlags usage, VkImage* p_image);

extern uint8_t shAllocateImageMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkImage image, VkDeviceMemory* p_image_memory);

extern uint8_t shBindImageMemory(const VkDevice device, const VkImage image, const uint32_t offset, const VkDeviceMemory image_memory);

extern uint8_t shClearImageMemory(const VkDevice device, const VkImage image, const VkDeviceMemory image_memory);


typedef struct VkPhysicalDeviceMemoryBudgetPropertiesEXT ShMemoryBudgetProperties;

extern uint8_t shGetMemoryBudgetProperties(const VkPhysicalDevice physical_device, ShMemoryBudgetProperties* p_memory_budget_properties);



#define shBindVertexBuffers(graphics_cmd_buffer, first_binding, binding_count, p_vertex_buffers, p_offsets)\
	vkCmdBindVertexBuffers(graphics_cmd_buffer, first_binding, binding_count, p_vertex_buffers, p_offsets)

#define shBindIndexBuffer(graphics_cmd_buffer, offset, p_index_buffer)\
	vkCmdBindIndexBuffer(graphics_cmd_buffer, *(p_index_buffer), offset, VK_INDEX_TYPE_UINT32)



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VK_MEMORY_H
