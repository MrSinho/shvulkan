#ifndef SH_VULKAN_1_0_H
#define SH_VULKAN_1_0_H

#include <vulkan/vulkan.h>

#ifdef SH_USE_VULKAN_1_0
	#define SH_VULKAN_VERSION VK_API_VERSION_1_0
#elif defined SH_USE_VULKAN_1_1
	#define SH_VULKAN_VERSION VK_API_VERSION_1_0
#else
	#define SH_VULKAN_VERSION VK_API_VERSION_1_2
#endif

#endif