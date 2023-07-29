#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVulkan.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>

#define SWAPCHAIN_IMAGE_COUNT          3
#define MAX_SWAPCHAIN_IMAGE_COUNT      6
#define RENDERPASS_ATTACHMENT_COUNT    3
#define SUBPASS_COLOR_ATTACHMENT_COUNT 1 

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
	0.0f, 1.3f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
   -0.4f,-0.2f, 0.3f, 1.0f,

	0.2f, 0.0f, 0.0f, 0.0f,//model 1
	0.0f, 1.3f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.4f,-0.2f, 0.2f, 1.0f,

	0.7f, 0.0f, 0.0f, 0.0f,//model 2
	0.0f, 0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.3f, 0.1f, 1.0f
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

#define DESCRIPTOR_SET_COUNT        1
#define INFO_DESCRIPTOR_SET_IDX     0
#define OPTIONAL_DESCRIPTOR_SET_IDX 1

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

void createPipelinesDataPool(
	VkDevice          device,
	VkBuffer          descriptors_buffer,
	uint32_t          swapchain_image_count,
	ShVkPipelinePool* p_pipeline_pool
);

void createPipeline(
	VkDevice          device,
	VkRenderPass      renderpass,
	uint32_t          width,
	uint32_t          height,
	uint32_t          sample_count,
	uint32_t          swapchain_image_count,
	ShVkPipelinePool* p_pipeline_pool
);

void resizeWindow(
	uint32_t                  width,
	uint32_t                  height,
	VkInstance                instance,
	GLFWwindow*               window,
	VkSurfaceKHR*             p_surface,
	VkSurfaceCapabilitiesKHR* p_surface_capabilities,
	VkPhysicalDevice          physical_device,
	VkDevice                  device,
	uint32_t                  graphics_queue_family_index,
	uint32_t                  sample_count,
	VkSwapchainKHR*           p_swapchain,
	VkFormat*                 p_swapchain_image_format,
	VkSharingMode             swapchain_image_sharing_mode,
	uint32_t*                 p_swapchain_image_count,
	VkImageView*              p_swapchain_image_views,
	VkImage*                  p_swapchain_images,
	VkImage*                  input_color_image,
	VkDeviceMemory*           input_color_image_memory,
	VkImageView*              p_input_color_image_view,
	VkImage*                  p_depth_image,
	VkDeviceMemory*           p_depth_image_memory,
	VkImageView*              p_depth_image_view,
	VkRenderPass*             p_renderpass,
	VkAttachmentDescription*  p_attachment_descriptions,
	VkSubpassDescription*     p_subpass,
	VkFramebuffer*            p_framebuffers
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

	GLFWmonitor* monitor    = (GLFWmonitor*)glfwGetPrimaryMonitor();
	GLFWvidmode* video_mode = (GLFWvidmode*)glfwGetVideoMode(monitor); 	


	int width  = (int)((float)video_mode->width  / 1.5f);
	int height = (int)((float)video_mode->height / 1.5f);

	uint32_t     instance_extension_count = 0;
	GLFWwindow*  window                   = glfwCreateWindow(width, height, "vulkan scene", NULL, NULL);
	const char** pp_instance_extensions   = glfwGetRequiredInstanceExtensions(&instance_extension_count);

#ifdef _WIN32
	glfwSetWindowSizeLimits(window, 400, 300, GLFW_DONT_CARE, GLFW_DONT_CARE);
#else
	glfwSetWindowSizeLimits(window, width, height, width, height);//X11 is so problematic
#endif//_WIN32

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
																	              
	VkAttachmentDescription          input_color_attachment                           = { 0 };
	VkAttachmentReference            input_color_attachment_reference                 = { 0 };
	VkAttachmentDescription          depth_attachment                                 = { 0 };
	VkAttachmentReference            depth_attachment_reference                       = { 0 };
	VkAttachmentDescription          resolve_attachment                               = { 0 };
	VkAttachmentReference            resolve_attachment_reference                     = { 0 };
	VkSubpassDescription             subpass                                          = { 0 };
																	                  
	VkRenderPass                     renderpass                                       = VK_NULL_HANDLE;

	VkImage                          swapchain_images[MAX_SWAPCHAIN_IMAGE_COUNT]      = { VK_NULL_HANDLE };
	VkImageView                      swapchain_image_views[MAX_SWAPCHAIN_IMAGE_COUNT] = { VK_NULL_HANDLE };
	VkImage                          depth_image = VK_NULL_HANDLE;
	VkDeviceMemory                   depth_image_memory                               = VK_NULL_HANDLE;
	VkImageView                      depth_image_view                                 = VK_NULL_HANDLE;
	VkImage                          input_color_image                                = VK_NULL_HANDLE;
	VkDeviceMemory                   input_color_image_memory                         = VK_NULL_HANDLE;
	VkImageView                      input_color_image_view                           = VK_NULL_HANDLE;
    
	VkFramebuffer                    framebuffers[MAX_SWAPCHAIN_IMAGE_COUNT]          = { VK_NULL_HANDLE };

	shCreateInstance(
		//application_name, engine_name, enable_validation_layers,
		"vulkan app", "vulkan engine", 1,
		instance_extension_count, pp_instance_extensions,
		VK_MAKE_API_VERSION(1, 3, 0, 0),//api_version,
		&instance
	);

	glfwCreateWindowSurface(
		instance, window, VK_NULL_HANDLE, &surface
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

	shGetDeviceQueues(
		device,//device
		1,//queue_count
		&present_queue_family_index,//p_queue_family_indices
		&present_queue//p_queues
	);

	VkSharingMode swapchain_image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
	if (graphics_queue_family_index != present_queue_family_index) {
		swapchain_image_sharing_mode = VK_SHARING_MODE_CONCURRENT;
	}
	shCreateSwapchain(
		device,//device
		physical_device,//physical_device
		surface,//surface
		VK_FORMAT_R32G32B32A32_SFLOAT,//image_format
		&swapchain_image_format,//p_image_format
		SWAPCHAIN_IMAGE_COUNT,//swapchain_image_count
		swapchain_image_sharing_mode,//image_sharing_mode
		SH_FALSE,//vsync
		&swapchain_image_count,
		&swapchain//p_swapchain
	);//need p_swapchain_image_count

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
		physical_device_properties, 64,
		1, 1, &sample_count
	);

	shCreateRenderpassAttachment(
		swapchain_image_format,//format
		sample_count,//sample_count
		VK_ATTACHMENT_LOAD_OP_CLEAR,//load_treatment
		VK_ATTACHMENT_STORE_OP_STORE,//store_treatment
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,//stencil_load_treatment
		VK_ATTACHMENT_STORE_OP_DONT_CARE,//stencil_store_treatment
		VK_IMAGE_LAYOUT_UNDEFINED,//initial_layout
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,//final_layout
		&input_color_attachment//p_attachment_description
	);
	shCreateRenderpassAttachmentReference(
		0,//attachment_idx
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,//layout
		&input_color_attachment_reference//p_attachment_reference
	);

	shCreateRenderpassAttachment(
		VK_FORMAT_D32_SFLOAT,
		sample_count,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		&depth_attachment
	);
	shCreateRenderpassAttachmentReference(
		1,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		&depth_attachment_reference
	);
	
	shCreateRenderpassAttachment(
		swapchain_image_format,
		1,//sample count
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		&resolve_attachment
	);
	shCreateRenderpassAttachmentReference(
		2,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		&resolve_attachment_reference
	);

	shCreateSubpass(
		VK_PIPELINE_BIND_POINT_GRAPHICS,//bind_point
		0,//input_attachment_count
		VK_NULL_HANDLE,//p_input_attachments_reference
		SUBPASS_COLOR_ATTACHMENT_COUNT,//color_attachment_count
		&input_color_attachment_reference,//p_color_attachments_reference
		&depth_attachment_reference,//p_depth_stencil_attachment_reference
		&resolve_attachment_reference,//p_resolve_attachment_reference
		0,//preserve_attachment_count
		VK_NULL_HANDLE,//p_preserve_attachments
		&subpass//p_subpass
	);

	VkAttachmentDescription attachment_descriptions[RENDERPASS_ATTACHMENT_COUNT] = {
		input_color_attachment, depth_attachment, resolve_attachment
	};
	shCreateRenderpass(
		device,//device
		RENDERPASS_ATTACHMENT_COUNT,//attachment_count
		attachment_descriptions,//p_attachments_descriptions
		1,//subpass_count
		&subpass,//p_subpasses
		&renderpass//p_renderpass
	);
	
	shCreateImage(
		device,
		VK_IMAGE_TYPE_2D,
		width, height, 1,
		VK_FORMAT_D32_SFLOAT,
		1, sample_count,
	VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		&depth_image
	);
	shAllocateImageMemory(
		device, physical_device, depth_image,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&depth_image_memory
	);
	shBindImageMemory(
		device, depth_image, 0, depth_image_memory
	);
	shCreateImageView(
		device, depth_image, VK_IMAGE_VIEW_TYPE_2D,
		VK_IMAGE_ASPECT_DEPTH_BIT, 1,
		VK_FORMAT_D32_SFLOAT, &depth_image_view
	);

	shCreateImage(
		device, VK_IMAGE_TYPE_2D, width, height, 1,
		swapchain_image_format, 1, sample_count,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE,
		&input_color_image
	);
	shAllocateImageMemory(
		device, physical_device, input_color_image,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &input_color_image_memory
	);
	shBindImageMemory(
		device, input_color_image, 0, input_color_image_memory
	);
	shCreateImageView(
		device, input_color_image, VK_IMAGE_VIEW_TYPE_2D,
		VK_IMAGE_ASPECT_COLOR_BIT, 1, swapchain_image_format,
		&input_color_image_view
	);

	for (uint32_t i = 0; i < swapchain_image_count; i++) {
		VkImageView image_views[RENDERPASS_ATTACHMENT_COUNT] = {
			input_color_image_view, depth_image_view, swapchain_image_views[i]
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

	VkBuffer       staging_buffer     = VK_NULL_HANDLE;
	VkDeviceMemory staging_memory     = VK_NULL_HANDLE;

	VkBuffer       vertex_buffer      = VK_NULL_HANDLE;
	VkBuffer       instance_buffer    = VK_NULL_HANDLE;
	VkBuffer       index_buffer       = VK_NULL_HANDLE;
	VkBuffer       descriptors_buffer = VK_NULL_HANDLE;

	VkDeviceMemory vertex_memory      = VK_NULL_HANDLE;
	VkDeviceMemory instance_memory    = VK_NULL_HANDLE;
	VkDeviceMemory index_memory       = VK_NULL_HANDLE;
	VkDeviceMemory descriptors_memory = VK_NULL_HANDLE;

	writeMemory(
		device,
		physical_device,
		graphics_cmd_buffers[0],//any graphics command buffer
		graphics_cmd_fences[0],
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

	ShVkPipelinePool* p_pipeline_pool =  shAllocatePipelinePool();

	shVkError(
		p_pipeline_pool == VK_NULL_HANDLE,
		"invalid pipeline pool memory",
		return -1
	);

	ShVkPipeline* p_pipeline = &p_pipeline_pool->pipelines[0];

	createPipelinesDataPool(
		device,
		descriptors_buffer,
		swapchain_image_count,
		p_pipeline_pool
	);

	createPipeline(
		device, renderpass, 
		width, height, sample_count,
		swapchain_image_count,
		p_pipeline_pool
	);

	uint32_t swapchain_image_idx  = 0;
	uint8_t  swapchain_suboptimal = 0;


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		int _width = 0;
		int _height = 0;
		glfwGetWindowSize(window, &_width, &_height);

		if (_width != 0 && _height != 0) {//otherwise it's minimized
			if (_width != width || _height != height) {//window is resized

				width  = _width;
				height = _height;

				resizeWindow(
					width, height, instance, window, &surface, &surface_capabilities, physical_device,
					device, graphics_queue_family_index, sample_count, &swapchain, &swapchain_image_format,
					swapchain_image_sharing_mode, &swapchain_image_count, swapchain_image_views, swapchain_images,
					&input_color_image, &input_color_image_memory, &input_color_image_view, &depth_image, &depth_image_memory,
					&depth_image_view, &renderpass, attachment_descriptions, &subpass, framebuffers
				);

				shDestroyPipeline (device, p_pipeline->pipeline);
				shPipelineSetViewport(0, 0,width, height, 0, 0,width, height, p_pipeline);
				shSetupGraphicsPipeline(device, renderpass, p_pipeline);
			}

			shAcquireSwapchainImage(
				device,//device
				swapchain,//swapchain
				UINT64_MAX,//timeout_ns
				current_image_acquired_semaphore,//acquired_signal_semaphore
				VK_NULL_HANDLE,//acquired_signal_fence
				&swapchain_image_idx,//p_swapchain_image_index
				&swapchain_suboptimal//p_swapchain_suboptimal
			);

			if (swapchain_suboptimal) {
				resizeWindow(
					width, height, instance, window, &surface, &surface_capabilities, physical_device,
					device, graphics_queue_family_index, sample_count, &swapchain, &swapchain_image_format,
					swapchain_image_sharing_mode, &swapchain_image_count, swapchain_image_views, swapchain_images,
					&input_color_image, &input_color_image_memory, &input_color_image_view, &depth_image, &depth_image_memory,
					&depth_image_view, &renderpass, attachment_descriptions, &subpass, framebuffers
				);
				swapchain_suboptimal = 0;
			}

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

			VkCommandBuffer cmd_buffer = graphics_cmd_buffers[swapchain_image_idx];

			shBeginCommandBuffer(cmd_buffer);
			
			triangle[6] = (float)sin(glfwGetTime());;
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

			VkClearValue clear_values[2] = { 0 };
			float* p_colors = clear_values[0].color.float32;
			p_colors[0] = 0.1f;
			p_colors[1] = 0.1f;
			p_colors[2] = 0.1f;

			clear_values[1].depthStencil.depth = 1.0f;

			shBeginRenderpass(
				cmd_buffer,//graphics_cmd_buffer
				renderpass,//renderpass
				0,//render_offset_x
				0,//render_offset_y
				surface_capabilities.currentExtent.width,//render_size_x
				surface_capabilities.currentExtent.height,//render_size_y
				2,//only attachments with VK_ATTACHMENT_LOAD_OP_CLEAR
				clear_values,//p_clear_values
				framebuffers[swapchain_image_idx]//framebuffer
			);

			VkDeviceSize vertex_offset = 0;
			VkDeviceSize vertex_offsets[2] = { 0, 0 };
			VkBuffer     vertex_buffers[2] = { vertex_buffer, instance_buffer };
			shBindVertexBuffers(cmd_buffer, 0, 2, vertex_buffers, vertex_offsets);

			shBindIndexBuffer(cmd_buffer, 0, index_buffer);

			shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);

			shPipelinePushConstants(cmd_buffer, projection_view, p_pipeline);

			shPipelineBindDescriptorSetUnits(
				cmd_buffer,                                 //cmd_buffer
				INFO_DESCRIPTOR_SET_IDX,                    //first_descriptor_set
				DESCRIPTOR_SET_COUNT * swapchain_image_idx, //first_descriptor_set_unit_idx
				DESCRIPTOR_SET_COUNT,                       //descriptor_set_unit_count
				VK_PIPELINE_BIND_POINT_GRAPHICS,            //bind_point
				0,                                          //dynamic_descriptors_count
				VK_NULL_HANDLE,                             //p_dynamic_offsets
				p_pipeline_pool,                            //p_pipeline_pool
				p_pipeline                                  //p_pipeline
			);

			shDrawIndexed(cmd_buffer, QUAD_INDEX_COUNT, 2, 0, 0, 0);

			shDraw(cmd_buffer, 3, 4, 1, 2);

			shEndRenderpass(cmd_buffer);

			shEndCommandBuffer(cmd_buffer);

			shQueueSubmit(
				1,//cmd_buffer_count
				&cmd_buffer,//p_cmd_buffers
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

			swapchain_image_idx = (swapchain_image_idx + 1) % swapchain_image_count;
		}
	}

	shWaitDeviceIdle(device);
	
	shPipelinePoolDestroyDescriptorPools(device, 0, 1, p_pipeline_pool);
	shPipelinePoolDestroyDescriptorSetLayouts(device, 0, 1, p_pipeline_pool);

	shPipelineDestroyShaderModules(device, 0, 2, p_pipeline);
	shPipelineDestroyLayout(device, p_pipeline);
	shDestroyPipeline(device, p_pipeline->pipeline);

	shClearPipeline(p_pipeline);

	shFreePipelinePool(p_pipeline_pool);


	shDestroySemaphores(device, 1, &current_image_acquired_semaphore);

	shDestroySemaphores(device, 1, &current_graphics_queue_finished_semaphore);

	shDestroyFences(device, swapchain_image_count, graphics_cmd_fences);

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

	shClearImageMemory(device, depth_image, depth_image_memory);
	shClearImageMemory(device, input_color_image, input_color_image_memory);
	shDestroyImageViews(device, 1, &depth_image_view);
	shDestroyImageViews(device, 1, &input_color_image_view);

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

	shQueueSubmit(1, &cmd_buffer, transfer_queue, fence, 0, VK_NULL_HANDLE, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, VK_NULL_HANDLE);
	shWaitForFences(device, 1, &fence, 1, UINT64_MAX);

	return;
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
	return;
}

void createPipelinesDataPool(
	VkDevice          device,
	VkBuffer          descriptors_buffer,
	uint32_t          swapchain_image_count,
	ShVkPipelinePool* p_pipeline_pool
) {
	//SAME BINDING FOR ALL
	//
	//
	shPipelinePoolCreateDescriptorSetLayoutBinding(
		0,                                 //binding
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, //descriptor_type
		1,                                 //descriptor_set_count
		VK_SHADER_STAGE_FRAGMENT_BIT,      //shader_stage
		p_pipeline_pool                    //p_pipeline_pool
	);
	


	for (uint32_t i = 0; i < DESCRIPTOR_SET_COUNT * swapchain_image_count; i += DESCRIPTOR_SET_COUNT) {
		//INFO
		//
		//
		shPipelinePoolSetDescriptorBufferInfos(
			i + INFO_DESCRIPTOR_SET_IDX, //first_descriptor
			1,                           //descriptor_count
			descriptors_buffer,          //buffer
			0,                           //buffer_offset
			sizeof(light),               //buffer_size
			p_pipeline_pool              //p_pipeline_pool
		);

#if DESCRIPTOR_SET_COUNT == 2
		//OPTIONAL
		//
		//
		shPipelinePoolSetDescriptorBufferInfos(
			i + OPTIONAL_DESCRIPTOR_SET_IDX, //first_descriptor
			1,                               //descriptor_count
			descriptors_buffer,              //buffer
			0,                               //buffer_offset
			sizeof(light) / 2,               //buffer_size
			p_pipeline_pool                  //p_pipeline_pool
		);
#endif
	}

	//SAME DESCRIPTOR SET LAYOUT FOR ALL
	//
	//
	shPipelinePoolCreateDescriptorSetLayout(
		device,         //device
		0,              //first_binding_idx
		1,              //binding_count
		0,              //set_layout_idx
		p_pipeline_pool //p_pipeline_pool
	);
	
	shPipelinePoolCopyDescriptorSetLayout(
		0,                                            //src_set_layout_idx
		0,                                            //first_dst_set_layout_idx
		DESCRIPTOR_SET_COUNT * swapchain_image_count, //dst_set_layout_count
		p_pipeline_pool                               //p_pipeline_pool
	);

	//SAME DESCRIPTOR POOL FOR ALL
	//
	//
	shPipelinePoolCreateDescriptorPool(
		device,                                       //device
		0,                                            //pool_idx
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,            //descriptor_type
		DESCRIPTOR_SET_COUNT * swapchain_image_count, //decriptor_count
		p_pipeline_pool                               //p_pipeline_pool
	);

	shPipelinePoolAllocateDescriptorSetUnits(
		device,                                       //device,
		0,                                            //binding,
		0,                                            //pool_idx,
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,            //descriptor_type,
		0,                                            //first_descriptor_set_unit,
		DESCRIPTOR_SET_COUNT * swapchain_image_count, //descriptor_set_unit_count,
		p_pipeline_pool                               //p_pipeline
	);

	//UPDATE ALL
	//
	//
	shPipelinePoolUpdateDescriptorSetUnits(
		device, 0, DESCRIPTOR_SET_COUNT * swapchain_image_count, p_pipeline_pool
	);

	return;
}

void createPipeline(
	VkDevice          device,
	VkRenderPass      renderpass,
	uint32_t          width,
	uint32_t          height,
	uint32_t          sample_count,
	uint32_t          swapchain_image_count,
	ShVkPipelinePool* p_pipeline_pool
) {
	ShVkPipeline* p_pipeline = &p_pipeline_pool->pipelines[0];

	uint32_t attribute_0_offset = 0;
	uint32_t attribute_0_size   = 12;
	uint32_t attribute_1_offset = attribute_0_offset + attribute_0_size;
	uint32_t attribute_1_size   = 8;

	shPipelineSetVertexBinding(
		PER_VERTEX_BINDING,
		attribute_0_size + attribute_1_size,
		VK_VERTEX_INPUT_RATE_VERTEX,
		p_pipeline
	);

	shPipelineSetVertexAttribute(
		0,
		PER_VERTEX_BINDING,
		VK_FORMAT_R32G32B32_SFLOAT,
		attribute_0_offset,
		p_pipeline
	);

	shPipelineSetVertexAttribute(
		1,
		PER_VERTEX_BINDING,
		VK_FORMAT_R32G32_SFLOAT,
		attribute_1_offset,
		p_pipeline
	);



	shPipelineSetVertexBinding(
		PER_INSTANCE_BINDING,
		64,
		VK_VERTEX_INPUT_RATE_INSTANCE,
		p_pipeline
	);

	shPipelineSetVertexAttribute(
		2,
		PER_INSTANCE_BINDING,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		0,
		p_pipeline
	);

	shPipelineSetVertexAttribute(
		3,
		PER_INSTANCE_BINDING,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		16,
		p_pipeline
	);

	shPipelineSetVertexAttribute(
		4,
		PER_INSTANCE_BINDING,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		32,
		p_pipeline
	);

	shPipelineSetVertexAttribute(
		5,
		PER_INSTANCE_BINDING,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		48,
		p_pipeline
	);

	shPipelineSetVertexInputState(p_pipeline);

	shPipelineCreateInputAssembly(
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		SH_FALSE,
		p_pipeline
	);

	shPipelineCreateRasterizer(
		VK_POLYGON_MODE_FILL,
		SH_FALSE,
		p_pipeline
	);

	shPipelineSetMultisampleState(
		sample_count,
		0.0f,
		p_pipeline
	);

	shPipelineSetViewport(
		0, 0,
		width, height,
		0, 0,
		width, height,
		p_pipeline
	);

	shPipelineColorBlendSettings(SH_FALSE, SH_TRUE, SUBPASS_COLOR_ATTACHMENT_COUNT, p_pipeline);

	uint32_t shader_size = 0;
	char* shader_code = readBinary(
		"../examples/shaders/bin/mesh.vert.spv",
		&shader_size
	);

	shPipelineCreateShaderModule(
		device,
		shader_size,
		shader_code,
		p_pipeline
	);

	free(shader_code);

	shPipelineCreateShaderStage(
		VK_SHADER_STAGE_VERTEX_BIT,
		p_pipeline
	);

	shader_code = readBinary(
		"../examples/shaders/bin/mesh.frag.spv",
		&shader_size
	);

	shPipelineCreateShaderModule(
		device,
		shader_size,
		shader_code,
		p_pipeline
	);

	shPipelineCreateShaderStage(
		VK_SHADER_STAGE_FRAGMENT_BIT,
		p_pipeline
	);

	shPipelineSetPushConstants(
		VK_SHADER_STAGE_VERTEX_BIT,
		0,
		sizeof(projection_view),
		p_pipeline
	);

	shPipelineCreateLayout(
		device,
		0,
		DESCRIPTOR_SET_COUNT * swapchain_image_count,
		p_pipeline_pool,
		p_pipeline
	);

	shSetupGraphicsPipeline(
		device,
		renderpass,
		p_pipeline
	);

	return;
}

void resizeWindow(
	uint32_t                  width,
	uint32_t                  height,
	VkInstance                instance,
	GLFWwindow*               window,
	VkSurfaceKHR*             p_surface,
	VkSurfaceCapabilitiesKHR* p_surface_capabilities,
	VkPhysicalDevice          physical_device,
	VkDevice                  device,
	uint32_t                  graphics_queue_family_index,
	uint32_t                  sample_count,
	VkSwapchainKHR*           p_swapchain,
	VkFormat*                 p_swapchain_image_format,
	VkSharingMode             swapchain_image_sharing_mode,
	uint32_t*                 p_swapchain_image_count,
	VkImageView*              p_swapchain_image_views,
	VkImage*                  p_swapchain_images,
	VkImage*                  p_input_color_image,
	VkDeviceMemory*           p_input_color_image_memory,
	VkImageView*              p_input_color_image_view,
	VkImage*                  p_depth_image,
	VkDeviceMemory*           p_depth_image_memory,
	VkImageView*              p_depth_image_view,
	VkRenderPass*             p_renderpass,
	VkAttachmentDescription*  p_attachment_descriptions,
	VkSubpassDescription*     p_subpass,
	VkFramebuffer*            p_framebuffers
) {
	shWaitDeviceIdle(device);

	shDestroyRenderpass(device, *p_renderpass);
	shDestroyFramebuffers(device, *p_swapchain_image_count, p_framebuffers);
	shDestroyImageViews(device, *p_swapchain_image_count, p_swapchain_image_views);
	shDestroySwapchain(device, *p_swapchain);
	shDestroySurface(instance, *p_surface);

	shClearImageMemory(device, *p_depth_image, *p_depth_image_memory);
	shClearImageMemory(device, *p_input_color_image, *p_input_color_image_memory);
	shDestroyImageViews(device, 1, p_depth_image_view);
	shDestroyImageViews(device, 1, p_input_color_image_view);

	glfwCreateWindowSurface(instance, window, VK_NULL_HANDLE, p_surface);
	shGetPhysicalDeviceSurfaceSupport(physical_device, graphics_queue_family_index, *p_surface, NULL);
	shGetPhysicalDeviceSurfaceCapabilities(physical_device, *p_surface, p_surface_capabilities);
	shCreateSwapchain(
		device, physical_device, *p_surface,
		*p_swapchain_image_format,
		p_swapchain_image_format,
		SWAPCHAIN_IMAGE_COUNT,
		swapchain_image_sharing_mode,
		SH_FALSE,
		p_swapchain_image_count,
		p_swapchain
	);
	shGetSwapchainImages(device, *p_swapchain, p_swapchain_image_count, p_swapchain_images);
	for (uint32_t i = 0; i < (*p_swapchain_image_count); i++) {
		shCreateImageView(
			device, p_swapchain_images[i],
			VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT,
			1, *p_swapchain_image_format,
			&p_swapchain_image_views[i]
		);
	}

	shCreateImage(
		device, VK_IMAGE_TYPE_2D,
		width, height, 1,
		VK_FORMAT_D32_SFLOAT,
		1, sample_count,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE, p_depth_image
	);
	shAllocateImageMemory(
		device, physical_device, *p_depth_image,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		p_depth_image_memory
	);
	shBindImageMemory(
		device, *p_depth_image, 0, *p_depth_image_memory
	);
	shCreateImageView(
		device, *p_depth_image, VK_IMAGE_VIEW_TYPE_2D,
		VK_IMAGE_ASPECT_DEPTH_BIT, 1,
		VK_FORMAT_D32_SFLOAT, p_depth_image_view
	);

	shCreateImage(
		device, VK_IMAGE_TYPE_2D,
		width, height, 1,
		*p_swapchain_image_format,
		1, sample_count,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		p_input_color_image
	);
	shAllocateImageMemory(
		device, physical_device, *p_input_color_image,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		p_input_color_image_memory
	);
	shBindImageMemory(
		device, *p_input_color_image, 0, *p_input_color_image_memory
	);
	shCreateImageView(
		device, *p_input_color_image, VK_IMAGE_VIEW_TYPE_2D,
		VK_IMAGE_ASPECT_COLOR_BIT, 1, *p_swapchain_image_format,
		p_input_color_image_view
	);

	shCreateRenderpass(device, RENDERPASS_ATTACHMENT_COUNT, p_attachment_descriptions, 1, p_subpass, p_renderpass);
	for (uint32_t i = 0; i < (*p_swapchain_image_count); i++) {
		VkImageView image_views[RENDERPASS_ATTACHMENT_COUNT] = {
			*p_input_color_image_view, *p_depth_image_view, p_swapchain_image_views[i]
		};
		shCreateFramebuffer(device, *p_renderpass, RENDERPASS_ATTACHMENT_COUNT, image_views, width, height, 1, &p_framebuffers[i]);
	}

	return;
}

#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER

char* readBinary(const char* path, uint32_t* p_size) {
	FILE* stream = fopen(path, "rb");
	if (stream == VK_NULL_HANDLE) {
		return VK_NULL_HANDLE;
	}
	fseek(stream, 0, SEEK_END);
	uint32_t code_size = ftell(stream);
	fseek(stream, 0, SEEK_SET);
	char* code = (char*)calloc(1, code_size);
	if (code == VK_NULL_HANDLE) {
		fclose(stream);
		return VK_NULL_HANDLE;
	}
	fread(code, code_size, 1, stream);
	*p_size = code_size;
	fclose(stream);
	return code;
}

#ifdef __cplusplus
}
#endif//__cplusplus
