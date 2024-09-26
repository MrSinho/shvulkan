#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shvulkan/shVulkan.h>

#include <vvo/vvo.h>

#include <stdio.h>
#include <math.h>

#define SRC_IMAGE_COUNT                1 //headless
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

char* readBinary(
	const char* path, 
	uint32_t* p_size
);

int main(void) {

	int width  = 800;
	int height = 600;

	VkInstance                       instance                                         = VK_NULL_HANDLE;
											                                        
	VkPhysicalDevice                 physical_device                                  = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties       physical_device_properties                       = { 0 };
	VkPhysicalDeviceFeatures         physical_device_features                         = { 0 };
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties                = { 0 };
																	                
	uint32_t                         graphics_queue_family_index                      = 0;
																	                
	VkDevice                         device                                           = VK_NULL_HANDLE;
	uint32_t                         device_extension_count                           = 0;
															                        
	VkQueue                          graphics_queue                                   = VK_NULL_HANDLE;
					           						                                    
	VkCommandPool                    graphics_cmd_pool                                = VK_NULL_HANDLE;
															                      
	VkCommandBuffer                  graphics_cmd_buffers[SRC_IMAGE_COUNT]            = { VK_NULL_HANDLE };
															                      
	VkFence                          graphics_cmd_fences[SRC_IMAGE_COUNT]             = { VK_NULL_HANDLE };
															                      
	VkSemaphore                      current_graphics_queue_finished_semaphore        = VK_NULL_HANDLE;
    
	uint32_t                         sample_count                                     = 0;
																	              
	VkAttachmentDescription          input_color_attachment                           = { 0 };
	VkAttachmentReference            input_color_attachment_reference                 = { 0 };
	VkAttachmentDescription          depth_attachment                                 = { 0 };
	VkAttachmentReference            depth_attachment_reference                       = { 0 };
	VkAttachmentDescription          resolve_attachment                               = { 0 };
	VkAttachmentReference            resolve_attachment_reference                     = { 0 };
	VkSubpassDescription             subpass                                          = { 0 };
																	                  
	VkRenderPass                     renderpass                                       = VK_NULL_HANDLE;

	VkImage                          depth_image = VK_NULL_HANDLE;
	VkDeviceMemory                   depth_image_memory                               = VK_NULL_HANDLE;
	VkImageView                      depth_image_view                                 = VK_NULL_HANDLE;
	VkImage                          input_color_image                                = VK_NULL_HANDLE;
	VkDeviceMemory                   input_color_image_memory                         = VK_NULL_HANDLE;
	VkImageView                      input_color_image_view                           = VK_NULL_HANDLE;
    
	VkFramebuffer                    framebuffers[SRC_IMAGE_COUNT]                    = { VK_NULL_HANDLE };

	VvoHandle vvo = { 0 };//For headless applications

	shCreateInstance(
		"vulkan app", //application_name
		"vulkan engine",//engine_name
		1,//enable_validation_layers
		0, //extension_count
		NULL,//pp_extension_names
		VK_MAKE_API_VERSION(1, 3, 0, 0),//api_version
		&instance//p_instance
	);

	shSelectPhysicalDevice(
		instance,//instance,
		NULL,//surface,
		VK_QUEUE_GRAPHICS_BIT |
		VK_QUEUE_TRANSFER_BIT,//requirements,
		&physical_device,//p_physical_device,
		&physical_device_properties,//p_physical_device_properties,
		&physical_device_features,//p_physical_device_features,
		&physical_device_memory_properties//p_physical_device_memory_properties
	);

	uint32_t graphics_queue_families_indices[SH_MAX_STACK_QUEUE_FAMILY_COUNT] = { 0 };
	shGetPhysicalDeviceQueueFamilies(
		physical_device,//physical_device
		NULL,//surface
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
		0,//protected
		&graphics_device_queue_info//p_device_queue_info
	);

	VkDeviceQueueCreateInfo device_queue_infos[2] = {
		graphics_device_queue_info,
	};
	shSetLogicalDevice(
		physical_device,//physical_device
		&device,//p_device
		0,//extension_count
		NULL,//pp_extension_names
		1,//device_queue_count
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

	shCombineMaxSamples(
		physical_device_properties, 64,
		1, 1, &sample_count
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

	vvoCreateDstImage(&vvo);

	shCreateRenderpassAttachment(//input color attachment
		vvo.src_image_format,//format
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
		VK_FORMAT_D32_SFLOAT,//format
		sample_count,//sample_count
		VK_ATTACHMENT_LOAD_OP_CLEAR,//load_treatment
		VK_ATTACHMENT_STORE_OP_STORE,//store_treatment
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,//stencil_load_treatment
		VK_ATTACHMENT_STORE_OP_DONT_CARE,//stencil_store_treatment
		VK_IMAGE_LAYOUT_UNDEFINED,//initial_layout
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,//final_layout
		&depth_attachment//p_attachment_description
	);
	shCreateRenderpassAttachmentReference(
		1,//attachment_idx
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,//layout
		&depth_attachment_reference//p_attachment_reference
	);
	
	vvoCreateRenderpassColorAttachment(&vvo, 2);//resolve attachment idx

	shCreateSubpass(
		VK_PIPELINE_BIND_POINT_GRAPHICS,//bind_point
		0,//input_attachment_count
		VK_NULL_HANDLE,//p_input_attachments_reference
		SUBPASS_COLOR_ATTACHMENT_COUNT,//color_attachment_count
		&input_color_attachment_reference,//p_color_attachments_reference
		&depth_attachment_reference,//p_depth_stencil_attachment_reference
		&vvo.color_attachment_reference,//p_resolve_attachment_reference
		0,//preserve_attachment_count
		VK_NULL_HANDLE,//p_preserve_attachments
		&subpass//p_subpass
	);

	VkAttachmentDescription attachment_descriptions[RENDERPASS_ATTACHMENT_COUNT] = {
		input_color_attachment, depth_attachment, vvo.color_attachment//replaces resolve_attachment
	};
	shCreateRenderpass(
		device,//device
		RENDERPASS_ATTACHMENT_COUNT,//attachment_count
		attachment_descriptions,//p_attachments_descriptions
		1,//subpass_count
		&subpass,//p_subpasses
		&renderpass//p_renderpass
	);
	
	//depth image
	shCreateImage(
		device,//device
		VK_IMAGE_TYPE_2D,//type
		width,//x
		height,//y
		1,//z
		VK_FORMAT_D32_SFLOAT,//format
		1,//mip_levels
		sample_count,//sample_count
		VK_IMAGE_TILING_OPTIMAL,//image_tiling
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,//usage
		VK_SHARING_MODE_EXCLUSIVE,//sharing_mode
		&depth_image//p_image
	);
	
	shAllocateImageMemory(
		device,//device
		physical_device,//physical_device
		depth_image,//image
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,//memory_property_flags
		&depth_image_memory//p_image_memory
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
		vvo.src_image_format, 1, sample_count,
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
		VK_IMAGE_ASPECT_COLOR_BIT, 1, vvo.src_image_format,
		&input_color_image_view
	);

	for (uint32_t i = 0; i < SRC_IMAGE_COUNT; i++) {
		VkImageView image_views[RENDERPASS_ATTACHMENT_COUNT] = {
			input_color_image_view, depth_image_view, vvo.src_image_views[i]// replaces swapchain_image_views
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
		SRC_IMAGE_COUNT,
		p_pipeline_pool
	);

	createPipeline(
		device, renderpass, 
		width, height, sample_count,
		SRC_IMAGE_COUNT,
		p_pipeline_pool
	);

	uint32_t swapchain_image_idx  = 0;//always will be if headless

	char* uri = "127.0.0.1:8002";

	vvoSetupServer(&vvo, uri);

	printf("Hosting stream server at %s\n", uri);

	while (1) {
		vvoPollEvents(&vvo);

		int _width = 0;
		int _height = 0;
			
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
		
		triangle[6] = (float)sin((double)(vvo.image_submissions_count) / 2.0f);//(float)sin(glfwGetTime());;
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
		p_colors[3] = 1.0f;


		clear_values[1].depthStencil.depth = 1.0f;

		shBeginRenderpass(
			cmd_buffer,//graphics_cmd_buffer
			renderpass,//renderpass
			0,//render_offset_x
			0,//render_offset_y
			width,//render_size_x
			height,//render_size_y
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

		//VVO check submissions
		if (vvo.image_submissions_count == 0) {
			shQueueSubmit(
				1,//cmd_buffer_count
				&cmd_buffer,//p_cmd_buffers
				graphics_queue,//queue
				graphics_cmd_fences[swapchain_image_idx],//fence
				0,//semaphores_to_wait_for_count
				NULL,//p_semaphores_to_wait_for
				VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,//wait_stage
				1,//signal_semaphore_count
				&current_graphics_queue_finished_semaphore//p_signal_semaphores
			);
		}
		else {
			shQueueSubmit(
				1,//cmd_buffer_count
				&cmd_buffer,//p_cmd_buffers
				graphics_queue,//queue
				graphics_cmd_fences[swapchain_image_idx],//fence
				1,//semaphores_to_wait_for_count
				&vvo.image_copy_semaphore,//p_semaphores_to_wait_for
				VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,//wait_stage
				1,//signal_semaphore_count
				&current_graphics_queue_finished_semaphore//p_signal_semaphores
			);
		}
		

		//Get image for streaming
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
			current_graphics_queue_finished_semaphore//graphics_queue_finished_semaphore
		);

		shWaitForFences(
			device,//device
			1,//fence_count
			&vvo.image_copy_fence,//p_fences
			1,//wait_for_all
			UINT64_MAX//timeout_ns
		);

		vvoReadDstImage(&vvo);

		vvoGetDstImagePngData(&vvo);
	}

	vvoVulkanRelease(&vvo);

	vvoRelease(&vvo);

	shWaitDeviceIdle(device);
	
	shPipelinePoolDestroyDescriptorPools(device, 0, 1, p_pipeline_pool);
	shPipelinePoolDestroyDescriptorSetLayouts(device, 0, 1, p_pipeline_pool);

	shPipelineDestroyShaderModules(device, 0, 2, p_pipeline);
	shPipelineDestroyLayout(device, p_pipeline);
	shDestroyPipeline(device, p_pipeline->pipeline);

	shClearPipeline(p_pipeline);

	shFreePipelinePool(p_pipeline_pool);


	shDestroySemaphores(device, 1, &current_graphics_queue_finished_semaphore);

	shDestroyFences(device, SRC_IMAGE_COUNT, graphics_cmd_fences);

	shDestroyCommandBuffers(device, graphics_cmd_pool, SRC_IMAGE_COUNT, graphics_cmd_buffers);

	shDestroyCommandPool(device, graphics_cmd_pool);

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

	shDestroyFramebuffers(device, SRC_IMAGE_COUNT, framebuffers);

	shDestroyDevice(device);

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
		device,//device
		staging_size,//size
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,//usage
		VK_SHARING_MODE_EXCLUSIVE,//sharing_mode
		p_staging_buffer//p_buffer
	);
	shAllocateBufferMemory(
		device,//device
		physical_device,//physical_device
		*p_staging_buffer,//buffer
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,//property_flags
		p_staging_memory//p_memory
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
		"../../examples/shaders/bin/mesh.vert.spv",
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
		"../../examples/shaders/bin/mesh.frag.spv",
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
