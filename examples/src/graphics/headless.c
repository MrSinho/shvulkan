#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVulkan.h>

#include <stdio.h>
#include <math.h>

#define SRC_IMAGE_COUNT             1
#define RENDERPASS_ATTACHMENT_COUNT 1

#include <vvo/vvo.h>



int main(void) {

	
	uint32_t width  = 400;
	uint32_t height = 400;
	
	VkInstance                       instance                                                  = VK_NULL_HANDLE;
											                                                   
	VkPhysicalDevice                 physical_device                                           = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties       physical_device_properties                                = { 0 };
	VkPhysicalDeviceFeatures         physical_device_features                                  = { 0 };
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties                         = { 0 };
																	                           
	uint32_t                         graphics_queue_family_index                               = 0;
																	                           
	VkDevice                         device                                                    = VK_NULL_HANDLE;
	uint32_t                         device_extension_count                                    = 0;
															                                   
	VkQueue                          graphics_queue                                            = VK_NULL_HANDLE;
					           						                                               
	VkCommandPool                    graphics_cmd_pool                                         = VK_NULL_HANDLE;
															                      	           
	VkCommandBuffer                  graphics_cmd_buffers[SRC_IMAGE_COUNT]               = { VK_NULL_HANDLE };
	VkFence                          graphics_cmd_fences[SRC_IMAGE_COUNT]                = { VK_NULL_HANDLE };

	VkSemaphore                      graphics_queue_finished_semaphores[SRC_IMAGE_COUNT] = { VK_NULL_HANDLE };

	VkSubpassDescription             subpass                        = { 0 };
																	                  
	VkRenderPass                     renderpass = VK_NULL_HANDLE;

	VkFramebuffer                    framebuffers[SRC_IMAGE_COUNT]          = { VK_NULL_HANDLE };

	VvoHandle vvo = { 0 };

	shCreateInstance(
		"vulkan app",//application_name,
		"vulkan engine",//engine_name, 
		1,//enable_validation_layers,
		0,//extension_count,
		NULL,//pp_extension_names,
		VK_MAKE_API_VERSION(1, 3, 0, 0),//api_version,
		&instance//p_instance
	);

	shSelectPhysicalDevice(
		instance,//instance,
		VK_NULL_HANDLE,//surface,
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
		VK_NULL_HANDLE,//surface
		VK_NULL_HANDLE,//p_queue_family_count
		VK_NULL_HANDLE,//p_graphics_queue_family_count
		VK_NULL_HANDLE,//p_surface_queue_family_count
		VK_NULL_HANDLE,//p_compute_queue_family_count
		VK_NULL_HANDLE,//p_transfer_queue_family_count
		graphics_queue_families_indices,//p_graphics_queue_family_indices
		NULL,//p_surface_queue_family_indices
		VK_NULL_HANDLE,//p_compute_queue_family_indices
		VK_NULL_HANDLE,//p_transfer_queue_family_indices
		VK_NULL_HANDLE//p_queue_families_properties
	);
	graphics_queue_family_index = graphics_queue_families_indices[0];

	float default_queue_priority = 1.0f;
	VkDeviceQueueCreateInfo graphics_device_queue_info = { 0 };
	shQueryForDeviceQueueInfo(
		graphics_queue_family_index,//queue_family_index
		1,//queue_count
		&default_queue_priority,//p_queue_priorities
		SH_FALSE,//protected
		&graphics_device_queue_info//p_device_queue_info
	);

	uint32_t device_queue_count = 1;//no eventual present queue
	VkDeviceQueueCreateInfo device_queue_infos[1] = {
		graphics_device_queue_info,
	};
	
	shSetLogicalDevice(
		physical_device,//physical_device
		&device,//p_device
		0,//extension_count
		NULL,//pp_extension_names
		device_queue_count,//device_queue_count
		device_queue_infos//p_device_queue_infos
	);

	shGetDeviceQueues(
		device,//device
		1,//queue_count
		&graphics_queue_family_index,//p_queue_family_indices
		&graphics_queue//p_queues
	);

	shCreateCommandPool(
		device,//device
		graphics_queue_family_index,//queue_family_index
		&graphics_cmd_pool//p_cmd_pool
	);

	shAllocateCommandBuffers(
		device,//device
		graphics_cmd_pool,//cmd_pool
		SRC_IMAGE_COUNT,//cmd_buffer_count
		graphics_cmd_buffers//p_cmd_buffer
	);

	shCreateFences(
		device,//device
		SRC_IMAGE_COUNT,//fence_count
		1,//signaled
		graphics_cmd_fences//p_fences
	);


	vvoInit(
		SRC_IMAGE_COUNT,//src_image_count
		width,//surface_width
		height,//surface_height
		&vvo//p_vvo
	);

	vvoVulkanInit(
		device,//device
		physical_device,//physical_device
		&vvo//p_vvo
	);
	
	vvoFindSupportedDeviceColorFormats(&vvo);

	vvoCreateSrcImages(&vvo);

	vvoCreateRenderpassColorAttachment(&vvo, 0);

	shCreateSubpass(
		VK_PIPELINE_BIND_POINT_GRAPHICS,//bind_point
		0,//input_attachment_count
		VK_NULL_HANDLE,//p_input_attachments_reference
		1,//color_attachment_count
		&vvo.color_attachment_reference,//p_color_attachments_reference
		VK_NULL_HANDLE,//p_depth_stencil_attachment_reference
		VK_NULL_HANDLE,//p_resolve_attachment_reference
		0,//preserve_attachment_count
		VK_NULL_HANDLE,//p_preserve_attachments
		&subpass//p_subpass
	);

	VkAttachmentDescription attachment_descriptions[RENDERPASS_ATTACHMENT_COUNT] = {
		vvo.color_attachment
	};
	shCreateRenderpass(
		device,//device
		RENDERPASS_ATTACHMENT_COUNT,//attachment_count
		attachment_descriptions,//p_attachments_descriptions
		1,//subpass_count
		&subpass,//p_subpasses
		&renderpass//p_renderpass
	);


	for (uint32_t i = 0; i < vvo.src_image_count; i++) {
		VkImageView image_views[RENDERPASS_ATTACHMENT_COUNT] = {
			vvo.src_image_views[i]
		};
		shCreateFramebuffer(
			device,//device
			renderpass,//renderpass
			RENDERPASS_ATTACHMENT_COUNT,//image_view_count
			image_views,//p_image_views
			width,//x
			height,//y
			1,//z
			&framebuffers[i]//p_framebuffer
		);
	}

	vvoCreateDstImage(&vvo);


	shCreateSemaphores(
		device,//device
		vvo.src_image_count,//semaphore_count
		graphics_queue_finished_semaphores//p_semaphores
	);

	uint32_t swapchain_image_idx = 0;//always will be

	VvoHandle* h = { 0 };

	char* uri = "127.0.0.1:8002";

	vvoSetupServer(&vvo, uri);

	printf("Hosting stream server at %s\n", uri);

	while (1) {
		vvoPollEvents(&vvo);

		shWaitForFences(
			device,
			1,
			&vvo.image_copy_fence,
			1,
			UINT64_MAX
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
		p_colors[0] = 1.0f;//(float)sin(glfwGetTime());
		p_colors[1] = 0.6f;//(float)cos(glfwGetTime());
		p_colors[2] = 0.8f;//(float)tan(glfwGetTime());
		p_colors[3] = 1.0f;//never forget alfa channel, otherwise image is black if image format has 4 channels

		shBeginRenderpass(
			graphics_cmd_buffers[swapchain_image_idx],//graphics_cmd_buffer
			renderpass,//renderpass
			0,//render_offset_x
			0,//render_offset_y
			width,//render_size_x
			height,//render_size_y
			1,//clear_value_count, only attachments with VK_ATTACHMENT_LOAD_OP_CLEAR
			clear_values,//p_clear_values
			framebuffers[swapchain_image_idx]//framebuffer
		);

		shEndRenderpass(graphics_cmd_buffers[swapchain_image_idx]);

		shEndCommandBuffer(graphics_cmd_buffers[swapchain_image_idx]);

		//no need to "image acquired semaphore" in headless
		if (vvo.image_submissions_count == 0) {
			shQueueSubmit(
				1,//cmd_buffer_count
				&graphics_cmd_buffers[swapchain_image_idx],//p_cmd_buffers
				graphics_queue,//queue
				graphics_cmd_fences[swapchain_image_idx],//fence
				0,//semaphores_to_wait_for_count
				NULL,//p_semaphores_to_wait_for
				VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,//wait_stage
				1,//signal_semaphore_count
				&graphics_queue_finished_semaphores[swapchain_image_idx]//p_signal_semaphores
			);
		}
		else {
			shQueueSubmit(
				1,//cmd_buffer_count
				&graphics_cmd_buffers[swapchain_image_idx],//p_cmd_buffers
				graphics_queue,//queue
				graphics_cmd_fences[swapchain_image_idx],//fence
				1,//semaphores_to_wait_for_count
				&vvo.image_copy_semaphore,//p_semaphores_to_wait_for
				VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,//wait_stage
				1,//signal_semaphore_count
				&graphics_queue_finished_semaphores[swapchain_image_idx]//p_signal_semaphores
			);
		}
		

		//HEADLESS: GET IMAGE DATA
		shWaitForFences(device, 1, &graphics_cmd_fences[swapchain_image_idx], 1, UINT64_MAX);
		
		vvoCopyToDstImage(
			&vvo,//p_vvo
			graphics_cmd_buffers[swapchain_image_idx],//transfer_cmd_buffer
			graphics_queue_family_index,//graphics_queue_family_index
			graphics_queue_family_index,//transfer_queue_family_index
			swapchain_image_idx//src_image_idx
		);

		vvoSubmitImageCopy(
			&vvo,//p_vvo
			graphics_cmd_buffers[swapchain_image_idx],//transfer_cmd_buffer
			graphics_queue,//transfer_queue
			graphics_queue_finished_semaphores[swapchain_image_idx]//graphics_queue_finished_semaphore
		);

		shWaitForFences(
			device,//device
			1,//fence_count
			&vvo.image_copy_fence,//p_fences
			1,//wait_for_all
			UINT64_MAX//timeout_ns
		);

		vvoReadDstImage(&vvo);
	}
	
	vvoVulkanRelease(&vvo);
	
	vvoRelease(&vvo);

	shWaitDeviceIdle(device);

	shDestroySemaphores(device, vvo.src_image_count, graphics_queue_finished_semaphores);

	shDestroyFences(device, vvo.src_image_count, graphics_cmd_fences);

	shDestroyCommandBuffers(device, graphics_cmd_pool, vvo.src_image_count, graphics_cmd_buffers);

	shDestroyCommandPool(device, graphics_cmd_pool);

	shDestroyRenderpass(device, renderpass);

	shDestroyFramebuffers(device, vvo.src_image_count, framebuffers);

	//shDestroyImageViews(device, vvo.src_image_count, vvo_swapchain_image_views);

	shDestroyDevice(device);

	shDestroyInstance(instance);

	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus
