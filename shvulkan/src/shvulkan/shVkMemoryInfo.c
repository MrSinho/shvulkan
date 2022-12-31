#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkMemoryInfo.h"
#include "shvulkan/shVkCheck.h"

#include <stdlib.h>
#include <memory.h>



uint8_t shCreateBuffer(const VkDevice device, const uint32_t size, VkBufferUsageFlags usage_flags, VkBuffer* p_buffer) {
	shVkError(p_buffer == NULL, "invalid arguments", return 0);
	VkBufferCreateInfo buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		(VkDeviceSize)size,						//size;
		usage_flags,							//usage;
		VK_SHARING_MODE_EXCLUSIVE,				//sharingMode;
		0,										//queueFamilyIndexCount;
		NULL									//pQueueFamilyIndices;
	};

	shVkResultError(
		vkCreateBuffer(device, &buffer_create_info, NULL, p_buffer),
		"error creating buffer", 
		return 0
	);

	return 1;
}

uint8_t shAllocateMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const VkMemoryPropertyFlags property_flags, VkDeviceMemory *p_memory) {
	shVkError(device			== NULL, "invalid device handle",			return 0);
	shVkError(physical_device	== NULL, "invalid physical device memory",	return 0);
	shVkError(buffer			== NULL, "invalid buffer pointer",			return 0);
	shVkError(p_memory			== NULL, "invalid device memory pointer",	return 0);

	uint32_t memory_type_index = 0;
	shGetMemoryType(device, physical_device, property_flags, &memory_type_index);

	VkMemoryRequirements memory_requirements = { 0 };
	vkGetBufferMemoryRequirements(device, buffer, &memory_requirements);

	VkMemoryAllocateInfo memory_allocate_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,	//sType;
		NULL,									//pNext;
		memory_requirements.size,				//allocationSize;
		memory_type_index						//memoryTypeIndex;
	};

	shVkResultError(
		vkAllocateMemory(device, &memory_allocate_info, NULL, p_memory),
		"error allocating memory", 
		return 0
	);

	return 1;
}

uint8_t shCopyBuffer(VkCommandBuffer transfer_cmd_buffer, VkBuffer src_buffer, const uint32_t src_offset, const uint32_t dst_offset, const uint32_t size, VkBuffer dst_buffer) {
	shVkError(transfer_cmd_buffer == NULL, "invalid command buffer", return 0);
	shVkError(src_buffer == NULL, "invalid source buffer", return 0);
	shVkError(dst_buffer == NULL, "invalid destination buffer", return 0);

	VkBufferCopy region = {
		src_offset,
		dst_offset,
		size
	};
	vkCmdCopyBuffer(transfer_cmd_buffer, src_buffer, dst_buffer, 1, &region);

	return 1;
}

uint8_t shBindMemory(const VkDevice device, VkBuffer buffer, const uint32_t offset, const VkDeviceMemory buffer_memory) {
	shVkError(device == NULL,			"invalid device handle", return 0);
	shVkError(buffer == NULL,			"invalid buffer handle", return 0);
	shVkError(buffer_memory == NULL,	"invalid buffer memory", return 0);
	
	shVkResultError(
		vkBindBufferMemory(device, buffer, buffer_memory, offset),
		"error binding buffer memory", return 0
	)

	return 1;
}

uint8_t shGetMemoryType(const VkDevice device, const VkPhysicalDevice physical_device, const VkMemoryPropertyFlags property_flags, uint32_t* p_memory_type_index) {
	shVkError(p_memory_type_index == NULL, "invalid memory type index pointer", return 0);
	VkPhysicalDeviceMemoryProperties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

	for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {

		if (memory_properties.memoryTypes[i].propertyFlags & (1 << i) &&
			(memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
			*p_memory_type_index = i;
			return 1;
		}

	}
	shVkError(
		0,
		"cannot find suitable memory type for the given buffer", 
		return 0
	);

	return 0;
}

uint8_t shReadMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t offset, const uint32_t data_size, void* p_data) {
	shVkError(p_data == NULL, "invalid memory buffer", return 0);
	void* data;
	shVkResultError(
		vkMapMemory(device, memory, offset, data_size, 0, &data),
		"error mapping memory", return 0
	);
	memcpy(p_data, data, (size_t)data_size);
	vkUnmapMemory(device, memory);

	return 1;
}

uint8_t shWriteMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t offset, const uint32_t data_size, const void* p_data) {
	shVkError(p_data == NULL, "invalid memory buffer", return 0);
	void* data;
	shVkResultError(
		vkMapMemory(device, memory, offset, data_size, 0, &data),
		"error mapping memory", return 0
	);
	memcpy(data, p_data, (size_t)data_size);
	vkUnmapMemory(device, memory);

	return 1;
}

uint8_t shClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory) {
	shVkError(device == NULL, "invalid device handle", return 0);
	shVkError(buffer == NULL, "invalid buffer memory", return 0);
	shVkError(memory == NULL, "invalid device memory", return 0);

	shVkResultError(
		vkDeviceWaitIdle(device),
		"failed waiting device idle",
		return 0
	);
	vkDestroyBuffer(device, buffer, NULL);
	vkFreeMemory(device, memory, NULL);

	return 1;
}

uint8_t shCreateImage(const VkDevice device, VkImageType type, uint32_t x, uint32_t y, uint32_t z, VkFormat format, uint32_t mip_levels, VkSampleCountFlagBits sample_count, VkImageUsageFlags usage, VkImage* p_image) {
	shVkError(device == NULL, "invalid device memory", return 0);
	shVkError(p_image == NULL, "invalid image memory", return 0);

	VkExtent3D image_extent = {
		x, y, z
	};

	VkImageCreateInfo image_create_info = {
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,	//sType;			
		NULL,									//pNext;
		0,										//flags;
		type,									//imageType;
		format,									//format;
		image_extent,							//extent;
		mip_levels,								//mipLevels;
		1,										//arrayLayers;
		sample_count,							//samples;
		VK_IMAGE_TILING_OPTIMAL,				//tiling;
		usage,									//usage;
		VK_SHARING_MODE_EXCLUSIVE,				//sharingMode;
		0,										//queueFamilyIndexCount;
		NULL									//pQueueFamilyIndices;
	};
	shVkResultError(
		vkCreateImage(device, &image_create_info, NULL, p_image),
		"error creating image", 
		return 0
	);

	return 1;
}

uint8_t shAllocateImageMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkImage image, VkDeviceMemory* p_image_memory) {
	shVkError(physical_device == NULL, "invalid physical device ", return 0);
	shVkError(p_image_memory == NULL, "invalid image memory", return 0);
	shVkError(image == NULL, "invalid image", return 0);
	shVkError(p_image_memory == NULL, "invalid image memory", return 0);

	VkMemoryRequirements memory_requirements = { 0 };
	vkGetImageMemoryRequirements(device, image, &memory_requirements);

	uint32_t memory_type_index = 0;
	shVkError(
		shGetMemoryType(device, physical_device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memory_type_index) == 0,
		"failed detecting memory type",
		return 0
	);

	VkMemoryAllocateInfo memory_allocate_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		NULL,
		memory_requirements.size,
		memory_type_index
	};

	shVkResultError(
		vkAllocateMemory(device, &memory_allocate_info, NULL, p_image_memory),
		"error allocating image memory",
		return 0
	);

	return 1;
}

uint8_t shBindImageMemory(const VkDevice device, const VkImage image, const uint32_t offset, const VkDeviceMemory image_memory) {
	shVkError(device       == NULL, "invalid device memory", return 0);
	shVkError(image        == NULL, "invalid image",         return 0);
	shVkError(image_memory == NULL, "invalid image memory",  return 0);

	shVkResultError(
		vkBindImageMemory(device, image, image_memory, offset),
		"error binding image memory",
		return 0
	);

	return 1;
}

uint8_t shClearImageMemory(const VkDevice device, const VkImage image, const VkDeviceMemory image_memory) {
	shVkError(device == NULL, "invalid device memory", return 0);
	shVkError(image == NULL, "invalid image", return 0);
	shVkError(image_memory == NULL, "invalid image memory", return 0);

	shVkResultError(
		vkDeviceWaitIdle(device),
		"failed waiting device idle",
		return 0
	);

	vkDestroyImage(device, image, NULL);
	vkFreeMemory(device, image_memory, NULL);

	return 0;
}

uint8_t shGetMemoryBudgetProperties(const VkPhysicalDevice physical_device, ShMemoryBudgetProperties* p_memory_budget_properties) {

	VkPhysicalDeviceMemoryBudgetPropertiesEXT memory_budget_properties = {
		VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT,
		NULL
	};

	VkPhysicalDeviceMemoryProperties2 memory_properties_2 = {
		VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2,
		&memory_budget_properties,
	};
	vkGetPhysicalDeviceMemoryProperties2(physical_device, &memory_properties_2);

	uint32_t memory_budget = 0;
	uint32_t process_used_memory = 0;

	for (uint32_t i = 0; i < VK_MAX_MEMORY_HEAPS; i++) {
		memory_budget += (uint32_t)memory_budget_properties.heapBudget[i];
		process_used_memory += (uint32_t)memory_budget_properties.heapUsage[i];
	}
	
	(p_memory_budget_properties != NULL) && memcpy(p_memory_budget_properties, &memory_budget_properties, sizeof(VkPhysicalDeviceMemoryBudgetPropertiesEXT));

	return 1;
}

#ifdef __cplusplus
}
#endif//__cplusplus