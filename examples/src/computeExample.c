#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDrawLoop.h>

int main(void) {
	ShVkCore core = shVkCoreInitPrerequisites();
	shCreateInstance(&core, "Vulkan compute example", "shVulkan Engine", 0, NULL);
	shSelectPhysicalDevice(&core, SH_VK_CORE_COMPUTE);
	shSetLogicalDevice(&core);
	shGetComputeQueue(&core);
	shCreateComputeCommandBuffer(&core);

	#define BUFFER_SIZE 128

	VkBuffer buffer;
	VkDeviceMemory buffer_memory;

	return 0;
}