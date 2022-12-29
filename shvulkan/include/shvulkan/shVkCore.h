#ifndef SH_VK_CORE_H
#define SH_VK_CORE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdlib.h>

#include "shvulkan/shVkVersion.h"
#include "shvulkan/shVkCheck.h"

typedef enum ShVkImageType {
	SH_SWAPCHAIN_IMAGE = 0b001,
	SH_DEPTH_IMAGE = 0b010
} ShVkImageType;

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
	/*Extensions*/
	uint32_t                            device_extension_count;
	char                                extension_names[2][VK_MAX_EXTENSION_NAME_SIZE];
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

static ShVkCore* shAllocateShVkCore() {
	ShVkCore* p_core = (ShVkCore*)calloc(1, sizeof(ShVkCore));
	shVkError(p_core == NULL, "invalid core memory", return NULL);
	return p_core;
}

static uint8_t shFreeVkCore(ShVkCore** pp_core) {
	shVkError(pp_core == NULL, "invalid core memory", return 0);
	shVkError(*pp_core == NULL, "invalid core memory", return 0);
	free(*pp_core);
	*pp_core = NULL;
	return 1;
}



extern uint8_t shCreateInstance(ShVkCore* p_core, const char* application_name, const char* engine_name, const uint8_t enable_validation_layers, const uint32_t extension_count, const char** extension_names);

extern uint8_t shSelectPhysicalDevice(ShVkCore* p_core, const VkQueueFlags requirements);

extern uint8_t shSetQueueInfo(const uint32_t queue_family_index, const float* priority, VkDeviceQueueCreateInfo* p_queue_info);

extern uint8_t shGetPhysicalDeviceSurfaceSupport(ShVkCore* p_core, ShVkQueue queue);

extern uint8_t shSetLogicalDevice(ShVkCore* p_core);

extern uint8_t shCreateSwapchain(ShVkCore* p_core);

extern uint8_t shGetSwapchainImages(ShVkCore* p_core);

extern uint8_t shCreateImageView(ShVkCore* p_core, const VkImage image, const ShVkImageType type, VkImageView* p_image_view);

extern uint8_t shCreateSwapchainImageViews(ShVkCore* p_core);

extern uint8_t shCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex, VkCommandPool* p_cmd_pool);

extern uint8_t shCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool, VkCommandBuffer* p_cmd_buffer);

extern uint8_t shCreateCommandData(ShVkCore* p_core, const VkQueueFlagBits usage, const uint32_t thread_count, ShVkCommand** pp_command);

extern uint8_t shCreateRenderPass(ShVkCore* p_core);

extern uint8_t shSetFramebuffers(ShVkCore* p_core);

extern uint8_t shSetSyncObjects(ShVkCore* p_core);

extern uint8_t shSwapchainRelease(ShVkCore* p_core);

extern uint8_t shDepthBufferRelease(ShVkCore* p_core);

extern uint8_t shSurfaceRelease(ShVkCore* p_core);

extern uint8_t shCmdRelease(ShVkCore* p_core);

extern uint8_t shRenderPassRelease(ShVkCore* p_core);

extern uint8_t shInstanceRelease(ShVkCore* p_core);

extern uint8_t shVulkanRelease(ShVkCore* p_core);



#define shWaitDeviceIdle(device)\
	vkDeviceWaitIdle((VkDevice)device);


#define shGetGraphicsQueue(p_core) vkGetDeviceQueue((p_core)->device, (p_core)->graphics_queue.queue_family_index, 0, &(p_core)->graphics_queue.queue)

#define shGetComputeQueue(p_core) vkGetDeviceQueue((p_core)->device, (p_core)->compute_queue.queue_family_index, 0, &(p_core)->compute_queue.queue)

#define shInitSwapchainData(p_core) shCreateSwapchain(p_core); shGetSwapchainImages(p_core); shCreateSwapchainImageViews(p_core)

#define shCreateDepthImageView(p_core) shCreateImageView(p_core, (p_core)->depth_image, SH_DEPTH_IMAGE, &(p_core)->depth_image_view)

#define shCreateGraphicsCommandBuffers(p_core, thread_count)\
	shCreateCommandData(p_core, VK_QUEUE_GRAPHICS_BIT, thread_count, &(p_core)->p_graphics_commands)

#define shCreateComputeCommandBuffers(p_core, thread_count)\
	shCreateCommandData(p_core, VK_QUEUE_COMPUTE_BIT, thread_count, &(p_core)->p_compute_commands)

#define shResetCommandBuffer(cmd_buffer) vkResetCommandBuffer(cmd_buffer, 0)

#define shResetFences(device, fence_count, p_fences) vkResetFences(device, fence_count, p_fences);

#define shResetFence(device, fence) vkResetFences(device, 1, &fence);

extern uint8_t shBeginCommandBuffer(const VkCommandBuffer cmd_buffer);

#define shCmdDispatch(cmd_buffer, group_count_x, group_count_y, group_count_z) vkCmdDispatch(cmd_buffer, group_count_x, group_count_y, group_count_z)

extern uint8_t shQueueSubmit(const uint32_t cmd_buffer_count, VkCommandBuffer* p_cmd_buffers, const VkQueue queue, VkFence fence);

#define shWaitForFences(device, fence_count, p_fences) vkWaitForFences(device, fence_count, p_fences, VK_TRUE, 100000000000)

#define shWaitForFence(device, fence) vkWaitForFences(device, 1, &fence, VK_TRUE, 100000000000)

#define shEndCommandBuffer(cmd_buffer) vkEndCommandBuffer(cmd_buffer)


#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VK_CORE_H