#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVulkan.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>

#define SWAPCHAIN_IMAGE_COUNT       2
#define MAX_SWAPCHAIN_IMAGE_COUNT   6
#define RENDERPASS_ATTACHMENT_COUNT 1



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
	glfwWindowHint(GLFW_RESIZABLE,  GLFW_TRUE);

	int width  = 720;
	int height = 480;

	uint32_t     instance_extension_count = 0;
	GLFWwindow*  window                   = glfwCreateWindow(720, 480, "vulkan resizable clear color", NULL, NULL);
	const char** pp_instance_extensions   = glfwGetRequiredInstanceExtensions(&instance_extension_count);


	VkInstance                       instance                                         = VK_NULL_HANDLE;
											                                          
	VkSurfaceKHR                     surface                                          = VK_NULL_HANDLE;
	VkSurfaceCapabilitiesKHR         surface_capabilities                             = { 0 };
																	                  
	VkPhysicalDevice                 physical_device                                  = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties       physical_device_properties                       = { 0 };
	VkPhysicalDeviceFeatures         physical_device_features                         = { 0 };
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties                = { 0 };
																	                  
	uint32_t                         graphics_queue_family_index                      = 0;
	uint32_t                         present_queue_family_index                       = 0;
																	                  
	VkDevice                         device                                           = VK_NULL_HANDLE;
	uint32_t                         device_extension_count                           = 0;
															                          
	VkQueue                          graphics_queue                                   = VK_NULL_HANDLE;
	VkQueue                          present_queue                                    = VK_NULL_HANDLE;
					           						                                      
	VkCommandPool                    graphics_cmd_pool                                = VK_NULL_HANDLE;
	VkCommandPool                    present_cmd_pool                                 = VK_NULL_HANDLE;
															                      
	VkCommandBuffer                  graphics_cmd_buffers[MAX_SWAPCHAIN_IMAGE_COUNT]  = { VK_NULL_HANDLE };
	VkCommandBuffer                  present_cmd_buffer                               = VK_NULL_HANDLE;
											
	VkFence                          graphics_cmd_fences[MAX_SWAPCHAIN_IMAGE_COUNT]   = { VK_NULL_HANDLE };

	VkSemaphore                      current_image_acquired_semaphore                 = VK_NULL_HANDLE;
	VkSemaphore                      current_graphics_queue_finished_semaphore        = VK_NULL_HANDLE;
																				      
	VkSwapchainKHR                   swapchain                                        = VK_NULL_HANDLE;
	VkFormat                         swapchain_image_format                           = 0;
	uint32_t                         swapchain_image_count                            = 0;
																	                  
	uint32_t                         sample_count                                     = 0;
																	                  
	VkAttachmentDescription          swapchain_attachment                             = { 0 };
	VkAttachmentReference            swapchain_attachment_reference                   = { 0 };
	VkSubpassDescription             subpass                                          = { 0 };
																	                  
	VkRenderPass                     renderpass                                       = VK_NULL_HANDLE;

	VkImage                          swapchain_images[MAX_SWAPCHAIN_IMAGE_COUNT]      = { VK_NULL_HANDLE };
	VkImageView                      swapchain_image_views[MAX_SWAPCHAIN_IMAGE_COUNT] = { VK_NULL_HANDLE };

	VkFramebuffer                    framebuffers[MAX_SWAPCHAIN_IMAGE_COUNT]          = { VK_NULL_HANDLE };

	shCreateInstance(
		"vulkan app",//application_name,
		"vulkan engine",//engine_name, 
		1,//enable_validation_layers,
		instance_extension_count,//extension_count,
		pp_instance_extensions,//pp_extension_names,
		VK_MAKE_API_VERSION(1, 3, 0, 0),//api_version,
		&instance//p_instance
	);

	glfwCreateWindowSurface(
		instance,
		window,
		VK_NULL_HANDLE,
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
		VK_NULL_HANDLE,//p_queue_family_count
		VK_NULL_HANDLE,//p_graphics_queue_family_count
		VK_NULL_HANDLE,//p_surface_queue_family_count
		VK_NULL_HANDLE,//p_compute_queue_family_count
		VK_NULL_HANDLE,//p_transfer_queue_family_count
		graphics_queue_families_indices,//p_graphics_queue_family_indices
		present_queue_families_indices,//p_surface_queue_family_indices
		VK_NULL_HANDLE,//p_compute_queue_family_indices
		VK_NULL_HANDLE,//p_transfer_queue_family_indices
		VK_NULL_HANDLE//p_queue_families_properties
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

	VkSharingMode swapchain_image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
	if (graphics_queue_family_index != present_queue_family_index) {
		swapchain_image_sharing_mode = VK_SHARING_MODE_CONCURRENT;
	}
	shCreateSwapchain(
		device,//device
		physical_device,//physical_device
		surface,//surface
		VK_FORMAT_R8G8B8_UNORM,//image_format
		&swapchain_image_format,//p_image_format
		SWAPCHAIN_IMAGE_COUNT,//swapchain_image_count
		swapchain_image_sharing_mode,//image_sharing_mode
		0,//vsync
		&swapchain_image_count,//p_swapchain_image_count
		&swapchain//p_swapchain
	);

	shCreateCommandPool(
		device,//device
		graphics_queue_family_index,//queue_family_index
		&graphics_cmd_pool//p_cmd_pool
	);

	shAllocateCommandBuffers(
		device,//device
		graphics_cmd_pool,//cmd_pool
		swapchain_image_count,//cmd_buffer_count
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
		device,//device
		present_cmd_pool,//cmd_pool
		1,//cmd_buffer_count
		&present_cmd_buffer//p_cmd_buffer
	);

	shCreateFences(
		device,//device
		swapchain_image_count,//fence_count
		1,//signaled
		graphics_cmd_fences//p_fences
	);

	shGetSwapchainImages(
		device,//device
		swapchain,//swapchain
		&swapchain_image_count,//p_swapchain_image_count
		swapchain_images//p_swapchain_images
	);

	for (uint32_t i = 0; i < swapchain_image_count; i++) {
		shCreateImageView(
			device,//device
			swapchain_images[i],//image
			VK_IMAGE_VIEW_TYPE_2D,//view_type
			VK_IMAGE_ASPECT_COLOR_BIT,//image_aspect
			1,//mip_levels
			swapchain_image_format,//format
			&swapchain_image_views[i]//p_image_view
		);
	}

	shCombineMaxSamples(
		physical_device_properties,//physical_device_properties
		64,//sample_count 
		1,//combine_color_sample 
		1,//combine_depth_sample 
		&sample_count//p_sample_count
	);

	shCreateRenderpassAttachment(
		swapchain_image_format,//format
		1,//sample_count
		VK_ATTACHMENT_LOAD_OP_CLEAR,//load_treatment
		VK_ATTACHMENT_STORE_OP_STORE,//store_treatment
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,//stencil_load_treatment
		VK_ATTACHMENT_STORE_OP_DONT_CARE,//stencil_store_treatment
		VK_IMAGE_LAYOUT_UNDEFINED,//initial_layout
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,//final_layout
		&swapchain_attachment//p_attachment_description
	);
	shCreateRenderpassAttachmentReference(
		0,//attachment_idx
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,//layout
		&swapchain_attachment_reference//p_attachment_reference
	);
	
	shCreateSubpass(
		VK_PIPELINE_BIND_POINT_GRAPHICS,//bind_point
		0,//input_attachment_count
		VK_NULL_HANDLE,//p_input_attachments_reference
		1,//color_attachment_count
		&swapchain_attachment_reference,//p_color_attachments_reference
		VK_NULL_HANDLE,//p_depth_stencil_attachment_reference
		VK_NULL_HANDLE,//p_resolve_attachment_reference
		0,//preserve_attachment_count
		VK_NULL_HANDLE,//p_preserve_attachments
		&subpass//p_subpass
	);

	VkAttachmentDescription attachment_descriptions[RENDERPASS_ATTACHMENT_COUNT] = {
		swapchain_attachment
	};
	shCreateRenderpass(
		device,//device
		RENDERPASS_ATTACHMENT_COUNT,//attachment_count
		attachment_descriptions,//p_attachments_descriptions
		1,//subpass_count
		&subpass,//p_subpasses
		&renderpass//p_renderpass
	);


	for (uint32_t i = 0; i < swapchain_image_count; i++) {
		VkImageView image_views[RENDERPASS_ATTACHMENT_COUNT] = {
		swapchain_image_views[i]
		};
		shCreateFramebuffer(
			device,//device
			renderpass,//renderpass
			RENDERPASS_ATTACHMENT_COUNT,//image_view_count
			image_views,//p_image_views
			surface_capabilities.currentExtent.width,//x
			surface_capabilities.currentExtent.height,//y
			1,//z
			&framebuffers[i]//p_framebuffer
		);
	}

	shCreateSemaphores(
		device,//device
		1,//semaphore_count
		&current_image_acquired_semaphore//p_semaphores
	);

	shCreateSemaphores(
		device,//device
		1,//semaphore_count
		&current_graphics_queue_finished_semaphore//p_semaphores
	);

	uint32_t swapchain_image_idx = 0;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		int _width = 0;
		int _height = 0;
		glfwGetWindowSize(window, &_width, &_height);

		if (_width != 0 && height != 0) {//otherwise it's minimized
			if (_width != width || _height != height) {//window is resized
				width  = _width;
				height = _height;

				shWaitDeviceIdle(device);

				shDestroyRenderpass(device, renderpass);
				shDestroyFramebuffers(device, swapchain_image_count, framebuffers);
				shDestroyImageViews(device, swapchain_image_count, swapchain_image_views);
				shDestroySwapchain(device, swapchain);
				shDestroySurface(instance, surface);

				glfwCreateWindowSurface(instance, window, VK_NULL_HANDLE, &surface);
				uint8_t graphics_supported = 0;
				shGetPhysicalDeviceSurfaceSupport(physical_device, graphics_queue_family_index, surface, &graphics_supported);//always true
				shGetPhysicalDeviceSurfaceCapabilities(physical_device, surface, &surface_capabilities);
				shCreateSwapchain(
					device,
					physical_device,
					surface,
					swapchain_image_format,
					&swapchain_image_format,
					swapchain_image_count,
					swapchain_image_sharing_mode,
					0,
					&swapchain_image_count,
					&swapchain
				);
				shGetSwapchainImages(device, swapchain, &swapchain_image_count, swapchain_images);
				for (uint32_t i = 0; i < swapchain_image_count; i++) {
					shCreateImageView(device, swapchain_images[i], VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, 1, swapchain_image_format, &swapchain_image_views[i]);
				}
				shCreateRenderpass(device, RENDERPASS_ATTACHMENT_COUNT, attachment_descriptions, 1, &subpass, &renderpass);
				for (uint32_t i = 0; i < swapchain_image_count; i++) {
					VkImageView image_views[RENDERPASS_ATTACHMENT_COUNT] = { swapchain_image_views[i] };
					shCreateFramebuffer(device, renderpass, RENDERPASS_ATTACHMENT_COUNT, image_views, _width, _height, 1, &framebuffers[i]);
				}

				if (swapchain_image_count == 2 && swapchain_image_idx != 0) {//start from swaphain image idx 0
					swapchain_image_idx = 0;
				}
			}
		}
		

		shAcquireSwapchainImage(
			device,//device
			swapchain,//swapchain
			UINT64_MAX,//timeout_ns
			current_image_acquired_semaphore,//acquired_signal_semaphore
			VK_NULL_HANDLE,//acquired_signal_fence
			&swapchain_image_idx//p_swapchain_image_index
		);

		shWaitForFences(
			device,//device
			1,//fence_count
			&graphics_cmd_fences[swapchain_image_idx],//p_fences
			1,//wait_for_all
			UINT64_MAX//timeout_ns
		);

		shResetFences(
			device,//device
			1,//fence_count
			&graphics_cmd_fences[swapchain_image_idx]//p_fences
		);
		shBeginCommandBuffer(graphics_cmd_buffers[swapchain_image_idx]);

		VkClearValue clear_values[1] = { 0 };
		float* p_colors = clear_values[0].color.float32;
		p_colors[0] = (float)sin(glfwGetTime());
		p_colors[1] = (float)cos(glfwGetTime());
		p_colors[2] = (float)tan(glfwGetTime());
		
		shBeginRenderpass(
			graphics_cmd_buffers[swapchain_image_idx],//graphics_cmd_buffer
			renderpass,//renderpass
			0,//render_offset_x
			0,//render_offset_y
			surface_capabilities.currentExtent.width,//render_size_x
			surface_capabilities.currentExtent.height,//render_size_y
			1,//clear_value_count, only attachments with VK_ATTACHMENT_LOAD_OP_CLEAR
			clear_values,//p_clear_values
			framebuffers[swapchain_image_idx]//framebuffer
		);

		shEndRenderpass(graphics_cmd_buffers[swapchain_image_idx]);

		shEndCommandBuffer(graphics_cmd_buffers[swapchain_image_idx]);

		shQueueSubmit(
			1,//cmd_buffer_count
			&graphics_cmd_buffers[swapchain_image_idx],//p_cmd_buffers
			graphics_queue,//queue
			graphics_cmd_fences[swapchain_image_idx],//fence
			1,//semaphores_to_wait_for_count
			&current_image_acquired_semaphore,//p_semaphores_to_wait_for
			VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,//wait_stage
			1,//signal_semaphore_count
			&current_graphics_queue_finished_semaphore//p_signal_semaphores
		);

		shQueuePresentSwapchainImage(
			present_queue,//present_queue
			1,//semaphores_to_wait_for_count
			&current_graphics_queue_finished_semaphore,//p_semaphores_to_wait_for
			swapchain,//swapchain
			swapchain_image_idx//swapchain_image_idx
		);

		if (swapchain_image_count == 2) {
			swapchain_image_idx = !swapchain_image_idx;//switch from idx 0 and 1
		}

	}

	shWaitDeviceIdle(device);

	shDestroySemaphores(device, 1, &current_image_acquired_semaphore);

	shDestroySemaphores(device, 1, &current_graphics_queue_finished_semaphore);

	shDestroyFences(device, swapchain_image_count, graphics_cmd_fences);

	shDestroyCommandBuffers(device, graphics_cmd_pool, swapchain_image_count, graphics_cmd_buffers);

	shDestroyCommandBuffers(device, present_cmd_pool, 1, &present_cmd_buffer);

	shDestroyCommandPool(device, graphics_cmd_pool);
	if (graphics_queue_family_index != present_queue_family_index) {
		shDestroyCommandPool(device, present_cmd_pool);
	}

	shDestroyRenderpass(device, renderpass);

	shDestroyFramebuffers(device, swapchain_image_count, framebuffers);

	shDestroyImageViews(device, swapchain_image_count, swapchain_image_views);

	shDestroySwapchain(device, swapchain);

	shDestroyDevice(device);

	shDestroySurface(instance, surface);

	shDestroyInstance(instance);

	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus