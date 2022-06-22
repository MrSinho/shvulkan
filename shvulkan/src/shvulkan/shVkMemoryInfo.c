#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkMemoryInfo.h"
#include "shvulkan/shVkCheck.h"

#include <stdlib.h>
#include <memory.h>



void shCreateBuffer(const VkDevice device, const uint32_t size, VkBufferUsageFlags usage_flags, VkBuffer* p_buffer) {
	shVkError(p_buffer == NULL, "invalid arguments", return);
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
		"error creating buffer", return
	);
}

void shAllocateMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const VkMemoryPropertyFlags property_flags, VkDeviceMemory *p_memory) {
	shVkError(p_memory == NULL, "invalid device memory pointer", return);
	uint32_t memory_type_index = 0;
	shGetMemoryType(device, physical_device, property_flags, &memory_type_index);

	VkMemoryRequirements memory_requirements;
	vkGetBufferMemoryRequirements(device, buffer, &memory_requirements);

	VkMemoryAllocateInfo memory_allocate_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,	//sType;
		NULL,									//pNext;
		memory_requirements.size,				//allocationSize;
		memory_type_index						//memoryTypeIndex;
	};

	shVkResultError(
		vkAllocateMemory(device, &memory_allocate_info, NULL, p_memory),
		"error allocating memory", return
	);
}

void shGetMemoryType(const VkDevice device, const VkPhysicalDevice physical_device, const VkMemoryPropertyFlags property_flags, uint32_t* p_memory_type_index) {
	shVkError(p_memory_type_index == NULL, "invalid memory type index pointer", return);
	VkPhysicalDeviceMemoryProperties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

	for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {

		if (memory_properties.memoryTypes[i].propertyFlags & (1 << i) &&
			(memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
			*p_memory_type_index = i;
			return;
		}

	}
	shVkError(VK_ERROR_UNKNOWN,
		"cannot find suitable memory type for the given buffer", return
	);
}

void shReadMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t offset, const uint32_t data_size, void* p_data) {
	shVkError(p_data == NULL, "invalid memory buffer", return);
	void* data;
	shVkResultError(
		vkMapMemory(device, memory, offset, data_size, 0, &data),
		"error mapping memory", return
	);
	memcpy(p_data, data, (size_t)data_size);
	vkUnmapMemory(device, memory);
}

void shWriteMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t offset, const uint32_t data_size, const void* p_data) {
	shVkError(p_data == NULL, "invalid memory buffer", return);
	void* data;
	shVkResultError(
		vkMapMemory(device, memory, offset, data_size, 0, &data),
		"error mapping memory", return
	);
	memcpy(data, p_data, (size_t)data_size);
	vkUnmapMemory(device, memory);
}

void shClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory) {
	vkDeviceWaitIdle(device);
	vkDestroyBuffer(device, buffer, NULL);
	vkFreeMemory(device, memory, NULL);
}

void shCreateImage(const VkDevice device, const VkPhysicalDevice physical_device, const uint32_t width, const uint32_t height, VkFormat format, VkImageUsageFlags usage, VkImage* p_image, VkDeviceMemory* p_image_memory) {
	VkExtent3D image_extent = {
		width, height, 1
	};

	VkImageCreateInfo image_create_info = {
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,	//sType;			
		NULL,									//pNext;
		0,										//flags;
		VK_IMAGE_TYPE_2D,						//imageType;
		format,									//format;
		image_extent,							//extent;
		1,										//mipLevels;
		1,										//arrayLayers;
		VK_SAMPLE_COUNT_1_BIT,					//samples;
		VK_IMAGE_TILING_OPTIMAL,				//tiling;
		usage,									//usage;
		VK_SHARING_MODE_EXCLUSIVE,				//sharingMode;
		0,										//queueFamilyIndexCount;
		NULL									//pQueueFamilyIndices;
	};
	shVkResultError(vkCreateImage(device, &image_create_info, NULL, p_image),
		"error creating image", return
	);

	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(device, *p_image, &memory_requirements);

	uint32_t memory_type_index = 0;
	shGetMemoryType(device, physical_device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memory_type_index);

	VkMemoryAllocateInfo memory_allocate_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		NULL,
		memory_requirements.size,
		memory_type_index
	};

	shVkResultError(vkAllocateMemory(device, &memory_allocate_info, NULL, p_image_memory),
		"error allocating image memory", return
	);

	vkBindImageMemory(device, *p_image, *p_image_memory, 0);
}

void shGetMemoryBudgetProperties(const VkPhysicalDevice physical_device, uint32_t* p_memory_budget, uint32_t* p_process_used_memory, VkPhysicalDeviceMemoryBudgetPropertiesEXT* p_memory_budget_properties) {

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
	
	(p_memory_budget != NULL) && (*p_memory_budget = memory_budget);
	(p_process_used_memory != NULL ) && (*p_process_used_memory = process_used_memory);
	(p_memory_budget_properties != NULL) && memcpy(p_memory_budget_properties, &memory_budget_properties, sizeof(VkPhysicalDeviceMemoryBudgetPropertiesEXT));
}

#ifdef __cplusplus
}
#endif//__cplusplus