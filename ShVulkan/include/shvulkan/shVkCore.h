#ifndef SH_VK_CORE_H
#define SH_VK_CORE_H

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "shvulkan/shVkVersion.h"

typedef enum shImageType {
	SH_SWAPCHAIN_IMAGE = 0b001,
	SH_DEPTH_IMAGE = 0b010
} shImageType;

#define SH_DEPTH_IMAGE_FORMAT		VK_FORMAT_D32_SFLOAT
#define SH_SWAPCHAIN_IMAGE_FORMAT	VK_FORMAT_R8G8B8A8_UNORM

typedef struct ShSurface {
	VkSurfaceKHR				surface;
	uint32_t					width;
	uint32_t					height;
	VkSurfaceCapabilitiesKHR	surface_capabilities;
} ShSurface;

typedef struct ShQueue {
	uint32_t		queue_family_index;
	VkQueue			queue;
} ShQueue;

typedef struct ShVkCore {
	/*Primary*/
	VkInstance					instance;
	VkPhysicalDevice			physical_device;
	VkPhysicalDeviceProperties	physical_device_properties;
	VkPhysicalDeviceFeatures	physical_device_features;
	VkDevice					device;
	/*Surface*/
	ShSurface					surface;
	/*Queues*/
	VkQueueFlags				required_queue_flags;
	ShQueue						graphics_queue;
	ShQueue						compute_queue;
	/*Commands*/
	VkCommandPool				graphics_cmd_pool;
	VkCommandPool				compute_cmd_pool;
	VkCommandBuffer				graphics_cmd_buffer;
	VkCommandBuffer				compute_cmd_buffer;
	/*Swapchain*/
	VkSwapchainKHR				swapchain;
	VkFormat					swapchain_image_format;
	uint32_t					swapchain_image_count;
	VkImage*					p_swapchain_images;
	VkImageView*				p_swapchain_image_views;
	VkFramebuffer*				p_frame_buffers;
	/*Depth buffer*/
	VkImage						depth_image;
	VkDeviceMemory				depth_image_memory;
	VkImageView					depth_image_view;
	/*Render pass + sync objects*/
	VkRenderPass				render_pass;
	VkSemaphore					render_semaphore;
	VkSemaphore					present_semaphore;
	VkFence						render_fence;
} ShVkCore;


#define shGetGraphicsQueue(p_core) vkGetDeviceQueue((p_core)->device, (p_core)->graphics_queue.queue_family_index, 0, &(p_core)->graphics_queue.queue)

#define shGetComputeQueue(p_core) vkGetDeviceQueue((p_core)->device, (p_core)->compute_queue.queue_family_index, 0, &(p_core)->compute_queue.queue)

#define shInitSwapchainData(p_core) shCreateSwapchain(p_core); shGetSwapchainImages(p_core); shCreateSwapchainImageViews(p_core)

#define shCreateDepthImageView(p_core) shCreateImageView(p_core, (p_core)->depth_image, SH_DEPTH_IMAGE, &(p_core)->depth_image_view)

#define shCreateGraphicsCommandBuffer(p_core) shCreateCmdPool((p_core)->device, (p_core)->graphics_queue.queue_family_index, &(p_core)->graphics_cmd_pool); shCreateCmdBuffer((p_core)->device, (p_core)->graphics_cmd_pool, &(p_core)->graphics_cmd_buffer)

#define shCreateComputeCommandBuffer(p_core) shCreateCmdPool((p_core)->device, (p_core)->compute_queue.queue_family_index, &(p_core)->compute_cmd_pool); shCreateCmdBuffer((p_core)->device, (p_core)->compute_cmd_pool, &(p_core)->compute_cmd_buffer)

#define SH_VK_CORE_GRAPHICS VK_QUEUE_GRAPHICS_BIT

#define SH_VK_CORE_COMPUTE VK_QUEUE_COMPUTE_BIT

extern void shCreateInstance(ShVkCore* p_core, const char* application_name, const char* engine_name, const uint32_t extension_count, const char** extension_names);

extern void shGetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* p_surface_capabilities);

extern void shSelectPhysicalDevice(ShVkCore* p_core, const VkQueueFlags requirements);

extern void shSetQueueInfo(const uint32_t queueFamilyIndex, const float* priority, VkDeviceQueueCreateInfo* p_queue_info);

extern void shSetLogicalDevice(ShVkCore* p_core);

extern void shCreateSwapchain(ShVkCore* p_core);

extern void shGetSwapchainImages(ShVkCore* p_core);

extern void shCreateImageView(ShVkCore* p_core, const VkImage image, const shImageType type, VkImageView* p_image_view);

extern void shCreateSwapchainImageViews(ShVkCore* p_core);

extern void shCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex, VkCommandPool* p_cmd_pool);

extern void shCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool, VkCommandBuffer* p_cmd_buffer);

extern void shCreateRenderPass(ShVkCore* p_core);

extern void shSetFramebuffers(ShVkCore* p_core);

extern void shSetSyncObjects(ShVkCore* p_core);

extern void shSwapchainRelease(ShVkCore* p_core);

extern void shDepthBufferRelease(ShVkCore* p_core);

extern void shSurfaceRelease(ShVkCore* p_core);

extern void shCmdRelease(ShVkCore* p_core);

extern void shRenderPassRelease(ShVkCore* p_core);

extern void shInstanceRelease(ShVkCore* p_core);

extern void shVulkanRelease(ShVkCore* p_core);

#endif//SH_VK_CORE_H