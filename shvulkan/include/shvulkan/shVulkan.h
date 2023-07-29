#ifndef SH_VULKAN_H
#define SH_VULKAN_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdlib.h>



#ifndef SH_TRUE
#ifndef __cplusplus
#define SH_TRUE 1
#else
#define SH_TRUE true
#endif//__cplusplus
#endif//SH_TRUE

#ifndef SH_FALSE
#ifndef __cplusplus
#define SH_FALSE 0
#else
#define SH_FALSE false
#endif//__cplusplus
#endif//SH_FALSE



#ifndef VK_MAKE_API_VERSION
#define VK_MAKE_API_VERSION(variant, major, minor, patch)\
    ((((uint32_t)(variant)) << 29U) | (((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))
#endif//VK_MAKE_API_VERSION



#define shVkError(condition, error_msg, failure_expression)\
	if ((int)(condition)) {\
		printf("shvulkan error: %s\n", (const char*)(error_msg));\
		failure_expression;\
	}

#define shVkResultError(result, error_msg, failure_expression)\
	if ((VkResult)(result) != VK_SUCCESS) {\
		printf("shvulkan error: %s, %s\n", error_msg, shTranslateVkResult((VkResult)(result)));\
		failure_expression;\
	}

#define SH_MAX_STACK_VALIDATION_LAYER_COUNT 32

	extern uint8_t shFindValidationLayer(
		const char* validation_layer_name
	);

	extern const char* shTranslateVkResult(
		VkResult vk_result
	);



#define SH_MAX_STACK_QUEUE_FAMILY_COUNT          32
#define SH_MAX_STACK_PHYSICAL_DEVICE_COUNT       8
#define SH_MAX_STACK_QUEUE_COUNT                 64
#define SH_MAX_STACK_DEVICE_SURFACE_FORMAT_COUNT 256
#define SH_MAX_STACK_SURFACE_PRESENT_MODE_COUNT  16

extern uint8_t shCreateInstance(
	const char*    application_name,
	const char*    engine_name,
	const uint8_t  enable_validation_layers,
	const uint32_t extension_count,
	const char**   pp_extension_names,
	uint32_t       api_version,
	VkInstance*    p_instance
);

extern uint8_t shGetPhysicalDeviceQueueFamilies(
	VkPhysicalDevice         physical_device,
	VkSurfaceKHR             surface,
	uint32_t*                p_queue_family_count,
	uint32_t*                p_graphics_queue_family_count,
	uint32_t*                p_surface_queue_family_count,
	uint32_t*                p_compute_queue_family_count,
	uint32_t*                p_transfer_queue_family_count,
	uint32_t*                p_graphics_queue_family_indices,
	uint32_t*                p_surface_queue_family_indices,
	uint32_t*                p_compute_queue_family_indices,
	uint32_t*                p_transfer_queue_family_indices,
	VkQueueFamilyProperties* p_queue_families_properties
);

extern uint8_t shGetQueueFamilySurfaceSupport(
	VkPhysicalDevice physical_device,
	uint32_t         queue_family_index,
	VkSurfaceKHR     surface,
	uint8_t*         p_support
);

extern uint8_t shSelectPhysicalDevice(
	VkInstance                        instance,
	VkSurfaceKHR                      surface,
	VkQueueFlags                      requirements,
	VkPhysicalDevice*                 p_physical_device,
	VkPhysicalDeviceProperties*       p_physical_device_properties,
	VkPhysicalDeviceFeatures*         p_physical_device_features,
	VkPhysicalDeviceMemoryProperties* p_physical_device_memory_properties
);

extern uint8_t shGetPhysicalDeviceSurfaceSupport(
	VkPhysicalDevice physical_device,
	uint32_t         queue_family_index,
	VkSurfaceKHR     surface,
	uint8_t*         p_supported
);

extern uint8_t shGetPhysicalDeviceSurfaceCapabilities(
	VkPhysicalDevice          physical_device,
	VkSurfaceKHR              surface,
	VkSurfaceCapabilitiesKHR* p_surface_capabilities
);

extern uint8_t shQueryForDeviceQueueInfo(
	uint32_t                 queue_family_index,
	uint32_t                 queue_count,
	float*                   p_queue_priorities,
	uint8_t                  protected,
	VkDeviceQueueCreateInfo* p_device_queue_info
);

extern uint8_t shSetLogicalDevice(
	VkPhysicalDevice         physical_device,
	VkDevice*                p_device,
	uint32_t                 extension_count,
	char**                   pp_extension_names,
	uint32_t                 device_queue_count,
	VkDeviceQueueCreateInfo* p_device_queue_infos
);

extern uint8_t shGetDeviceQueues(
	VkDevice  device,
	uint32_t  queue_count,
	uint32_t* p_queue_family_indices,
	VkQueue*  p_queues
);

extern uint8_t shCreateSwapchain(
	VkDevice                 device,
	VkPhysicalDevice         physical_device,
	VkSurfaceKHR             surface,
	VkFormat                 image_format,
	VkFormat*                p_image_format,
	uint32_t                 swapchain_image_count,
	VkSharingMode            image_sharing_mode,
	uint8_t                  vsync,
	uint32_t*                p_swapchain_image_count,
	VkSwapchainKHR*          p_swapchain
);

extern uint8_t shCombineMaxSamples(
	VkPhysicalDeviceProperties physical_device_properties,
	uint32_t                   sample_count,
	uint8_t                    combine_color_sample,
	uint8_t                    combine_depth_sample,
	uint32_t*                  p_sample_count
);

extern uint8_t shGetSwapchainImages(
	VkDevice       device,
	VkSwapchainKHR swapchain,
	uint32_t*      p_swapchain_image_count,
	VkImage*       p_swapchain_images
);

extern uint8_t shCreateImageView(
	VkDevice              device,
	VkImage               image,
	VkImageViewType       view_type,
	VkImageAspectFlagBits image_aspect,
	uint32_t              mip_levels,
	VkFormat              format,
	VkImageView*          p_image_view
);

extern uint8_t shCreateSwapchainImageViews(
	VkDevice     device,
	VkFormat     format,
	uint32_t     swapchain_image_count,
	VkImage*     p_swapchain_images,
	VkImageView* p_swapchain_image_views
);

extern uint8_t shCreateCommandPool(
	VkDevice       device,
	uint32_t       queue_family_index,
	VkCommandPool* p_cmd_pool
);

extern uint8_t shAllocateCommandBuffers(
	VkDevice         device,
	VkCommandPool    cmd_pool,
	uint32_t         cmd_buffer_count,
	VkCommandBuffer* p_cmd_buffer
);

extern uint8_t shCreateRenderpassAttachment(
	VkFormat                 format,
	uint32_t                 sample_count,
	VkAttachmentLoadOp       load_treatment,
	VkAttachmentStoreOp      store_treatment,
	VkAttachmentLoadOp       stencil_load_treatment,
	VkAttachmentStoreOp      stencil_store_treatment,
	VkImageLayout            initial_layout,
	VkImageLayout            final_layout,
	VkAttachmentDescription* p_attachment_description
);

extern uint8_t shCreateRenderpassAttachmentReference(
	uint32_t               attachment_idx,
	VkImageLayout          layout,
	VkAttachmentReference* p_attachment_reference
);

extern uint8_t shCreateSubpass(
	VkPipelineBindPoint    bind_point,
	uint32_t               input_attachment_count,
	VkAttachmentReference* p_input_attachments_reference,
	uint32_t               color_attachment_count,
	VkAttachmentReference* p_color_attachments_reference,
	VkAttachmentReference* p_depth_stencil_attachment_reference,
	VkAttachmentReference* p_resolve_attachment_reference,
	uint32_t               preserve_attachment_count,
	uint32_t*              p_preserve_attachments,
	VkSubpassDescription*  p_subpass
);

extern uint8_t shCreateRenderpass(
	VkDevice                 device,
	uint32_t                 attachment_count,
	VkAttachmentDescription* p_attachments_descriptions,
	uint32_t                 subpass_count,
	VkSubpassDescription*    p_subpasses,
	VkRenderPass*            p_renderpass
);

extern uint8_t shCreateFramebuffer(
	VkDevice       device,
	VkRenderPass   renderpass,
	uint32_t       image_view_count,
	VkImageView*   p_image_views,
	uint32_t       x,
	uint32_t       y,
	uint32_t       z,
	VkFramebuffer* p_framebuffer
);

extern uint8_t shWaitDeviceIdle(
	VkDevice device
);

extern uint8_t shDestroySwapchain(
	VkDevice       device,
	VkSwapchainKHR swapchain
);

extern uint8_t shDestroyFramebuffers(
	VkDevice       device,
	uint32_t       framebuffer_count,
	VkFramebuffer* p_framebuffers
);

extern uint8_t shDestroyImageViews(
	VkDevice     device,
	uint32_t     image_view_count,
	VkImageView* p_image_views
);

extern uint8_t shDestroySurface(
	VkInstance   instance,
	VkSurfaceKHR surface
);

extern uint8_t shDestroyCommandBuffers(
	VkDevice         device,
	VkCommandPool    cmd_pool,
	uint32_t         cmd_buffer_count,
	VkCommandBuffer* p_cmd_buffers
);

extern uint8_t shDestroyCommandPool(
	VkDevice      device,
	VkCommandPool cmd_pool
);

extern uint8_t shDestroyRenderpass(
	VkDevice     device,
	VkRenderPass render_pass
);

extern uint8_t shDestroyDevice(
	VkDevice device
);

extern uint8_t shDestroyInstance(
	VkInstance instance
);

extern uint8_t shResetCommandBuffer(
	VkCommandBuffer cmd_buffer
);

extern uint8_t shBeginCommandBuffer(
	VkCommandBuffer cmd_buffer
);

extern uint8_t shEndCommandBuffer(
	VkCommandBuffer cmd_buffer
);

extern uint8_t shCmdDispatch(
	VkCommandBuffer cmd_buffer,
	uint32_t        group_count_x,
	uint32_t        group_count_y,
	uint32_t        group_count_z
);

extern uint8_t shQueueSubmit(
	uint32_t             cmd_buffer_count,
	VkCommandBuffer*     p_cmd_buffers,
	VkQueue              queue,
	VkFence              fence,
	uint32_t             semaphores_to_wait_for_count,
	VkSemaphore*         p_semaphores_to_wait_for,
	VkPipelineStageFlags wait_stage,
	uint32_t             signal_semaphore_count,
	VkSemaphore*         p_signal_semaphores
);

extern uint8_t shWaitForQueue(
	VkQueue queue
);

extern uint8_t shCreateFences(
	VkDevice device,
	uint32_t fence_count,
	uint8_t  signaled,
	VkFence* p_fences
);

extern uint8_t shCreateSemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores
);

extern uint8_t shDestroyFences(
	VkDevice device,
	uint32_t fence_count,
	VkFence* p_fences
);

extern uint8_t shDestroySemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores
);

extern uint8_t shResetFences(
	VkDevice device,
	uint32_t fence_count,
	VkFence* p_fences
);

extern uint8_t shResetSemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores
);

extern uint8_t shWaitForFences(
	VkDevice device,
	uint32_t fence_count,
	VkFence* p_fences,
	uint8_t  wait_for_all,
	uint64_t timeout_ns
);

extern uint8_t shWaitForSemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores,
	uint8_t      wait_for_all,
	uint64_t     timeout_ns,
	uint64_t*    p_semaphores_values
);

extern uint8_t shAcquireSwapchainImage(
	VkDevice       device,
	VkSwapchainKHR swapchain,
	uint64_t       timeout_ns,
	VkSemaphore    acquired_signal_semaphore,
	VkFence        acquired_signal_fence,
	uint32_t*      p_swapchain_image_index,
	uint8_t*       p_swapchain_suboptimal
);

extern uint8_t shBeginRenderpass(
	VkCommandBuffer    graphics_cmd_buffer,
	VkRenderPass       renderpass,
	int32_t            render_offset_x,
	int32_t            render_offset_y,
	uint32_t           render_size_x,
	uint32_t           render_size_y,
	uint32_t           clear_value_count,
	VkClearValue*      p_clear_values,
	VkFramebuffer      framebuffer
);

extern uint8_t shEndRenderpass(
	VkCommandBuffer graphics_cmd_buffer
);

extern uint8_t shDraw(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        vertex_count,
	uint32_t        first_vertex,
	uint32_t        instance_count,
	uint32_t        first_instance
);

extern uint8_t shDrawIndexed(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        index_count,
	uint32_t        instance_count,
	uint32_t        first_index,
	int32_t         vertex_offset,
	uint32_t        first_instance
);

extern uint8_t shQueuePresentSwapchainImage(
	VkQueue        present_queue,
	uint32_t       semaphores_to_wait_for_count,
	VkSemaphore*   p_semaphores_to_wait_for,
	VkSwapchainKHR swapchain,
	uint32_t       swapchain_image_idx
);



#define SH_VEC1_SIGNED_FLOAT		VK_FORMAT_R32_SFLOAT
#define SH_VEC2_SIGNED_FLOAT		VK_FORMAT_R32G32_SFLOAT
#define SH_VEC3_SIGNED_FLOAT		VK_FORMAT_R32G32B32_SFLOAT
#define SH_VEC4_SIGNED_FLOAT		VK_FORMAT_R32G32B32A32_SFLOAT

#define SH_VEC1_SIGNED_DOUBLE		VK_FORMAT_R64_SFLOAT
#define SH_VEC2_SIGNED_DOUBLE		VK_FORMAT_R64G64_SFLOAT
#define SH_VEC3_SIGNED_DOUBLE		VK_FORMAT_R64G64B64_SFLOAT
#define SH_VEC4_SIGNED_DOUBLE		VK_FORMAT_R64G64B64A64_SFLOAT

#define SH_VEC1_SIGNED_INT			VK_FORMAT_R32_SINT
#define SH_VEC2_SIGNED_INT			VK_FORMAT_R32G32_SINT
#define SH_VEC3_SIGNED_INT			VK_FORMAT_R32G32B32_SINT
#define SH_VEC4_SIGNED_INT			VK_FORMAT_R32G32B32A32_SINT

#define SH_VEC1_UNSIGNED_INT		VK_FORMAT_R32_UINT
#define SH_VEC2_UNSIGNED_INT		VK_FORMAT_R32G32_UINT
#define SH_VEC3_UNSIGNED_INT		VK_FORMAT_R32G32B32_UINT
#define SH_VEC4_UNSIGNED_INT		VK_FORMAT_R32G32B32A32_UINT

#define SH_VEC1_UNSIGNED_LONG		VK_FORMAT_R64_UINT
#define SH_VEC2_UNSIGNED_LONG		VK_FORMAT_R64G64_UINT
#define SH_VEC3_UNSIGNED_LONG		VK_FORMAT_R64G64B64_UINT
#define SH_VEC4_UNSIGNED_LONG		VK_FORMAT_R64G64B64A64_UINT



extern uint8_t shCreateBuffer(
	VkDevice           device,
	uint32_t           size,
	VkBufferUsageFlags usage,
	VkSharingMode      sharing_mode,
	VkBuffer*          p_buffer
);

extern uint8_t shGetMemoryType(
	VkDevice              device,
	VkPhysicalDevice      physical_device,
	VkMemoryPropertyFlags property_flags,
	uint32_t*             p_memory_type_index
);

extern uint8_t shAllocateBufferMemory(
	VkDevice              device,
	VkPhysicalDevice      physical_device,
	VkBuffer              buffer,
	VkMemoryPropertyFlags property_flags,
	VkDeviceMemory*       p_memory
);

extern uint8_t shCopyBuffer(
	VkCommandBuffer transfer_cmd_buffer,
	VkBuffer        src_buffer,
	uint32_t        src_offset,
	uint32_t        dst_offset,
	uint32_t        size,
	VkBuffer        dst_buffer
);

#define SH_MAX_STACK_BUFFER_REGION_COUNT 256

uint8_t shCopyBufferRegions(
	VkCommandBuffer transfer_cmd_buffer,
	VkBuffer        src_buffer,
	uint32_t        region_count,
	uint32_t*       p_src_offsets,
	uint32_t*       p_dst_offsets,
	uint32_t*       p_sizes,
	VkBuffer        dst_buffer
);

extern uint8_t shBindBufferMemory(
	VkDevice       device,
	VkBuffer       buffer,
	uint32_t       offset,
	VkDeviceMemory buffer_memory
);

extern uint8_t shReadMemory(
	VkDevice       device,
	VkDeviceMemory memory,
	uint32_t       offset,
	uint32_t       data_size,
	void*          p_data
);

extern uint8_t shWriteMemory(
	VkDevice       device,
	VkDeviceMemory memory,
	uint32_t       offset,
	uint32_t       data_size,
	void*          p_data
);

extern uint8_t shClearBufferMemory(
	VkDevice       device,
	VkBuffer       buffer,
	VkDeviceMemory memory
);

extern uint8_t shCreateImage(
	VkDevice              device,
	VkImageType           type,
	uint32_t              x,
	uint32_t              y,
	uint32_t              z,
	VkFormat              format,
	uint32_t              mip_levels,
	VkSampleCountFlagBits sample_count,
	VkImageTiling         image_tiling,
	VkImageUsageFlags     usage,
	VkSharingMode         sharing_mode,
	VkImage*              p_image
);

extern uint8_t shAllocateImageMemory(
	VkDevice              device,
	VkPhysicalDevice      physical_device,
	VkImage               image,
	VkMemoryPropertyFlags memory_property_flags,
	VkDeviceMemory*       p_image_memory
);

extern uint8_t shBindImageMemory(
	VkDevice       device,
	VkImage        image,
	uint32_t       offset,
	VkDeviceMemory image_memory
);

extern uint8_t shClearImageMemory(
	VkDevice       device,
	VkImage        image,
	VkDeviceMemory image_memory
);

extern uint8_t shGetMemoryBudgetProperties(
	VkPhysicalDevice                           physical_device,
	VkPhysicalDeviceMemoryBudgetPropertiesEXT* p_memory_budget_properties
);

extern uint8_t shBindVertexBuffers(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        first_binding,
	uint32_t        binding_count,
	VkBuffer*       p_vertex_buffers,
	VkDeviceSize*   p_vertex_offsets
);

extern uint8_t shBindIndexBuffer(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        index_offset,
	VkBuffer        index_buffer
);



extern uint8_t shSetVertexBinding(
	uint32_t                           binding,
	uint32_t                           size,
	VkVertexInputRate                  input_rate,
	VkVertexInputBindingDescription*   p_vertex_input_binding
);

extern uint8_t shSetVertexAttribute(
	uint32_t                           location,
	uint32_t                           binding,
	VkFormat                           format,
	uint32_t                           offset,
	VkVertexInputAttributeDescription* p_vertex_input_attribute
);

extern uint8_t shSetVertexInputState(
	uint32_t                              vertex_binding_count,
	VkVertexInputBindingDescription*      p_vertex_bindings,
	uint32_t                              vertex_attribute_count,
	VkVertexInputAttributeDescription*    p_vertex_attributes,
	VkPipelineVertexInputStateCreateInfo* p_vertex_input_state
);

extern uint8_t shCreateInputAssembly(
	VkPrimitiveTopology                     primitive_topology,
	VkBool32                                primitive_restart_enable,
	VkPipelineInputAssemblyStateCreateInfo* p_input_assembly
);

extern uint8_t shCreateRasterizer(
	VkPolygonMode                           polygon_mode,
	VkCullModeFlagBits                      cull_mode,
	VkPipelineRasterizationStateCreateInfo* p_rasterizer
);

extern uint8_t shSetMultisampleState(
	VkSampleCountFlagBits                 sample_count,
	float                                 min_sample_shading_size,
	VkPipelineMultisampleStateCreateInfo* p_multisample_state
);

extern uint8_t shSetViewport(
	uint32_t                           viewport_pos_x,
	uint32_t                           viewport_pos_y,
	uint32_t                           viewport_width,
	uint32_t                           viewport_height,
	VkViewport*                        p_viewport,
	uint32_t                           scissors_pos_x,
	uint32_t                           scissors_pos_y,
	uint32_t                           scissors_width,
	uint32_t                           scissors_height,
	VkRect2D*                          p_scissors,
	VkPipelineViewportStateCreateInfo* p_viewport_state
);

extern uint8_t shColorBlendSettings(
	uint8_t                              enable_color_blending,
	uint8_t                              enable_alpha_blending,
	uint32_t                             subpass_color_attachment_count,
	VkPipelineColorBlendAttachmentState* p_color_blend_attachment_states,
	VkPipelineColorBlendStateCreateInfo* p_color_blend_state
);

extern uint8_t shCreateShaderModule(
	VkDevice        device,
	uint32_t        size,
	char*           code,
	VkShaderModule* p_shader_module
);

extern uint8_t shCreateShaderStage(
	VkShaderModule                   shader_module,
	VkShaderStageFlags               shader_stage_flag,
	VkPipelineShaderStageCreateInfo* p_shader_stage
);

extern uint8_t shSetPushConstants(
	VkShaderStageFlags   shader_stage,
	uint32_t             offset,
	uint32_t             size,
	VkPushConstantRange* p_push_constant_range
);

extern uint8_t shCreateDescriptorSetLayoutBinding(
	uint32_t                      binding,
	VkDescriptorType              descriptor_type,
	uint32_t                      descriptor_set_count,
	VkShaderStageFlags            shader_stage,
	VkDescriptorSetLayoutBinding* p_binding
);

extern uint8_t shCreateDescriptorSetLayout(
	VkDevice                      device,
	uint32_t                      binding_count,
	VkDescriptorSetLayoutBinding* p_bindings,
	VkDescriptorSetLayout*        p_descriptor_set_layout
);

extern uint8_t shCreateDescriptorPool(
	VkDevice              device,
	uint32_t              pool_size_count,
	VkDescriptorPoolSize* p_pool_sizes,
	VkDescriptorPool*     p_descriptor_pool
);

extern uint8_t shSetDescriptorBufferInfo(
	VkBuffer                buffer,
	uint32_t                buffer_offset,
	uint32_t                buffer_size,
	VkDescriptorBufferInfo* p_buffer_info
);

extern uint8_t shAllocateDescriptorSetUnits(
	VkDevice                device,
	VkDescriptorPool        descriptor_pool,
	VkDescriptorType        descriptor_type,
	uint32_t                binding,
	uint32_t                descriptor_set_unit_count,
	VkDescriptorSetLayout*  p_descriptor_set_layouts,
	VkDescriptorSet*        p_descriptor_sets,
	VkDescriptorBufferInfo* p_buffer_infos,
	VkWriteDescriptorSet*   p_write_descriptor_sets
);

extern uint8_t shCreatePipelineLayout(
	VkDevice               device,
	uint32_t               push_constant_range_count,
	VkPushConstantRange*   p_push_constants_range,
	uint32_t               src_descriptor_set_layout_count,
	VkDescriptorSetLayout* p_src_descriptor_set_layouts,
	VkPipelineLayout*      p_pipeline_layout
);

extern uint8_t shDestroyDescriptorPool(
	VkDevice         device,
	VkDescriptorPool descriptor_pool
);

extern uint8_t shDestroyDescriptorSetLayout(
	VkDevice              device,
	VkDescriptorSetLayout descriptor_set_layout
);

extern uint8_t shDestroyShaderModule(
	VkDevice       device,
	VkShaderModule shader_module
);

extern uint8_t shDestroyPipelineLayout(
	VkDevice         device,
	VkPipelineLayout pipeline_layout
);

extern uint8_t shDestroyPipeline(
	VkDevice   device,
	VkPipeline pipeline
);



#define SH_MAX_PIPELINE_VERTEX_BINDING_COUNT           32
#define SH_MAX_PIPELINE_VERTEX_ATTRIBUTE_COUNT         32
											           
#define SH_MAX_PIPELINE_SHADER_STAGE_COUNT             6

#define SH_MAX_PIPELINE_SUBPASS_COLOR_ATTACHMENT_COUNT 9

typedef struct ShVkPipeline {
	/*Vertex inputs*/
	uint32_t								vertex_binding_count;
	VkVertexInputBindingDescription			vertex_bindings[SH_MAX_PIPELINE_VERTEX_BINDING_COUNT];
	uint32_t								vertex_attribute_count;
	VkVertexInputAttributeDescription		vertex_attributes[SH_MAX_PIPELINE_VERTEX_ATTRIBUTE_COUNT];
	VkPipelineVertexInputStateCreateInfo	vertex_input_state_info;
	VkPipelineInputAssemblyStateCreateInfo	input_assembly;
	/*Shaders*/
	uint32_t                                shader_module_count;
	VkShaderModule                          shader_modules[SH_MAX_PIPELINE_SHADER_STAGE_COUNT];
	uint32_t                                shader_stage_count;
	VkPipelineShaderStageCreateInfo         shader_stages[SH_MAX_PIPELINE_SHADER_STAGE_COUNT];
	/*Push constants*/
	VkPushConstantRange                     push_constant_range;
	/*Descriptor sets*/
	//uint32_t                                descriptor_set_count;
	//uint32_t                                descriptor_set_unit_count;
	/*Rasterizer*/
	VkPipelineRasterizationStateCreateInfo	rasterizer;
	/*Viewport*/
	VkViewport								viewport;
	VkRect2D								scissors;
	VkPipelineViewportStateCreateInfo		viewport_state;
	/*Color blending*/
	VkPipelineColorBlendAttachmentState		color_blend_attachment_states[SH_MAX_PIPELINE_SUBPASS_COLOR_ATTACHMENT_COUNT];
	VkPipelineColorBlendStateCreateInfo		color_blend_state;
	/*Multisample state*/
	VkPipelineMultisampleStateCreateInfo	multisample_state_info;
	/*Pipeline*/
	VkPipelineLayout                        pipeline_layout;
	VkPipeline                              pipeline;
} ShVkPipeline;



typedef struct ShVkPipelinePool ShVkPipelinePool;



extern uint8_t shClearPipeline(
	ShVkPipeline* p_pipeline
);

extern uint8_t shPipelineSetVertexBinding(
	uint32_t          binding,
	uint32_t          size,
	VkVertexInputRate input_rate,
	ShVkPipeline*     p_pipeline
);

extern uint8_t shPipelineSetVertexAttribute(
	uint32_t      location,
	uint32_t      binding,
	VkFormat      format,
	uint32_t      offset,
	ShVkPipeline* p_pipeline
);

extern uint8_t shPipelineSetVertexInputState(
	ShVkPipeline* p_pipeline
);

extern uint8_t shPipelineCreateInputAssembly(
	VkPrimitiveTopology primitive_topology,
	VkBool32            primitive_restart_enable,
	ShVkPipeline*       p_pipeline
);

extern uint8_t shPipelineCreateRasterizer(
	VkPolygonMode      polygon_mode,
	VkCullModeFlagBits cull_mode,
	ShVkPipeline*      p_pipeline
);

extern uint8_t shPipelineSetMultisampleState(
	VkSampleCountFlagBits sample_count,
	float                 min_sample_shading_size,
	ShVkPipeline*         p_pipeline
);

extern uint8_t shPipelineSetViewport(
	uint32_t      viewport_pos_x,
	uint32_t      viewport_pos_y,
	uint32_t      viewport_width,
	uint32_t      viewport_height,
	uint32_t      scissors_pos_x,
	uint32_t      scissors_pos_y,
	uint32_t      scissors_width,
	uint32_t      scissors_height,
	ShVkPipeline* p_pipeline
);

extern uint8_t shPipelineColorBlendSettings(
	uint8_t       enable_color_blending,
	uint8_t       enable_alpha_blending,
	uint32_t      subpass_color_attachment_count,
	ShVkPipeline* p_pipeline
);

extern uint8_t shPipelineCreateShaderModule(
	VkDevice      device,
	uint32_t      size,
	char*         code,
	ShVkPipeline* p_pipeline
);

extern uint8_t shPipelineCreateShaderStage(
	VkShaderStageFlags shader_stage,
	ShVkPipeline*      p_pipeline
);

extern uint8_t shPipelineSetPushConstants(
	VkShaderStageFlags shader_stage,
	uint32_t           offset,
	uint32_t           size,
	ShVkPipeline*      p_pipeline
);

extern uint8_t shPipelineCreateLayout(
	VkDevice          device,
	uint32_t          first_descriptor_set_layout,
	uint32_t          descriptor_set_layout_count,
	ShVkPipelinePool* p_pipeline_pool,
	ShVkPipeline*     p_pipeline
);

extern uint8_t shSetupComputePipeline(
	VkDevice      device,
	ShVkPipeline* p_pipeline
);

extern uint8_t shSetupGraphicsPipeline(
	VkDevice      device,
	VkRenderPass  render_pass,
	ShVkPipeline* p_pipeline
);

extern uint8_t shPipelinePushConstants(
	VkCommandBuffer cmd_buffer,
	void*           p_data,
	ShVkPipeline*   p_pipeline
);

extern uint8_t shBindPipeline(
	VkCommandBuffer     cmd_buffer,
	VkPipelineBindPoint bind_point,
	ShVkPipeline*       p_pipeline
);

extern uint8_t shPipelineBindDescriptorSetUnits(
	VkCommandBuffer     cmd_buffer,
	uint32_t            first_descriptor_set,
	uint32_t            first_descriptor_set_unit_idx,
	uint32_t            descriptor_set_unit_count,
	VkPipelineBindPoint bind_point,
	uint32_t            dynamic_descriptors_count,
	uint32_t*           p_dynamic_offsets,
	ShVkPipelinePool*   p_pipeline_pool,
	ShVkPipeline*       p_pipeline
);

extern uint8_t shPipelineDestroyShaderModules(
	VkDevice      device,
	uint32_t      first_module,
	uint32_t      module_count,
	ShVkPipeline* p_pipeline
);

extern uint8_t shPipelineDestroyLayout(
	VkDevice      device,
	ShVkPipeline* p_pipeline
);



#define SH_PIPELINE_POOL_MAX_PIPELINE_COUNT                      64
#define SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT                    64

typedef struct ShVkPipelinePool {
	ShVkPipeline                 pipelines[SH_PIPELINE_POOL_MAX_PIPELINE_COUNT];

	uint32_t                     descriptor_set_layout_binding_count;
	uint32_t                     src_descriptor_set_layout_count;
	uint32_t                     descriptor_pool_count;
	
	uint32_t                     descriptor_count;
	uint32_t                     write_descriptor_set_count;
	uint32_t                     descriptor_set_unit_count;//= write_descriptor_set_count

	VkDescriptorSetLayoutBinding descriptor_set_layout_bindings[SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT];//there is redundancy in order to fulfill the requirements for write desccriptor sets
	VkDescriptorSetLayout        descriptor_set_layouts        [SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT];//there is redundancy in order to fulfill the requirements for write desccriptor sets

	VkDescriptorPool             descriptor_pools              [SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT];
	VkDescriptorSet              descriptor_sets               [SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT];
	VkDescriptorBufferInfo       descriptor_buffer_infos       [SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT];
	VkWriteDescriptorSet         write_descriptor_sets         [SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT];

} ShVkPipelinePool;



#define shAllocatePipelinePool() ((ShVkPipelinePool*)calloc(1, sizeof(ShVkPipelinePool)))
#define shFreePipelinePool       free

extern uint8_t shPipelinePoolCreateDescriptorSetLayoutBinding(
	uint32_t           binding_idx,
	uint32_t           binding,
	VkDescriptorType   descriptor_type,
	uint32_t           descriptor_set_count,
	VkShaderStageFlags shader_stage,
	ShVkPipelinePool*  p_pipeline_pool
);

extern uint8_t shPipelinePoolCopyDescriptorSetLayoutBinding(
	uint32_t          src_binding_idx,
	uint32_t          first_dst_binding_idx,
	uint32_t          dst_binding_count,
	ShVkPipelinePool* p_pipeline_pool
);

extern uint8_t shPipelinePoolCreateDescriptorSetLayout(
	VkDevice          device,
	uint32_t          first_binding_idx,
	uint32_t          binding_count,
	uint32_t          set_layout_idx,//set_idx
	ShVkPipelinePool* p_pipeline_pool
);

extern uint8_t shPipelinePoolCopyDescriptorSetLayout(
	uint32_t          src_set_layout_idx,
	uint32_t          first_dst_set_layout_idx,
	uint32_t          dst_set_layout_count,
	ShVkPipelinePool* p_pipeline_pool
);

extern uint8_t shPipelinePoolCreateDescriptorPool(
	VkDevice          device,
	uint32_t          pool_idx,
	VkDescriptorType  descriptor_type,
	uint32_t          descriptor_count,
	ShVkPipelinePool* p_pipeline_pool
);

extern uint8_t shPipelinePoolAllocateDescriptorSetUnits(
	VkDevice          device,
	uint32_t          binding,
	uint32_t          pool_idx,
	VkDescriptorType  descriptor_type,
	uint32_t          first_descriptor_set_unit,
	uint32_t          descriptor_set_unit_count,
	ShVkPipelinePool* p_pipeline_pool
);

extern uint8_t shPipelinePoolSetDescriptorBufferInfos(
	uint32_t          first_descriptor,
	uint32_t          descriptor_count,
	VkBuffer          buffer,
	uint32_t          buffer_offset,
	uint32_t          buffer_size,
	ShVkPipelinePool* p_pipeline_pool
);

extern uint8_t shPipelinePoolDestroyDescriptorSetLayouts(
	VkDevice          device,
	uint32_t          first_set_layout,
	uint32_t          set_layout_count,
	ShVkPipelinePool* p_pipeline_pool
);

extern uint8_t shPipelinePoolDestroyDescriptorPools(
	VkDevice          device,
	uint32_t          first_pool,
	uint32_t          pool_count,
	ShVkPipelinePool* p_pipeline_pool
);

extern uint8_t shPipelinePoolUpdateDescriptorSetUnits(
	VkDevice          device,
	uint32_t          first_descriptor_set_unit,
	uint32_t          descriptor_set_unit_count,
	ShVkPipelinePool* p_pipeline_pool
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VULKAN_H