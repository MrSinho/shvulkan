#ifndef SH_VK_CORE_H
#define SH_VK_CORE_HSH_MAX_STACK_QUEUE_FAMILY_COUNT

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>
#include <stdint.h>


#include "shvulkan/shVulkan.h"


#define SH_VK_QUEUE_FAMILIES_MAX_QUEUE_FAMILY_COUNT 32

typedef struct ShVkQueueFamilies {

	uint32_t                graphics_queue_family_count;
	uint32_t                surface_queue_family_count;
	uint32_t                compute_queue_family_count;

	uint32_t                graphics_queue_family_indices[SH_VK_QUEUE_FAMILIES_MAX_QUEUE_FAMILY_COUNT];
	uint32_t                surface_queue_family_indices [SH_VK_QUEUE_FAMILIES_MAX_QUEUE_FAMILY_COUNT];
	uint32_t                compute_queue_family_indices [SH_VK_QUEUE_FAMILIES_MAX_QUEUE_FAMILY_COUNT];

	VkQueueFamilyProperties queue_families_properties    [SH_VK_QUEUE_FAMILIES_MAX_QUEUE_FAMILY_COUNT];
} ShVkQueueFamilies;



#define SH_VK_CORE_MAX_DEVICE_COUNT          32

#define SH_VK_CORE_MAX_COMMAND_POOLS         32
#define SH_VK_CORE_MAX_COMMAND_BUFFER_COUNT  64
#define SH_VK_CORE_MAX_FENCE_COUNT           SH_VK_CORE_MAX_COMMAND_BUFFER_COUNT

#define SH_VK_CORE_MAX_SEMAPHORE_COUNT       SH_MAX_STACK_QUEUE_COUNT
#define SH_VK_CORE_MAX_QUEUE_COUNT           64

#define SH_VK_CORE_MAX_SWAPCHAIN_IMAGE_COUNT 16
#define SH_VK_CORE_MAX_FRAMEBUFFER_COUNT     32

#define SH_VK_CORE_MAX_RENDERPASS_COUNT      16

typedef struct ShVkCore {
	
	VkInstance                       instance;

	VkPhysicalDevice                 physical_device;
	VkPhysicalDeviceProperties       physical_device_properties;
	VkPhysicalDeviceFeatures         physical_device_features;
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
	
	uint32_t                         device_count;
	VkDevice                         devices                 [SH_VK_CORE_MAX_DEVICE_COUNT];
							         
	uint32_t                         queue_count;
	VkQueue                          queues                  [SH_MAX_STACK_QUEUE_COUNT];
	VkDeviceQueueCreateInfo          queue_infos             [SH_MAX_STACK_QUEUE_COUNT];
	uint32_t                         queue_family_indices    [SH_MAX_STACK_QUEUE_COUNT];
							         
	uint32_t                         command_pool_count;
	VkCommandPool                    cmd_pools               [SH_VK_CORE_MAX_COMMAND_POOLS];
	uint32_t                         command_buffers_count   [SH_VK_CORE_MAX_COMMAND_POOLS];
	VkCommandBuffer                  command_buffers         [SH_VK_CORE_MAX_COMMAND_BUFFER_COUNT];

	uint32_t                         fence_count;
	VkFence                          fences                  [SH_VK_CORE_MAX_FENCE_COUNT];
	uint32_t                         semaphore_count;
	VkSemaphore                      semaphores              [SH_VK_CORE_MAX_SEMAPHORE_COUNT];

	VkSurfaceKHR                     surface;
	VkSurfaceCapabilitiesKHR         surface_capabilities;

	VkSwapchainKHR                   swapchain;
	VkFormat                         swapchain_image_format;
	uint32_t                         swapchain_image_count;
	VkImage                          swapchain_images        [SH_VK_CORE_MAX_SWAPCHAIN_IMAGE_COUNT];
	VkImageView                      swapchain_image_views   [SH_VK_CORE_MAX_SWAPCHAIN_IMAGE_COUNT];

	VkFramebuffer                    framebuffers            [SH_VK_CORE_MAX_FRAMEBUFFER_COUNT];
							         
	VkImage                          depth_image;
	VkDeviceMemory                   depth_image_memory;
	VkImageView                      depth_image_view;
							         
	uint32_t                         sample_count;
	VkImage                          output_image;
	VkDeviceMemory                   output_memory;
	VkImageView                      output_image_view;
							         
	VkRenderPass                     render_passes           [SH_VK_CORE_MAX_RENDERPASS_COUNT];
} ShVkCore;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VK_CORE_H