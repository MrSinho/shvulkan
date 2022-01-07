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

#define shCreateVertexBuffer(p_core, size, p_vertex_buffer)\
	shCreateBuffer((p_core)->device, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, p_vertex_buffer)

#define shAllocateVertexBuffer(p_core, vertex_buffer, p_vertex_buffer_memory)\
	shAllocateMemory((p_core)->device, (p_core)->physical_device, vertex_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_vertex_buffer_memory)

#define shMapVertexBufferMemory(p_core, vertex_buffer_memory, size, p_vertices)\
	shMapMemory((p_core)->device, vertex_buffer_memory, size, (void*)p_vertices)

#define shCreateIndexBuffer(p_core, size, p_index_buffer)\
	shCreateBuffer((p_core)->device, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, p_index_buffer)

#define shAllocateIndexBuffer(p_core, index_buffer, p_index_buffer_memory)\
	shAllocateMemory((p_core)->device, (p_core)->physical_device, index_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_index_buffer_memory)

#define shMapIndexBufferMemory(p_core, index_buffer_memory, size, p_indices)\
	shMapMemory((p_core)->device, index_buffer_memory, size, (void*)p_indices)

#define shClearVertexBufferMemory(p_core, vertex_buffer, vertex_buffer_memory)\
	shClearBufferMemory((p_core)->device, vertex_buffer, vertex_buffer_memory)

#define shClearIndexBufferMemory(p_core, index_buffer, index_buffer_memory)\
	shClearBufferMemory((p_core)->device, index_buffer, index_buffer_memory)

#define shCreateUniformBuffer(p_core, uniform_buffer_size, uniform_idx, p_pipeline)\
	shCreateBuffer((p_core)->device, uniform_buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &(p_pipeline)->uniform_buffers[uniform_idx]); (p_pipeline)->uniform_buffers_size[uniform_idx] = uniform_buffer_size

#define shAllocateUniformBuffer(p_core, uniform_idx, p_pipeline)\
	shAllocateMemory((p_core)->device, (p_core)->physical_device, (p_pipeline)->uniform_buffers[uniform_idx], VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &(p_pipeline)->uniform_buffers_memory[uniform_idx]);

#define shClearUniformBufferMemory(p_core, uniform_idx, p_pipeline)\
	shClearBufferMemory((p_core)->device, (p_pipeline)->uniform_buffers[uniform_idx], (p_pipeline)->uniform_buffers_memory[uniform_idx])

#define shMapUniformBufferMemory(p_core, uniform_buffer_memory, size, p_uniform_data)\
	shMapMemory((p_core)->device, uniform_buffer_memory, size, p_uniform_data)
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
