#ifndef SH_VK_CORE_H
#define SH_VK_CORE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "shvulkan/shVkVersion.h"
#include "shvulkan/shVkCheck.h"

typedef enum shImageType {
	SH_SWAPCHAIN_IMAGE = 0b001,
	SH_DEPTH_IMAGE = 0b010
} shImageType;

#define SH_DEPTH_IMAGE_FORMAT		VK_FORMAT_D32_SFLOAT
#define SH_SWAPCHAIN_IMAGE_FORMAT	VK_FORMAT_R8G8B8A8_UNORM

typedef struct ShVkSurface {
	VkSurfaceKHR				surface;
	uint32_t					width;
	uint32_t					height;
	VkSurfaceCapabilitiesKHR	surface_capabilities;
} ShVkSurface;

typedef struct ShVkQueue {
	uint32_t		queue_family_index;
	VkQueue			queue;
} ShVkQueue;

typedef struct ShVkCommand {
	VkCommandBuffer cmd_buffer;
	VkCommandPool	cmd_pool;
	VkFence			fence;
} ShVkCommand;

typedef struct ShVkCore {
	/*Primary*/
	VkInstance							instance;
	VkPhysicalDevice					physical_device;
	VkPhysicalDeviceProperties			physical_device_properties;
	VkPhysicalDeviceFeatures			physical_device_features;
	VkPhysicalDeviceMemoryProperties	physical_device_memory_properties;
	VkDevice							device;
	/*Surface*/
	ShVkSurface							surface;
	/*Queues*/
	VkQueueFlags						required_queue_flags;
	ShVkQueue							graphics_queue;
	ShVkQueue							compute_queue;
	/*Commands*/
	uint32_t							thread_count;
	ShVkCommand*						p_graphics_commands;
	ShVkCommand*						p_compute_commands;
	/*Swapchain*/
	VkSwapchainKHR						swapchain;
	VkFormat							swapchain_image_format;
	uint32_t							swapchain_image_count;
	VkImage*							p_swapchain_images;
	VkImageView*						p_swapchain_image_views;
	VkFramebuffer*						p_frame_buffers;
	/*Depth buffer*/
	VkImage								depth_image;
	VkDeviceMemory						depth_image_memory;
	VkImageView							depth_image_view;
	/**/
	VkRenderPass						render_pass;
	VkSemaphore*						p_render_semaphores;
} ShVkCore;



#define SH_VK_CORE_GRAPHICS VK_QUEUE_GRAPHICS_BIT

#define SH_VK_CORE_COMPUTE VK_QUEUE_COMPUTE_BIT

extern void shCreateInstance(ShVkCore* p_core, const char* application_name, const char* engine_name, const uint8_t enable_validation_layers, const uint32_t extension_count, const char** extension_names);

extern void shSelectPhysicalDevice(ShVkCore* p_core, const VkQueueFlags requirements);

extern void shSetQueueInfo(const uint32_t queue_family_index, const float* priority, VkDeviceQueueCreateInfo* p_queue_info);

extern void shSetLogicalDevice(ShVkCore* p_core, VkQueueFlags requirements);

extern void shCreateSwapchain(ShVkCore* p_core);

extern void shGetSwapchainImages(ShVkCore* p_core);

extern void shCreateImageView(ShVkCore* p_core, const VkImage image, const shImageType type, VkImageView* p_image_view);

extern void shCreateSwapchainImageViews(ShVkCore* p_core);

extern void shCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex, VkCommandPool* p_cmd_pool);

extern void shCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool, VkCommandBuffer* p_cmd_buffer);

extern void shCreateCommandData(ShVkCore* p_core, const VkQueueFlagBits usage, const uint32_t thread_count, ShVkCommand** pp_command);

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



#define shGetGraphicsQueue(p_core) vkGetDeviceQueue((p_core)->device, (p_core)->graphics_queue.queue_family_index, 0, &(p_core)->graphics_queue.queue)

#define shGetComputeQueue(p_core) vkGetDeviceQueue((p_core)->device, (p_core)->compute_queue.queue_family_index, 0, &(p_core)->compute_queue.queue)

#define shInitSwapchainData(p_core) shCreateSwapchain(p_core); shGetSwapchainImages(p_core); shCreateSwapchainImageViews(p_core)

#define shCreateDepthImageView(p_core) shCreateImageView(p_core, (p_core)->depth_image, SH_DEPTH_IMAGE, &(p_core)->depth_image_view)

#define shCreateGraphicsCommandBuffers(p_core, thread_count)\
	shCreateCommandData(p_core, VK_QUEUE_GRAPHICS_BIT, thread_count, &(p_core)->p_graphics_commands)\

#define shCreateComputeCommandBuffers(p_core, thread_count)\
	shCreateCommandData(p_core, VK_QUEUE_COMPUTE_BIT, thread_count, &(p_core)->p_compute_commands)\

#define shResetCommandBuffer(cmd_buffer) vkResetCommandBuffer(cmd_buffer, 0)

#define shResetFences(device, fence_count, p_fences) vkResetFences(device, fence_count, p_fences);

#define shResetFence(device, p_fence) vkResetFences(device, 1, p_fence);

extern void shBeginCommandBuffer(const VkCommandBuffer cmd_buffer);

#define shCmdDispatch(cmd_buffer, group_count_x, group_count_y, group_count_z) vkCmdDispatch(cmd_buffer, group_count_x, group_count_y, group_count_z)

extern void shQueueSubmit(VkCommandBuffer* cmd_buffer, const VkQueue queue, VkFence fence);

#define shWaitForFences(device, fence_count, p_fences) vkWaitForFences(device, fence_count, p_fences, VK_TRUE, 100000000000)

#define shWaitForFence(device, p_fence) vkWaitForFences(device, 1, p_fence, VK_TRUE, 100000000000)

#define shEndCommandBuffer(cmd_buffer) vkEndCommandBuffer(cmd_buffer)


#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VK_CORE_H