#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDrawLoop.h>

#ifndef NDEBUG
#define VALIDATION_LAYERS_ENABLED 1
#else
#define VALIDATION_LAYERS_ENABLED 0
#endif//NDEBUG

int main(void) {
	ShVkCore core = { 0 };
	shCreateInstance(&core, "Vulkan compute example", "shVulkan Engine", VALIDATION_LAYERS_ENABLED, 0, NULL);
	shSelectPhysicalDevice(&core, SH_VK_CORE_COMPUTE);
	shSetLogicalDevice(&core);
	shGetComputeQueue(&core);
	shCreateComputeCommandBuffer(&core);

	#define BUFFER_SIZE 32 * 4

	VkBuffer src_buffer;
	VkDeviceMemory src_buffer_memory;
	shCreateBuffer(core.device, BUFFER_SIZE, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, &src_buffer);
	shAllocateMemory(
		core.device,
		core.physical_device,
		src_buffer,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&src_buffer_memory	
	);

	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus