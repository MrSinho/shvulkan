#ifndef SH_MEMORY_INFO_H
#define SH_MEMORY_INFO_H

#include <stdint.h>
#include <vulkan/vulkan.h>

typedef struct ShVkCore	ShVkCore;

#define shCreateVertexBuffer(core, p_mesh_info, p_mesh)\
	shCreateBuffer(core.device, p_mesh_info->vertex_count * sizeof(p_mesh_info->p_vertices[0]), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &p_mesh->vertex_buffer)

#define shAllocateVertexBuffer(core, p_mesh)\
	shAllocateMemory(core.device, core.physical_device, p_mesh->vertex_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &p_mesh->vertex_buffer_memory);

#define shMapVertexBufferMemory(core, p_mesh_info, p_mesh)\
	shMapMemory(core.device, p_mesh->vertex_buffer_memory, p_mesh_info->vertex_count * sizeof(p_mesh_info->p_vertices[0]), (void*)p_mesh_info->p_vertices);

#define shCreateIndexBuffer(core, p_mesh_info, p_mesh)\
	shCreateBuffer(core.device, p_mesh_info->index_count * sizeof(p_mesh_info->p_indices[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, &p_mesh->index_buffer);

#define shAllocateIndexBuffer(core, p_mesh)\
	shAllocateMemory(core.device, core.physical_device, p_mesh->index_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &p_mesh->index_buffer_memory);

#define shMapIndexBufferMemory(core, p_mesh_info, p_mesh)\
	shMapMemory(core.device, p_mesh->index_buffer_memory, p_mesh_info->index_count * sizeof(p_mesh_info->p_indices[0]), (void*)p_mesh_info->p_indices);

#define SH_DEPTH_IMAGE_FORMAT VK_FORMAT_D32_SFLOAT

#define shCreateDepthImage(p_core)\
	shCreateImage(*p_core, p_core->window.width, p_core->window.height, SH_DEPTH_IMAGE_FORMAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, &p_core->depth_image, &p_core->depth_image_memory);

#define shInitDepthData(p_core)\
	shCreateDepthImage(p_core); shCreateDepthImageView(p_core)

typedef struct ShMesh {
	VkBuffer 		vertex_buffer;
	VkDeviceMemory 	vertex_buffer_memory;
	VkBuffer		index_buffer;
	VkDeviceMemory	index_buffer_memory;
} ShMesh;

extern void shCreateBuffer(const VkDevice device, const uint32_t bufferSize, VkBufferUsageFlagBits usage_flags, VkBuffer* p_buffer);

extern void shGetMemoryType(const VkDevice device, const VkPhysicalDevice physical_device, const uint32_t typeFlags, uint32_t *p_memory_type_index);

extern void shAllocateMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const uint32_t type_flags, VkDeviceMemory* p_memory);

extern void shMapMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t data_size, const void* p_data);

extern void shClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory);

extern void shCreateImage(ShVkCore core, const uint32_t width, const uint32_t height, VkFormat format, VkImageUsageFlags usage, VkImage* p_image, VkDeviceMemory* p_image_memory);

#endif//SH_VK_MEMORY_H
