#ifndef SH_MEMORY_INFO_H
#define SH_MEMORY_INFO_H

#include <stdint.h>
#include <vulkan/vulkan.h>

typedef struct ShVkCore	ShVkCore;

#define SH_VEC1_SIGNED_FLOAT		VK_FORMAT_R32_SFLOAT
#define SH_VEC2_SIGNED_FLOAT		VK_FORMAT_R32G32_SFLOAT
#define SH_VEC3_SIGNED_FLOAT		VK_FORMAT_R32G32B32_SFLOAT

#define SH_VEC1_SIGNED_INT			VK_FORMAT_R32_SINT
#define SH_VEC2_SIGNED_INT			VK_FORMAT_R32G32_SINT
#define SH_VEC3_SIGNED_INT			VK_FORMAT_R32G32B32_SINT

#define SH_VEC1_UNSIGNED_INT		VK_FORMAT_R32_UINT
#define SH_VEC2_UNSIGNED_INT		VK_FORMAT_R32G32_UINT
#define SH_VEC3_UNSIGNED_INT		VK_FORMAT_R32G32B32_UINT

#define shCreateVertexBuffer(core, size, p_vertex_buffer)\
	shCreateBuffer(core.device, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, p_vertex_buffer)

#define shAllocateVertexBuffer(core, vertex_buffer, p_vertex_buffer_memory)\
	shAllocateMemory(core.device, core.physical_device, vertex_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_vertex_buffer_memory)

#define shMapVertexBufferMemory(core, vertex_buffer_memory, size, p_vertices)\
	shMapMemory(core.device, vertex_buffer_memory, size, (void*)p_vertices)

#define shCreateIndexBuffer(core, size, p_index_buffer)\
	shCreateBuffer(core.device, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, p_index_buffer)

#define shAllocateIndexBuffer(core, index_buffer, p_index_buffer_memory)\
	shAllocateMemory(core.device, core.physical_device, index_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_index_buffer_memory)

#define shMapIndexBufferMemory(core, index_buffer_memory, size, p_indices)\
	shMapMemory(core.device, index_buffer_memory, size, (void*)p_indices)

#define shClearVertexBufferMemory(device, vertex_buffer, vertex_buffer_memory)\
	shClearBufferMemory(device, vertex_buffer, vertex_buffer_memory)

#define shClearIndexBufferMemory(device, index_buffer, index_buffer_memory)\
	shClearBufferMemory(device, index_buffer, index_buffer_memory)

#define SH_DEPTH_IMAGE_FORMAT VK_FORMAT_D32_SFLOAT

#define shCreateDepthImage(p_core)\
	shCreateImage((p_core)->device, (p_core)->physical_device, (p_core)->surface.width, (p_core)->surface.height, SH_DEPTH_IMAGE_FORMAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, &(p_core)->depth_image, &(p_core)->depth_image_memory)

#define shInitDepthData(p_core)\
	shCreateDepthImage(p_core); shCreateDepthImageView(p_core)

extern void shCreateBuffer(const VkDevice device, const uint32_t bufferSize, VkBufferUsageFlagBits usage_flags, VkBuffer* p_buffer);

extern void shGetMemoryType(const VkDevice device, const VkPhysicalDevice physical_device, const uint32_t typeFlags, uint32_t *p_memory_type_index);

extern void shAllocateMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const uint32_t type_flags, VkDeviceMemory* p_memory);

extern void shMapMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t data_size, const void* p_data);

extern void shClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory);

extern void shCreateImage(const VkDevice device, const VkPhysicalDevice physical_device, const uint32_t width, const uint32_t height, VkFormat format, VkImageUsageFlags usage, VkImage* p_image, VkDeviceMemory* p_image_memory);

#endif//SH_VK_MEMORY_H
