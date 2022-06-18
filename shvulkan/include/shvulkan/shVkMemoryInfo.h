#ifndef SH_MEMORY_INFO_H
#define SH_MEMORY_INFO_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>
#include <vulkan/vulkan.h>
#include <memory.h>

#define SH_VEC1_SIGNED_FLOAT		VK_FORMAT_R32_SFLOAT
#define SH_VEC2_SIGNED_FLOAT		VK_FORMAT_R32G32_SFLOAT
#define SH_VEC3_SIGNED_FLOAT		VK_FORMAT_R32G32B32_SFLOAT

#define SH_VEC1_SIGNED_INT			VK_FORMAT_R32_SINT
#define SH_VEC2_SIGNED_INT			VK_FORMAT_R32G32_SINT
#define SH_VEC3_SIGNED_INT			VK_FORMAT_R32G32B32_SINT

#define SH_VEC1_UNSIGNED_INT		VK_FORMAT_R32_UINT
#define SH_VEC2_UNSIGNED_INT		VK_FORMAT_R32G32_UINT
#define SH_VEC3_UNSIGNED_INT		VK_FORMAT_R32G32B32_UINT

#define SH_MAX_UNIFORM_BUFFER_SIZE 65536



extern void shCreateBuffer(const VkDevice device, const uint32_t size, VkBufferUsageFlags usage_flags, VkBuffer* p_buffer);

extern void shGetMemoryType(const VkDevice device, const VkPhysicalDevice physical_device, const VkMemoryPropertyFlags property_flags, uint32_t *p_memory_type_index);

extern void shAllocateMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const VkMemoryPropertyFlags property_flags, VkDeviceMemory* p_memory);

#define shBindMemory(device, vk_buffer, memory)\
	shVkResultError(\
		vkBindBufferMemory(device, vk_buffer, memory, 0),\
		"error binding buffer memory "\
	)

extern void shReadMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t offset, const uint32_t data_size, void* p_data);

extern void shWriteMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t offset, const uint32_t data_size, const void* p_data);

extern void shClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory);

extern void shCreateImage(const VkDevice device, const VkPhysicalDevice physical_device, const uint32_t width, const uint32_t height, VkFormat format, VkImageUsageFlags usage, VkImage* p_image, VkDeviceMemory* p_image_memory);

extern void shGetMemoryBudgetProperties(const VkPhysicalDevice physical_device, uint32_t* p_memory_budget, uint32_t* p_process_used_memory, VkPhysicalDeviceMemoryBudgetPropertiesEXT* p_memory_budget_properties);



#define shCreateVertexBuffer(device, size, p_vertex_buffer)\
	shCreateBuffer(device, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, p_vertex_buffer)

#define shAllocateVertexBufferMemory(device, physical_device, vertex_buffer, p_vertex_buffer_memory)\
	shAllocateMemory(device, physical_device, vertex_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_vertex_buffer_memory)

#define shBindVertexBufferMemory(device, vertex_buffer, vertex_buffer_memory)\
	shBindMemory(device, vertex_buffer, vertex_buffer_memory)

#define shWriteVertexBufferMemory(device, vertex_buffer_memory, size, p_vertices)\
	shWriteMemory(device, vertex_buffer_memory, 0, size, (void*)p_vertices)

static void shBindVertexBuffer(VkCommandBuffer graphics_cmd_buffer, VkBuffer* p_vertex_buffer) {
	const VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(graphics_cmd_buffer, 0, 1, p_vertex_buffer, &offset);
}

#define shCreateIndexBuffer(device, size, p_index_buffer)\
	shCreateBuffer(device, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, p_index_buffer)

#define shAllocateIndexBufferMemory(device, physical_device, index_buffer, p_index_buffer_memory)\
	shAllocateMemory(device, physical_device, index_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_index_buffer_memory)

#define shBindIndexBufferMemory(device, index_buffer, index_buffer_memory)\
	shBindMemory(device, index_buffer, index_buffer_memory)

#define shWriteIndexBufferMemory(device, index_buffer_memory, size, p_indices)\
	shWriteMemory(device, index_buffer_memory, 0, size, (void*)p_indices)

#define shBindIndexBuffer(graphics_cmd_buffer, p_index_buffer)\
	vkCmdBindIndexBuffer(graphics_cmd_buffer, *p_index_buffer, 0, VK_INDEX_TYPE_UINT32)

#define shClearVertexBufferMemory(device, vertex_buffer, vertex_buffer_memory)\
	shClearBufferMemory(device, vertex_buffer, vertex_buffer_memory)

#define shClearIndexBufferMemory(device, index_buffer, index_buffer_memory)\
	shClearBufferMemory(device, index_buffer, index_buffer_memory)

#define shPipelineAllocateDescriptorBufferMemory(device, physical_device, descriptor_idx, p_pipeline)\
	shAllocateMemory(device, physical_device, (p_pipeline)->descriptor_buffers[descriptor_idx], VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &(p_pipeline)->descriptor_buffers_memory[descriptor_idx])

#define shPipelineBindDescriptorBufferMemory(device, descriptor_idx, p_pipeline)\
	shBindMemory(device, (p_pipeline)->descriptor_buffers[descriptor_idx], (p_pipeline)->descriptor_buffers_memory[descriptor_idx])

#define shPipelineClearDescriptorBufferMemory(device, descriptor_idx, p_pipeline)\
	shClearBufferMemory(device, (p_pipeline)->descriptor_buffers[descriptor_idx], (p_pipeline)->descriptor_buffers_memory[descriptor_idx])

#define SH_DEPTH_IMAGE_FORMAT VK_FORMAT_D32_SFLOAT

#define shCreateDepthImage(p_core)\
	shCreateImage((p_core)->device, (p_core)->physical_device, (p_core)->surface.width, (p_core)->surface.height, SH_DEPTH_IMAGE_FORMAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, &(p_core)->depth_image, &(p_core)->depth_image_memory)

#define shInitDepthData(p_core)\
	shCreateDepthImage(p_core); shCreateDepthImageView(p_core)

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VK_MEMORY_H
