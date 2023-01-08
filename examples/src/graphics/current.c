#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkCheck.h>


#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include <math.h>



#define QUAD_VERTEX_COUNT 32
#define QUAD_INDEX_COUNT 6
#define TRIANGLE_VERTEX_COUNT 24

#define PER_VERTEX_BINDING 0
#define PER_INSTANCE_BINDING 1

float quad[QUAD_VERTEX_COUNT] = {
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
float triangle[TRIANGLE_VERTEX_COUNT] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
uint32_t indices[QUAD_INDEX_COUNT] = {
	0, 1, 2,
	2, 3, 0
};
float light[8] = {
	0.0f,  2.0f, 0.0f, 1.0f, //position
	0.0f, 0.45f, 0.9f, 1.0f	 //color
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
	VkCommandBuffer  transfer_cmd_buffer,
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
	VkBuffer*        p_descriptor_buffer,
	VkDeviceMemory*  p_descriptors_memory
);

void writeMemory(
	VkDevice         device,
	VkPhysicalDevice physical_device,
	VkCommandBuffer  transfer_cmd_buffer,
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
	VkBuffer*        p_descriptor_buffer,
	VkDeviceMemory*  p_descriptors_memory
);

int main(void) {

	int r = glfwInit();
	assert(r != 0);

	r = glfwVulkanSupported();
	assert(r != GLFW_FALSE);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	uint32_t    instance_extension_count = 0;
	GLFWwindow* window                   = glfwCreateWindow(720, 480, "vulkan sample", NULL, NULL);
	char**      pp_instance_extensions   = glfwGetRequiredInstanceExtensions(&instance_extension_count);


	VkInstance                       instance                          = NULL;
											                           
	VkSurfaceKHR                     surface                           = NULL;
	VkSurfaceCapabilitiesKHR         surface_capabilities              = { 0 };

	VkPhysicalDevice                 physical_device                   = NULL;
	VkPhysicalDeviceProperties       physical_device_properties        = { 0 };
	VkPhysicalDeviceFeatures         physical_device_features          = { 0 };
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties = { 0 };

	uint32_t                         graphics_queue_family_index       = 0;
	uint32_t                         present_queue_family_index        = 0;

	VkDevice                         device                            = NULL;
	uint32_t                         device_extension_count            = 0;
															           
	VkQueue                          graphics_queue                    = NULL;
	VkQueue                          present_queue                     = NULL;
					           						                       
	VkCommandPool                    graphics_cmd_pool                 = NULL;
	VkCommandPool                    present_cmd_pool                  = NULL;
															           
	VkCommandBuffer                  graphics_cmd_buffer               = NULL;
	VkCommandBuffer                  present_cmd_buffer                = NULL;
															           
	VkFence                          graphics_cmd_fence                = NULL;
	VkFence                          present_cmd_fence                 = NULL;
															           
	VkSemaphore                      image_acquired_semaphore          = NULL;
	VkSemaphore                      renderpass_finished_semaphore     = NULL;
					                   	        				         		          
	VkSwapchainKHR                   swapchain                         = NULL;
	VkFormat                         swapchain_image_format            = 0;
	uint32_t                         swapchain_image_count             = 0;

	uint32_t                         sample_count                      = 0;

	VkAttachmentDescription          swapchain_attachment              = { 0 };
	VkAttachmentReference            swapchain_attachment_reference    = { 0 };
	VkAttachmentDescription          depth_attachment                  = { 0 };
	VkAttachmentReference            depth_attachment_reference        = { 0 };
	VkSubpassDescription             subpass                           = { 0 };

	VkRenderPass                     renderpass                        = NULL;

	VkImage                          swapchain_images[1]               = { NULL };
	VkImageView                      swapchain_image_views[1]          = { NULL };

	VkImage                          depth_image                       = NULL;
	VkDeviceMemory                   depth_image_memory                = NULL;
	VkImageView                      depth_image_view                  = NULL;
	
	VkFramebuffer                    framebuffers[1]                   = { NULL };

	shCreateInstance(
		&instance, 
		"vulkan app", 
		"vulkan engine", 
		1, 
		instance_extension_count, 
		pp_instance_extensions
	);

	glfwCreateWindowSurface(
		instance,
		window,
		NULL,
		&surface
	);

	shSelectPhysicalDevice(
		instance,
		surface,
		VK_QUEUE_GRAPHICS_BIT | 
		VK_QUEUE_COMPUTE_BIT  | 
		VK_QUEUE_TRANSFER_BIT,
		&physical_device,
		&physical_device_properties,
		&physical_device_features,
		&physical_device_memory_properties
	);

	uint32_t graphics_queue_families_indices[SH_MAX_STACK_QUEUE_FAMILY_COUNT] = { 0 };
	uint32_t present_queue_families_indices [SH_MAX_STACK_QUEUE_FAMILY_COUNT] = { 0 };
	shGetPhysicalDeviceQueueFamilies(
		physical_device,
		surface,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		graphics_queue_families_indices,
		present_queue_families_indices,
		NULL,
		NULL,
		NULL
	);
	graphics_queue_family_index = graphics_queue_families_indices[0];
	present_queue_family_index  = present_queue_families_indices [0];

	shGetPhysicalDeviceSurfaceCapabilities(
		physical_device,
		surface,
		&surface_capabilities
	);

	float default_queue_priority = 1.0f;
	VkDeviceQueueCreateInfo graphics_device_queue_info = { 0 };
	shQueryForDeviceQueueInfo(
		graphics_queue_family_index,
		1,
		&default_queue_priority,
		0,
		&graphics_device_queue_info
	);

	VkDeviceQueueCreateInfo present_device_queue_info = { 0 };
	shQueryForDeviceQueueInfo(
		present_queue_family_index,
		1,
		&default_queue_priority,
		0,
		&present_device_queue_info
	);

	VkDeviceQueueCreateInfo device_queue_infos[2] = {
		graphics_device_queue_info,
		present_device_queue_info
	};
	char* device_extensions[2]  = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	uint32_t device_queue_count = (graphics_queue_family_index == present_queue_family_index) ? 1 : 2;
	shSetLogicalDevice(
		physical_device,
		&device,
		1,
		device_extensions,
		device_queue_count,
		device_queue_infos
	);

	shGetDeviceQueues(
		device,
		1,
		&graphics_queue_family_index,
		&graphics_queue
	);

	if (graphics_queue_family_index != present_queue_family_index) {
		shGetDeviceQueues(
			device,
			1,
			&present_queue_family_index,
			&present_queue
		);
	}
	else {
		present_queue = graphics_queue;
	}

	shCreateCmdPool(
		device,
		graphics_queue_family_index,
		&graphics_cmd_pool
	);

	shAllocateCmdBuffers(
		device,
		graphics_cmd_pool,
		1,
		&graphics_cmd_buffer
	);

	if (graphics_queue_family_index != present_queue_family_index) {
		shCreateCmdPool(
			device,
			present_queue_family_index,
			&present_cmd_pool
		);
		shAllocateCmdBuffers(
			device,
			present_cmd_pool,
			1,
			&present_cmd_buffer
		);
	}
	else {
		present_cmd_pool   = graphics_cmd_pool;
		present_cmd_buffer = graphics_cmd_buffer;
	}

	shCreateFences(
		device,
		1,
		&graphics_cmd_fence
	);

	shCreateFences(
		device,
		1,
		&present_cmd_fence
	);

	VkSharingMode swapchain_image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
	if (graphics_queue_family_index != present_queue_family_index) {
		swapchain_image_sharing_mode = VK_SHARING_MODE_CONCURRENT;
	}
	shCreateSwapchain(
		device,
		physical_device,
		surface,
		surface_capabilities,
		VK_FORMAT_R8G8B8_UNORM,
		&swapchain_image_format,
		1,
		swapchain_image_sharing_mode,
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
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,//VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
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
		VK_ATTACHMENT_STORE_OP_STORE,//or store op dont care?
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		//VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL only if separateDepthStencilLayouts feature is enabled
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		&depth_attachment
	);
	shCreateRenderpassAttachmentReference(
		//VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL if attachment is VK_ATTACHMENT_UNUSED
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

	VkImageView image_views[3] = {
		swapchain_image_views[0], depth_image_view
	};
	shSetFramebuffer(
		device,
		renderpass,
		2,
		image_views,
		surface_capabilities.currentExtent.width,
		surface_capabilities.currentExtent.height,
		1,
		framebuffers
	);

	VkBuffer       staging_buffer     = NULL;
	VkDeviceMemory staging_memory     = NULL;

	VkBuffer       vertex_buffer      = NULL;
	VkBuffer       instance_buffer    = NULL;
	VkBuffer       index_buffer       = NULL;
	VkBuffer       descriptor_buffer  = NULL;

	VkDeviceMemory vertex_memory      = NULL;
	VkDeviceMemory instance_memory    = NULL;
	VkDeviceMemory index_memory       = NULL;
	VkDeviceMemory descriptor_memory  = NULL;

	writeMemory(
		device,
		physical_device,
		graphics_cmd_buffer,
		graphics_cmd_fence,
		graphics_queue,
		&staging_buffer, 
		&staging_memory, 
		&vertex_buffer, 
		&vertex_memory, 
		&instance_buffer, 
		&instance_memory, 
		&index_buffer, 
		&index_memory, 
		&descriptor_buffer, 
		&descriptor_memory
	);


	shCreateSemaphores(
		device,
		1,
		&image_acquired_semaphore
	);

	shCreateSemaphores(
		device,
		1,
		&renderpass_finished_semaphore
	);

	uint32_t swapchain_image_idx = 0;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		shLockFences(
			device, 
			1, 
			&graphics_cmd_fence
		);
		shBeginCommandBuffer(graphics_cmd_buffer);
		
		shAcquireSwapchainImage(
			device,
			swapchain,
			UINT64_MAX,
			image_acquired_semaphore,
			NULL,
			&swapchain_image_idx
		);

		shBeginRenderpass(
			graphics_cmd_buffer,
			renderpass,
			framebuffers[swapchain_image_idx],
			0, 0,
			surface_capabilities.currentExtent.width,
			surface_capabilities.currentExtent.height,
			1.0f, 0.0f, 0.0f, 1.0f,
			1, 0.0f, 0
		);

		shEndRenderpass(graphics_cmd_buffer);

		shEndCommandBuffer(graphics_cmd_buffer);

		shQueueSubmit(
			1,
			&graphics_cmd_buffer,
			graphics_queue,
			graphics_cmd_fence,
			1,
			&image_acquired_semaphore,
			VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
			1,
			&renderpass_finished_semaphore
		);

		shWaitForFences(
			device,
			1,
			&graphics_cmd_fence,
			1,
			UINT64_MAX
		);

		shBeginCommandBuffer(present_cmd_buffer);
		
		shQueuePresentSwapchainImage(
			present_queue,
			1,//to change
			&renderpass_finished_semaphore,
			swapchain,
			swapchain_image_idx
		);

		shEndCommandBuffer(present_cmd_buffer);
	}

	return 0;
}

void writeMemory(
	VkDevice         device,
	VkPhysicalDevice physical_device,
	VkCommandBuffer  transfer_cmd_buffer,
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
	VkBuffer*        p_descriptor_buffer,
	VkDeviceMemory*  p_descriptors_memory
) {
	uint32_t quad_vertices_offset     = 0;
	uint32_t triangle_vertices_offset = quad_vertices_offset     + sizeof(quad);
	uint32_t instance_models_offset   = triangle_vertices_offset + sizeof(triangle);
	uint32_t quad_indices_offset      = instance_models_offset   + sizeof(models);
	uint32_t light_offset             = quad_indices_offset      + sizeof(indices);
	uint32_t staging_size             = light_offset             + sizeof(light);

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
	shAllocateMemory(
		device, 
		physical_device, 
		*p_staging_buffer,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		p_staging_memory
	);
	shWriteMemory(device, *p_staging_memory, quad_vertices_offset,     sizeof(quad),     quad    );
	shWriteMemory(device, *p_staging_memory, triangle_vertices_offset, sizeof(triangle), triangle);
	shWriteMemory(device, *p_staging_memory, quad_indices_offset,      sizeof(indices),  indices );
	shWriteMemory(device, *p_staging_memory, instance_models_offset,   sizeof(models),   models  );
	shWriteMemory(device, *p_staging_memory, instance_models_offset,   sizeof(models),   models  );
	shWriteMemory(device, *p_staging_memory, light_offset,             sizeof(light),    light   );

	shBindMemory(device, *p_staging_buffer, 0, *p_staging_memory);


	//
	//SETUP DEVICE LOCAL DESTINATION BUFFERS
	//
	shCreateBuffer(device, sizeof(quad) + sizeof(triangle), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, p_vertex_buffer);
	shAllocateMemory(device, physical_device, *p_vertex_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_vertex_memory);
	shBindMemory(device, *p_vertex_buffer, 0, *p_vertex_memory);

	shCreateBuffer(device, sizeof(models), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, p_instance_buffer);
	shAllocateMemory(device, physical_device, *p_instance_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_instance_memory);
	shBindMemory(device, *p_instance_buffer, 0, *p_instance_memory);

	shCreateBuffer(device, sizeof(indices), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, p_index_buffer);
	shAllocateMemory(device, physical_device, *p_index_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_index_memory);
	shBindMemory(device, *p_index_buffer, 0, *p_index_memory);

	shCreateBuffer(device, sizeof(light), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE, p_descriptor_buffer);
	shAllocateMemory(device, physical_device, *p_descriptor_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_descriptors_memory);
	shBindMemory(device, *p_descriptor_buffer, 0, *p_descriptors_memory);

	//
	//COPY STAGING BUFFER TO DEVICE LOCAL MEMORY
	//
	shLockFences(device, 1, &fence);
	shBeginCommandBuffer(transfer_cmd_buffer);
	shCopyBuffer(transfer_cmd_buffer, *p_staging_buffer, quad_vertices_offset,   0, sizeof(quad) + sizeof(triangle), *p_vertex_buffer);
	shCopyBuffer(transfer_cmd_buffer, *p_staging_buffer, instance_models_offset, 0, sizeof(models),                  *p_instance_buffer);
	shCopyBuffer(transfer_cmd_buffer, *p_staging_buffer, quad_indices_offset,    0, sizeof(indices),                 *p_index_buffer);
	shCopyBuffer(transfer_cmd_buffer, *p_staging_buffer, light_offset,           0, sizeof(light),                   *p_descriptor_buffer);
	shEndCommandBuffer(transfer_cmd_buffer);
	shQueueSubmit(1, &transfer_cmd_buffer, transfer_queue, fence, 0, NULL, 0, 0, NULL);
	shWaitForFences(device, 1, &fence, 1, UINT64_MAX);
}



#ifdef __cplusplus
}
#endif//__cplusplus