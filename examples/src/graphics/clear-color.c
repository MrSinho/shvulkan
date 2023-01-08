#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkCore.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>

#define SWAPCHAIN_IMAGE_COUNT 2


int main(void) {

	int r = glfwInit();

	shVkError(
		r < 1,
		"failed initializing glfw",
		return 0
	);

	r = glfwVulkanSupported();

	shVkError(
		r == GLFW_FALSE,
		"glfw does not support vulkan",
		return 0;
	);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE,  GLFW_FALSE);

	uint32_t    instance_extension_count = 0;
	GLFWwindow* window                   = glfwCreateWindow(720, 480, "vulkan clear color", NULL, NULL);
	char**      pp_instance_extensions   = glfwGetRequiredInstanceExtensions(&instance_extension_count);


	VkInstance                       instance                                              = NULL;
											                                               
	VkSurfaceKHR                     surface                                               = NULL;
	VkSurfaceCapabilitiesKHR         surface_capabilities                                  = { 0 };
																	                       
	VkPhysicalDevice                 physical_device                                       = NULL;
	VkPhysicalDeviceProperties       physical_device_properties                            = { 0 };
	VkPhysicalDeviceFeatures         physical_device_features                              = { 0 };
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties                     = { 0 };
																	                       
	uint32_t                         graphics_queue_family_index                           = 0;
	uint32_t                         present_queue_family_index                            = 0;
																	                       
	VkDevice                         device                                                = NULL;
	uint32_t                         device_extension_count                                = 0;
															                               
	VkQueue                          graphics_queue                                        = NULL;
	VkQueue                          present_queue                                         = NULL;
					           						                                           
	VkCommandPool                    graphics_cmd_pool                                     = NULL;
	VkCommandPool                    present_cmd_pool                                      = NULL;
															                               
	VkCommandBuffer                  graphics_cmd_buffers[SWAPCHAIN_IMAGE_COUNT]           = { NULL };
	VkCommandBuffer                  present_cmd_buffer                                    = NULL;
															                               
	VkFence                          graphics_cmd_fences[SWAPCHAIN_IMAGE_COUNT]            = { NULL };
															                      		   
	VkSemaphore                      image_acquired_semaphores[SWAPCHAIN_IMAGE_COUNT]      = { NULL };
	VkSemaphore                      renderpass_finished_semaphores[SWAPCHAIN_IMAGE_COUNT] = { NULL };

	VkSwapchainKHR                   swapchain                                             = NULL;
	VkFormat                         swapchain_image_format                                = 0;
	uint32_t                         swapchain_image_count                                 = 0;
																	                       
	uint32_t                         sample_count                                          = 0;
																	                       
	VkAttachmentDescription          swapchain_attachment                                  = { 0 };
	VkAttachmentReference            swapchain_attachment_reference                        = { 0 };
	VkAttachmentDescription          depth_attachment                                      = { 0 };
	VkAttachmentReference            depth_attachment_reference                            = { 0 };
	VkSubpassDescription             subpass                                               = { 0 };
																	                       
	VkRenderPass                     renderpass                                            = NULL;

	VkImage                          swapchain_images[SWAPCHAIN_IMAGE_COUNT]      = { NULL };
	VkImageView                      swapchain_image_views[SWAPCHAIN_IMAGE_COUNT] = { NULL };

	VkImage                          depth_image = NULL;
	VkDeviceMemory                   depth_image_memory = NULL;
	VkImageView                      depth_image_view = NULL;

	VkFramebuffer                    framebuffers[SWAPCHAIN_IMAGE_COUNT]          = { NULL };

	shCreateInstance(
		&instance,//p_instance, 
		"vulkan app",//application_name, 
		"vulkan engine",//engine_name, 
		1,//enable_validation_layers,
		instance_extension_count,//extension_count, 
		pp_instance_extensions//pp_extension_names
	);

	glfwCreateWindowSurface(
		instance,
		window,
		NULL,
		&surface
	);

	shSelectPhysicalDevice(
		instance,//instance, 
		surface,//surface, 
		VK_QUEUE_GRAPHICS_BIT |
		VK_QUEUE_COMPUTE_BIT |
		VK_QUEUE_TRANSFER_BIT,//requirements,
		&physical_device,//p_physical_device,
		&physical_device_properties,//p_physical_device_properties,
		&physical_device_features,//p_physical_device_features,
		&physical_device_memory_properties//p_physical_device_memory_properties
	);

	uint32_t graphics_queue_families_indices[SH_MAX_STACK_QUEUE_FAMILY_COUNT] = { 0 };
	uint32_t present_queue_families_indices [SH_MAX_STACK_QUEUE_FAMILY_COUNT] = { 0 };
	shGetPhysicalDeviceQueueFamilies(
		physical_device,//physical_device
		surface,//surface
		NULL,//p_queue_family_count
		NULL,//p_graphics_queue_family_count
		NULL,//p_surface_queue_family_count
		NULL,//p_compute_queue_family_count
		NULL,//p_transfer_queue_family_count
		graphics_queue_families_indices,//p_graphics_queue_family_indices
		present_queue_families_indices,//p_surface_queue_family_indices
		NULL,//p_compute_queue_family_indices
		NULL,//p_transfer_queue_family_indices
		NULL//p_queue_families_properties
	);
	graphics_queue_family_index = graphics_queue_families_indices[0];
	present_queue_family_index  = present_queue_families_indices [0];

	shGetPhysicalDeviceSurfaceCapabilities(
		physical_device,//physical_device
		surface,//surface
		&surface_capabilities//p_surface_capabilities
	);

	float default_queue_priority = 1.0f;
	VkDeviceQueueCreateInfo graphics_device_queue_info = { 0 };
	shQueryForDeviceQueueInfo(
		graphics_queue_family_index,//queue_family_index
		1,//queue_count
		&default_queue_priority,//p_queue_priorities
		0,//protected
		&graphics_device_queue_info//p_device_queue_info
	);

	VkDeviceQueueCreateInfo present_device_queue_info = { 0 };
	shQueryForDeviceQueueInfo(
		present_queue_family_index,//queue_family_index
		1,//queue_count
		&default_queue_priority,//p_queue_priorities
		0,//protected
		&present_device_queue_info//p_device_queue_info
	);

	VkDeviceQueueCreateInfo device_queue_infos[2] = {
		graphics_device_queue_info,
		present_device_queue_info
	};
	char* device_extensions[2]  = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	uint32_t device_queue_count = (graphics_queue_family_index == present_queue_family_index) ? 1 : 2;
	shSetLogicalDevice(
		physical_device,//physical_device
		&device,//p_device
		1,//extension_count
		device_extensions,//pp_extension_names
		device_queue_count,//device_queue_count
		device_queue_infos//p_device_queue_infos
	);

	shGetDeviceQueues(
		device,//device
		1,//queue_count
		&graphics_queue_family_index,//p_queue_family_indices
		&graphics_queue//p_queues
	);

	if (graphics_queue_family_index != present_queue_family_index) {
		shGetDeviceQueues(
			device,//device
			1,//queue_count
			&present_queue_family_index,//p_queue_family_indices
			&present_queue//p_queues
		);
	}
	else {
		present_queue = graphics_queue;
	}

	shCreateCommandPool(
		device,//device
		graphics_queue_family_index,//queue_family_index
		&graphics_cmd_pool//p_cmd_pool
	);

	shAllocateCommandBuffers(
		device,//device
		graphics_cmd_pool,//cmd_pool
		SWAPCHAIN_IMAGE_COUNT,//cmd_buffer_count
		graphics_cmd_buffers//p_cmd_buffer
	);

	if (graphics_queue_family_index != present_queue_family_index) {
		shCreateCommandPool(
			device,//device
			present_queue_family_index,//queue_family_index
			&present_cmd_pool//p_cmd_pool
		);
	}
	else {
		present_cmd_pool   = graphics_cmd_pool;
	}
	shAllocateCommandBuffers(
		device,
		present_cmd_pool,
		1,
		&present_cmd_buffer
	);

	shCreateFences(
		device,
		SWAPCHAIN_IMAGE_COUNT,
		1,
		graphics_cmd_fences
	);

	VkSharingMode swapchain_image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
	if (graphics_queue_family_index != present_queue_family_index) {
		swapchain_image_sharing_mode = VK_SHARING_MODE_CONCURRENT;
	}
	shCreateSwapchain(
		device,
		physical_device,
		surface,
		VK_FORMAT_R8G8B8_UNORM,
		&swapchain_image_format,
		SWAPCHAIN_IMAGE_COUNT,
		swapchain_image_sharing_mode,
		0,
		&swapchain
	);

	shGetSwapchainImages(
		device,
		swapchain,
		&swapchain_image_count,
		swapchain_images
	);

	for (uint32_t i = 0; i < swapchain_image_count; i++) {
		shCreateImageView(
			device,
			swapchain_images[i],
			VK_IMAGE_VIEW_TYPE_2D,
			VK_IMAGE_ASPECT_COLOR_BIT,
			1,
			swapchain_image_format,
			&swapchain_image_views[i]
		);
	}

	shCombineMaxSamples(
		physical_device_properties,
		64, 1, 1, &sample_count
	);

	shCreateRenderpassAttachment(
		swapchain_image_format,
		1,//sample_count
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		&swapchain_attachment
	);
	shCreateRenderpassAttachmentReference(
		0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		&swapchain_attachment_reference
	);

	shCreateRenderpassAttachment(
		VK_FORMAT_D32_SFLOAT,
		1,//sample_count
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		&depth_attachment
	);
	shCreateRenderpassAttachmentReference(
		1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		&depth_attachment_reference
	);
	
	shCreateSubpass(
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		0,
		NULL,
		1,
		&swapchain_attachment_reference,
		&depth_attachment_reference,
		NULL,//resolve_attachment_reference
		0,
		NULL,
		&subpass
	);

	VkAttachmentDescription attachment_descriptions[2] = {
		swapchain_attachment, depth_attachment
	};
	shCreateRenderpass(
		device,
		2,
		attachment_descriptions,
		1,
		&subpass,
		&renderpass
	);


	shCreateImage(
		device,
		VK_IMAGE_TYPE_2D,
		surface_capabilities.currentExtent.width,
		surface_capabilities.currentExtent.height,
		1,
		VK_FORMAT_D32_SFLOAT,
		1,
		1,//sample_count
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		&depth_image
	);
	shAllocateImageMemory(
		device,
		physical_device,
		depth_image,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&depth_image_memory
	);
	shBindImageMemory(
		device,
		depth_image,
		0,
		depth_image_memory
	);
	shCreateImageView(
		device,
		depth_image,
		VK_IMAGE_VIEW_TYPE_2D,
		VK_IMAGE_ASPECT_DEPTH_BIT,
		1,
		VK_FORMAT_D32_SFLOAT,
		&depth_image_view
	);


	for (uint32_t i = 0; i < swapchain_image_count; i++) {
		VkImageView image_views[3] = {
		swapchain_image_views[i], depth_image_view
		};
		shCreateFramebuffer(
			device,
			renderpass,
			2,
			image_views,
			surface_capabilities.currentExtent.width,
			surface_capabilities.currentExtent.height,
			1,
			&framebuffers[i]
		);
	}

	shCreateSemaphores(
		device,
		swapchain_image_count,
		image_acquired_semaphores
	);

	shCreateSemaphores(
		device,
		swapchain_image_count,
		renderpass_finished_semaphores
	);

	uint32_t swapchain_image_idx = 0;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		shAcquireSwapchainImage(
			device,
			swapchain,
			UINT64_MAX,
			image_acquired_semaphores[swapchain_image_idx],
			NULL,
			&swapchain_image_idx
		);

		shResetFences(
			device, 
			1, 
			&graphics_cmd_fences[swapchain_image_idx]
		);
		shBeginCommandBuffer(graphics_cmd_buffers[swapchain_image_idx]);

		float red   = (float)sin(glfwGetTime());
		float green = (float)cos(glfwGetTime());
		float blue  = (float)tan(glfwGetTime());
		shBeginRenderpass(
			graphics_cmd_buffers[swapchain_image_idx],
			renderpass,
			framebuffers[swapchain_image_idx],
			0, 0,
			surface_capabilities.currentExtent.width,
			surface_capabilities.currentExtent.height,
			red, green, blue, 1.0f,
			1, 0.0f, 0
		);

		shEndRenderpass(graphics_cmd_buffers[swapchain_image_idx]);

		shEndCommandBuffer(graphics_cmd_buffers[swapchain_image_idx]);

		shQueueSubmit(
			1,
			&graphics_cmd_buffers[swapchain_image_idx],
			graphics_queue,
			graphics_cmd_fences[swapchain_image_idx],
			1,
			&image_acquired_semaphores[swapchain_image_idx],
			VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
			1,
			&renderpass_finished_semaphores[swapchain_image_idx]
		);
		shWaitForFences(
			device,
			1,
			&graphics_cmd_fences[swapchain_image_idx],
			1,
			UINT64_MAX
		);

		shQueuePresentSwapchainImage(
			present_queue,
			1,
			&renderpass_finished_semaphores[swapchain_image_idx],
			swapchain,
			swapchain_image_idx
		);

		if (swapchain_image_count == 2) {
			swapchain_image_idx = !swapchain_image_idx;
		}

	}

	shWaitDeviceIdle(device);

	shDestroySemaphores(
		device,
		swapchain_image_count,
		image_acquired_semaphores
	);

	shDestroySemaphores(
		device,
		swapchain_image_count,
		renderpass_finished_semaphores
	);

	shDestroyFences(
		device,
		swapchain_image_count,
		graphics_cmd_fences
	);

	shDestroyCommandBuffers(
		device,
		graphics_cmd_pool,
		swapchain_image_count,
		graphics_cmd_buffers
	);

	shDestroyCommandBuffers(
		device,
		present_cmd_pool,
		1,
		&present_cmd_buffer
	);

	shDestroyCommandPool(
		device,
		graphics_cmd_pool
	);
	if (graphics_queue_family_index != present_queue_family_index) {
		shDestroyCommandPool(
			device,
			present_cmd_pool
		);
	}

	shDestroyRenderpass(
		device,
		renderpass
	);

	shDestroyFramebuffers(
		device,
		swapchain_image_count,
		framebuffers
	);

	shDestroyImageViews(
		device,
		swapchain_image_count,
		swapchain_image_views
	);

	shDestroyImageViews(
		device,
		1,
		&depth_image_view
	);

	shClearImageMemory(device, depth_image, depth_image_memory);

	shDestroySwapchain(device, swapchain);

	shDestroyDevice(device);

	shDestroySurface(instance, surface);

	shDestroyInstance(instance);

	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus