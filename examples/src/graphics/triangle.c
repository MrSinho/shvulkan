#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVulkan.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>

#define SWAPCHAIN_IMAGE_COUNT 1
#define RENDERPASS_ATTACHMENT_COUNT 1

#define QUAD_VERTEX_COUNT     20
#define TRIANGLE_VERTEX_COUNT 15
#define QUAD_INDEX_COUNT      6

#define PER_VERTEX_BINDING    0
#define PER_INSTANCE_BINDING  1

float quad[QUAD_VERTEX_COUNT] = {
		-0.5f,-0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, 0.5f, 0.0f,  0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,  0.0f, 0.0f,
};

float triangle[TRIANGLE_VERTEX_COUNT] = {
		-1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 
		 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 
		 1.0f, 1.0f, 0.0f,  0.0f, 0.0f
};

float models[48] = {
	0.2f, 0.0f, 0.0f, 0.0f,//model 0
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.2f, 0.0f,
   -0.6f,-0.2f, 0.0f, 1.0f,

	0.2f, 0.0f, 0.0f, 0.0f,//model 1
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.2f, 0.0f,
	0.6f,-0.2f, 0.0f, 1.0f,

	0.6f, 0.0f, 0.0f, 0.0f,//model 2
	0.0f, 0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.4f, 0.0f, 1.0f
};

uint32_t indices[QUAD_INDEX_COUNT] = {
	0, 1, 2,
	2, 3, 0
};

float light[8] = {
	0.0f,  2.0f, 0.0f, 1.0f, //position
	0.0f, 0.45f, 0.9f, 1.0f	 //color
};

float projection_view[32] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

void writeMemory(
	VkDevice         device,
	VkPhysicalDevice physical_device,
	VkCommandBuffer  cmd_buffer,
	VkFence          fence,
	VkQueue          transfer_queue,
	VkBuffer*        p_staging_buffer,
	VkDeviceMemory*  p_staging_memory,
	VkBuffer*        p_vertex_buffer,
	VkDeviceMemory*  p_vertex_memory,
	VkBuffer*        p_instance_buffer,
	VkDeviceMemory*  p_instance_memory,
	VkBuffer*        p_index_buffer,
	VkDeviceMemory*  p_index_memory,
	VkBuffer*        p_descriptors_buffer,
	VkDeviceMemory*  p_descriptors_memory
);

void releaseMemory(
	VkDevice       device,
	VkBuffer       staging_buffer,
	VkDeviceMemory staging_memory,
	VkBuffer       vertex_buffer,
	VkDeviceMemory vertex_memory,
	VkBuffer       instance_buffer,
	VkDeviceMemory instance_memory,
	VkBuffer       index_buffer,
	VkDeviceMemory index_memory,
	VkBuffer       descriptors_buffer,
	VkDeviceMemory descriptors_memory
);

char* readBinary(
	const char* path, 
	uint32_t* p_size
);

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

	uint32_t    instance_extension_count = 0;
	GLFWwindow* window                   = glfwCreateWindow(720, 480, "vulkan resizable clear color", NULL, NULL);
	char**      pp_instance_extensions   = glfwGetRequiredInstanceExtensions(&instance_extension_count);


	VkInstance                       instance                                     = NULL;
											                                      
	VkSurfaceKHR                     surface                                      = NULL;
	VkSurfaceCapabilitiesKHR         surface_capabilities                         = { 0 };
																	              
	VkPhysicalDevice                 physical_device                              = NULL;
	VkPhysicalDeviceProperties       physical_device_properties                   = { 0 };
	VkPhysicalDeviceFeatures         physical_device_features                     = { 0 };
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties            = { 0 };
																	              
	uint32_t                         graphics_queue_family_index                  = 0;
	uint32_t                         present_queue_family_index                   = 0;
																	              
	VkDevice                         device                                       = NULL;
	uint32_t                         device_extension_count                       = 0;
															                      
	VkQueue                          graphics_queue                               = NULL;
	VkQueue                          present_queue                                = NULL;
					           						                                  
	VkCommandPool                    graphics_cmd_pool                            = NULL;
	VkCommandPool                    present_cmd_pool                             = NULL;
															                      
	VkCommandBuffer                  graphics_cmd_buffers[SWAPCHAIN_IMAGE_COUNT]  = { NULL };
	VkCommandBuffer                  present_cmd_buffer                           = NULL;
															                      
	VkFence                          current_graphics_cmd_buffer_fence            = { NULL };
															                      
	VkSemaphore                      current_image_acquired_semaphore             = NULL;
	VkSemaphore                      current_graphics_queue_finished_semaphore    = NULL;

	VkSwapchainKHR                   swapchain                                    = NULL;
	VkFormat                         swapchain_image_format                       = 0;
	uint32_t                         swapchain_image_count                        = 0;
																	              
	uint32_t                         sample_count                                 = 0;
																	              
	VkAttachmentDescription          swapchain_attachment                         = { 0 };
	VkAttachmentReference            swapchain_attachment_reference               = { 0 };
	VkSubpassDescription             subpass                                      = { 0 };
																	              
	VkRenderPass                     renderpass                                   = NULL;

	VkImage                          swapchain_images[SWAPCHAIN_IMAGE_COUNT]      = { NULL };
	VkImageView                      swapchain_image_views[SWAPCHAIN_IMAGE_COUNT] = { NULL };

	VkFramebuffer                    framebuffers[SWAPCHAIN_IMAGE_COUNT]          = { NULL };

	shCreateInstance(
		//application_name, engine_name, enable_validation_layers,
		"vulkan app", "vulkan engine", 1,
		instance_extension_count, pp_instance_extensions,
		&instance
	);

	glfwCreateWindowSurface(
		instance, window, NULL, &surface
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
		physical_device, surface, &surface_capabilities
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
		device,//device
		present_cmd_pool,//cmd_pool
		1,//cmd_buffer_count
		&present_cmd_buffer//p_cmd_buffer
	);

	shCreateFences(
		device,//device
		1,//fence_count
		1,//signaled
		&current_graphics_cmd_buffer_fence//p_fences
	);

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
		&swapchain//p_swapchain
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
		NULL,//p_input_attachments_reference
		1,//color_attachment_count
		&swapchain_attachment_reference,//p_color_attachments_reference
		NULL,//p_depth_stencil_attachment_reference
		NULL,//p_resolve_attachment_reference
		0,//preserve_attachment_count
		NULL,//p_preserve_attachments
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

	VkBuffer       staging_buffer     = NULL;
	VkDeviceMemory staging_memory     = NULL;

	VkBuffer       vertex_buffer      = NULL;
	VkBuffer       instance_buffer    = NULL;
	VkBuffer       index_buffer       = NULL;
	VkBuffer       descriptors_buffer = NULL;

	VkDeviceMemory vertex_memory      = NULL;
	VkDeviceMemory instance_memory    = NULL;
	VkDeviceMemory index_memory       = NULL;
	VkDeviceMemory descriptors_memory = NULL;

	writeMemory(
		device,
		physical_device,
		graphics_cmd_buffers[0],//any graphics command buffer
		current_graphics_cmd_buffer_fence,
		graphics_queue,
		&staging_buffer,
		&staging_memory,
		&vertex_buffer,
		&vertex_memory,
		&instance_buffer,
		&instance_memory,
		&index_buffer,
		&index_memory,
		&descriptors_buffer,
		&descriptors_memory
	);

	ShVkPipeline pipeline = { 0 };

	uint32_t attribute_0_offset = 0;
	uint32_t attribute_0_size   = 12;
	uint32_t attribute_1_offset = attribute_0_offset + attribute_0_size;
	uint32_t attribute_1_size   = 8;

	shPipelineSetVertexBinding(
		PER_VERTEX_BINDING,
		attribute_0_size + attribute_1_size,
		VK_VERTEX_INPUT_RATE_VERTEX,
		&pipeline
	);

	shPipelineSetVertexAttribute(
		0,
		PER_VERTEX_BINDING,
		VK_FORMAT_R32G32B32_SFLOAT,
		attribute_0_offset,
		&pipeline
	);

	shPipelineSetVertexAttribute(
		1,
		PER_VERTEX_BINDING,
		VK_FORMAT_R32G32_SFLOAT,
		attribute_1_offset,
		&pipeline
	);


	uint32_t attribute_2_offset = 0;
	uint32_t attribute_2_size   = 16;
	uint32_t attribute_3_offset = attribute_2_offset + attribute_2_size;
	uint32_t attribute_3_size   = 16;
	uint32_t attribute_4_offset = attribute_3_offset + attribute_3_size;
	uint32_t attribute_4_size   = 16;
	uint32_t attribute_5_offset = attribute_4_offset + attribute_4_size;
	uint32_t attribute_5_size   = 16;

	shPipelineSetVertexBinding(
		PER_INSTANCE_BINDING,
		attribute_2_size + attribute_3_size + attribute_4_size + attribute_5_size,
		VK_VERTEX_INPUT_RATE_INSTANCE,
		&pipeline
	);
	
	shPipelineSetVertexAttribute(
		2,
		PER_INSTANCE_BINDING,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		attribute_2_offset,
		&pipeline
	);

	shPipelineSetVertexAttribute(
		3,
		PER_INSTANCE_BINDING,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		attribute_3_offset,
		&pipeline
	);

	shPipelineSetVertexAttribute(
		4,
		PER_INSTANCE_BINDING,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		attribute_4_offset,
		&pipeline
	);

	shPipelineSetVertexAttribute(
		5,
		PER_INSTANCE_BINDING,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		attribute_5_offset,
		&pipeline
	);

	shPipelineSetVertexInputState(&pipeline);

	shPipelineCreateInputAssembly(
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		0,
		&pipeline
	);

	shPipelineCreateRasterizer(
		VK_POLYGON_MODE_FILL,
		0,
		&pipeline
	);

	shPipelineSetMultisampleState(
		1,
		0.0f,
		&pipeline
	);

	shPipelineSetViewport(
		0, 0,
		width, height,
		0, 0,
		width, height,
		&pipeline
	);

	shPipelineColorBlendSettings(&pipeline);
	
	uint32_t shader_size = 0;
	char*    shader_code = readBinary(
		"../examples/shaders/bin/mesh.vert.spv",
		&shader_size
	);
	
	shPipelineCreateShaderModule(
		device,
		shader_size,
		shader_code,
		&pipeline
	);

	free(shader_code);

	shPipelineCreateShaderStage(
		VK_SHADER_STAGE_VERTEX_BIT,
		&pipeline
	);

	shader_code = readBinary(
		"../examples/shaders/bin/mesh.frag.spv",
		&shader_size
	);

	shPipelineCreateShaderModule(
		device,
		shader_size,
		shader_code,
		&pipeline
	);

	shPipelineCreateShaderStage(
		VK_SHADER_STAGE_FRAGMENT_BIT,
		&pipeline
	);

	shPipelineSetPushConstants(
		VK_SHADER_STAGE_VERTEX_BIT,
		0,
		sizeof(projection_view),
		&pipeline
	);

	shPipelineCreateDescriptorSetLayout(
		device,
		0,
		0,
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		VK_SHADER_STAGE_FRAGMENT_BIT,
		&pipeline
	);

	shPipelineCreateDescriptorPool(
		device,
		0,
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		&pipeline
	);

	shPipelineSetDescriptorBufferInfo(
		0,
		descriptors_buffer,
		0,
		sizeof(light),
		&pipeline
	);

	shPipelineAllocateDescriptorSet(
		device,
		0,
		0,
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		&pipeline
	);

	shPipelineCreateLayout(
		device,
		1,
		&pipeline
	);
	
	shSetupGraphicsPipeline(
		device,
		renderpass,
		&pipeline
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

				glfwCreateWindowSurface(instance, window, NULL, &surface);
				shGetPhysicalDeviceSurfaceCapabilities(physical_device, surface, &surface_capabilities);
				shCreateSwapchain(
					device,
					physical_device,
					surface,
					swapchain_image_format,
					&swapchain_image_format,
					SWAPCHAIN_IMAGE_COUNT,
					swapchain_image_sharing_mode,
					0,
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

				shDestroyShaderModule  (device,  pipeline.shader_modules[0]);
				shDestroyShaderModule  (device,  pipeline.shader_modules[1]);
				shDestroyDescriptorPool(device,  pipeline.descriptor_pools[0]);
				shDestroyDescriptorPool(device,  pipeline.descriptor_pools[1]);
				shDestroyPipelineLayout(device,  pipeline.pipeline_layout);
				shDestroyPipeline      (device, pipeline.pipeline);

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
			NULL,//acquired_signal_fence
			&swapchain_image_idx//p_swapchain_image_index
		);

		shResetFences(
			device,//device
			1,//fence_count
			&current_graphics_cmd_buffer_fence//p_fences
		);

		VkCommandBuffer cmd_buffer = graphics_cmd_buffers[swapchain_image_idx];

		shBeginCommandBuffer(cmd_buffer);

		triangle[6] = (float)sin(glfwGetTime());
		shWriteMemory(
			device,
			staging_memory,
			sizeof(quad),
			sizeof(triangle),
			triangle
		);
		shCopyBuffer(
			cmd_buffer,
			staging_buffer,
			sizeof(quad), sizeof(quad), sizeof(triangle),
			vertex_buffer
		);

		shBeginRenderpass(
			cmd_buffer,//graphics_cmd_buffer
			renderpass,//renderpass
			framebuffers[swapchain_image_idx],//framebuffer
			0,//render_offset_x
			0,//render_offset_y
			surface_capabilities.currentExtent.width,//render_size_x
			surface_capabilities.currentExtent.height,//render_size_y
			0.1f,//clear_color_r
			0.1f,//clear_color_g
			0.1f,//clear_color_b
			1.0f,//clear_color_a
			0,//use_clear_depth_stencil_value
			0.0f,//clear_depth
			0//clear_stencil
		);

		VkDeviceSize vertex_offset     = 0;
		VkDeviceSize vertex_offsets[2] = {0, 0};
		VkBuffer     vertex_buffers[2] = { vertex_buffer, instance_buffer };
		vkCmdBindVertexBuffers(cmd_buffer, 0, 2, vertex_buffers, vertex_offsets);
		
		shBindIndexBuffer(cmd_buffer, 0, index_buffer);

		shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);

		shPipelinePushConstants(cmd_buffer, projection_view, &pipeline);

		shPipelineUpdateDescriptorSets(device, &pipeline);

		shPipelineBindDescriptorSets(cmd_buffer, 0, 1, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, NULL, &pipeline);

		vkCmdDrawIndexed(cmd_buffer, QUAD_INDEX_COUNT, 2, 0, 0, 0);

		shDraw(cmd_buffer, 3, 4, 1, 2);

		shEndRenderpass(cmd_buffer);

		shEndCommandBuffer(cmd_buffer);

		shQueueSubmit(
			1,//cmd_buffer_count
			&cmd_buffer,//p_cmd_buffers
			graphics_queue,//queue
			current_graphics_cmd_buffer_fence,//fence
			1,//semaphores_to_wait_for_count
			&current_image_acquired_semaphore,//p_semaphores_to_wait_for
			VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,//wait_stage
			1,//signal_semaphore_count
			&current_graphics_queue_finished_semaphore//p_signal_semaphores
		);
		shWaitForFences(
			device,//device
			1,//fence_count
			&current_graphics_cmd_buffer_fence,//p_fences
			1,//wait_for_all
			UINT64_MAX//timeout_ns
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

	shDestroyShaderModule(device, pipeline.shader_modules[0]);
	shDestroyShaderModule(device, pipeline.shader_modules[1]);
	shDestroyDescriptorPool(device, pipeline.descriptor_pools[0]);
	shDestroyDescriptorSetLayout(device, pipeline.descriptor_set_layouts[0]);
	shDestroyPipelineLayout(device, pipeline.pipeline_layout);
	shDestroyPipeline(device, pipeline.pipeline);

	shDestroySemaphores(device, 1, &current_image_acquired_semaphore);

	shDestroySemaphores(device, 1, &current_graphics_queue_finished_semaphore);

	shDestroyFences(device, 1, &current_graphics_cmd_buffer_fence);

	shDestroyCommandBuffers(device, graphics_cmd_pool, swapchain_image_count, graphics_cmd_buffers);

	shDestroyCommandBuffers(device, present_cmd_pool, 1, &present_cmd_buffer);

	shDestroyCommandPool(device, graphics_cmd_pool);
	if (graphics_queue_family_index != present_queue_family_index) {
		shDestroyCommandPool(device, present_cmd_pool);
	}

	releaseMemory(
		device, 
		staging_buffer, staging_memory, 
		vertex_buffer, vertex_memory,
		instance_buffer, instance_memory,
		index_buffer, index_memory,
		descriptors_buffer, descriptors_memory
	);

	shDestroyRenderpass(device, renderpass);

	shDestroyFramebuffers(device, swapchain_image_count, framebuffers);

	shDestroyImageViews(device, swapchain_image_count, swapchain_image_views);

	shDestroySwapchain(device, swapchain);

	shDestroyDevice(device);

	shDestroySurface(instance, surface);

	shDestroyInstance(instance);

	return 0;
}

void writeMemory(
	VkDevice         device,
	VkPhysicalDevice physical_device,
	VkCommandBuffer  cmd_buffer,
	VkFence          fence,
	VkQueue          transfer_queue,
	VkBuffer*        p_staging_buffer,
	VkDeviceMemory*  p_staging_memory,
	VkBuffer*        p_vertex_buffer,
	VkDeviceMemory*  p_vertex_memory,
	VkBuffer*        p_instance_buffer,
	VkDeviceMemory*  p_instance_memory,
	VkBuffer*        p_index_buffer,
	VkDeviceMemory*  p_index_memory,
	VkBuffer*        p_descriptors_buffer,
	VkDeviceMemory*  p_descriptors_memory
) {	
	//
	//USEFUL VARIABLES
	//
	uint32_t quad_vertices_offset     = 0;
	uint32_t triangle_vertices_offset = quad_vertices_offset     + sizeof(quad);
	uint32_t instance_models_offset   = triangle_vertices_offset + sizeof(triangle);
	uint32_t quad_indices_offset      = instance_models_offset   + sizeof(models);
	uint32_t light_offset             = quad_indices_offset      + sizeof(indices);

	uint32_t staging_size = light_offset + sizeof(light);

	//
	//WRITE ALL DATA TO STAGING BUFFER
	//
	shCreateBuffer(
		device,
		staging_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		p_staging_buffer
	);
	shAllocateBufferMemory(
		device,
		physical_device,
		*p_staging_buffer,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		p_staging_memory
	);
	shWriteMemory(device, *p_staging_memory, quad_vertices_offset,     sizeof(quad),     quad);
	shWriteMemory(device, *p_staging_memory, triangle_vertices_offset, sizeof(triangle), triangle);
	shWriteMemory(device, *p_staging_memory, instance_models_offset,   sizeof(models),   models);
	shWriteMemory(device, *p_staging_memory, quad_indices_offset,      sizeof(indices),  indices);
	shWriteMemory(device, *p_staging_memory, light_offset,             sizeof(light),    light);

	shBindBufferMemory(device, *p_staging_buffer, 0, *p_staging_memory);

	//
	//SETUP DEVICE LOCAL DESTINATION BUFFERS
	//
	shCreateBuffer(device, sizeof(quad) + sizeof(triangle), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, p_vertex_buffer);
	shAllocateBufferMemory(device, physical_device, *p_vertex_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_vertex_memory);
	shBindBufferMemory(device, *p_vertex_buffer, 0, *p_vertex_memory);

	shCreateBuffer(device, sizeof(models), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, p_instance_buffer);
	shAllocateBufferMemory(device, physical_device, *p_instance_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_instance_memory);
	shBindBufferMemory(device, *p_instance_buffer, 0, *p_instance_memory);

	shCreateBuffer(device, sizeof(indices), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, p_index_buffer);
	shAllocateBufferMemory(device, physical_device, *p_index_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_index_memory);
	shBindBufferMemory(device, *p_index_buffer, 0, *p_index_memory);

	shCreateBuffer(device, sizeof(light), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, p_descriptors_buffer);
	shAllocateBufferMemory(device, physical_device, *p_descriptors_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_descriptors_memory);
	shBindBufferMemory(device, *p_descriptors_buffer, 0, *p_descriptors_memory);

	//
	//COPY STAGING BUFFER TO DEVICE LOCAL MEMORY
	//
	shResetFences(device, 1, &fence);//to signaled
	shBeginCommandBuffer(cmd_buffer);
	shCopyBuffer(cmd_buffer, *p_staging_buffer, quad_vertices_offset,   0, sizeof(quad) + sizeof(triangle), *p_vertex_buffer);
	shCopyBuffer(cmd_buffer, *p_staging_buffer, instance_models_offset, 0, sizeof(models),                  *p_instance_buffer);
	shCopyBuffer(cmd_buffer, *p_staging_buffer, quad_indices_offset,    0, sizeof(indices),                 *p_index_buffer);
	shCopyBuffer(cmd_buffer, *p_staging_buffer, light_offset,           0, sizeof(light),                   *p_descriptors_buffer);
	shEndCommandBuffer(cmd_buffer);

	shQueueSubmit(1, &cmd_buffer, transfer_queue, fence, 0, NULL, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, NULL);
	shWaitForFences(device, 1, &fence, 1, UINT64_MAX);
}

void releaseMemory(
	VkDevice       device,
	VkBuffer       staging_buffer,
	VkDeviceMemory staging_memory,
	VkBuffer       vertex_buffer,
	VkDeviceMemory vertex_memory,
	VkBuffer       instance_buffer,
	VkDeviceMemory instance_memory,
	VkBuffer       index_buffer,
	VkDeviceMemory index_memory,
	VkBuffer       descriptors_buffer,
	VkDeviceMemory descriptors_memory
) {
	shWaitDeviceIdle(device);
	shClearBufferMemory(device, staging_buffer, staging_memory);
	shClearBufferMemory(device, vertex_buffer, vertex_memory);
	shClearBufferMemory(device, instance_buffer, instance_memory);
	shClearBufferMemory(device, index_buffer, index_memory);
	shClearBufferMemory(device, descriptors_buffer, descriptors_memory);
}



#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER

char* readBinary(const char* path, uint32_t* p_size) {
	FILE* stream = fopen(path, "rb");
	if (stream == NULL) {
		return NULL;
	}
	fseek(stream, 0, SEEK_END);
	uint32_t code_size = ftell(stream);
	fseek(stream, 0, SEEK_SET);
	char* code = (char*)calloc(1, code_size);
	if (code == NULL) {
		fclose(stream);
		return NULL;
	}
	fread(code, code_size, 1, stream);
	*p_size = code_size;
	fclose(stream);
	return code;
}

#ifdef __cplusplus
}
#endif//__cplusplus