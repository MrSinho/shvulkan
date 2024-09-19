#ifndef SH_VULKAN_H
#define SH_VULKAN_H



/**
 * @file my_vulkan_wrapper.h
 * @brief This file contains Vulkan wrapper functions for handling device creation and resource management.
 */



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



/**
 * @brief Error-checking macro for Vulkan conditions.
 * 
 * This macro checks a given condition. If the condition evaluates to true, it prints an error message
 * and executes a failure expression (like returning from a function or exiting).
 * 
 * @param condition The condition to check (non-zero for an error).
 * @param error_msg A message to be printed if the condition is true.
 * @param failure_expression The expression to execute when the condition is true (e.g., return or exit).
 * 
 * @note This macro does not return any value but executes the failure_expression if the condition is met.
 */
#define shVkError(condition, error_msg, failure_expression)\
	if ((int)(condition)) {\
		printf("shvulkan error: %s\n", (const char*)(error_msg));\
		failure_expression;\
	}

/**
 * @brief Error-checking macro for Vulkan VkResult.
 * 
 * This macro checks the result of a Vulkan API call. If the result is not VK_SUCCESS, 
 * it prints an error message along with the translated Vulkan error code and executes 
 * a failure expression (like returning from a function or exiting).
 * 
 * @param result The VkResult from a Vulkan function (VK_SUCCESS or an error code).
 * @param error_msg A message to be printed if the result is not VK_SUCCESS.
 * @param failure_expression The expression to execute when the result is an error (e.g., return or exit).
 * 
 * @note This macro calls `shTranslateVkResult()` to convert the VkResult enum into a string representation.
 */
#define shVkResultError(result, error_msg, failure_expression)\
	if ((VkResult)(result) != VK_SUCCESS) {\
		printf("shvulkan error: %s, %s\n", error_msg, shTranslateVkResult((VkResult)(result)));\
		failure_expression;\
	}

/**
 * @brief Translates a VkResult enum into a literal string.
 * 
 * This function converts a Vulkan `VkResult` code into a literal string that describes
 * the result, which is helpful for error messages and debugging.
 * 
 * @param vk_result The Vulkan result code (VkResult).
 * 
 * @return A string representing the Vulkan result literal.
 * 
 * @note This function is often used in error reporting to provide more descriptive
 * information about Vulkan errors, such as converting `VK_ERROR_OUT_OF_HOST_MEMORY`
 * to its string form.
 */
extern const char* shTranslateVkResult(
	VkResult vk_result
);


#define SH_MAX_STACK_VALIDATION_LAYER_COUNT 32

#define SH_MAX_STACK_QUEUE_FAMILY_COUNT          32
#define SH_MAX_STACK_PHYSICAL_DEVICE_COUNT       8
#define SH_MAX_STACK_QUEUE_COUNT                 64
#define SH_MAX_STACK_DEVICE_SURFACE_FORMAT_COUNT 256
#define SH_MAX_STACK_SURFACE_PRESENT_MODE_COUNT  16



/**
 * @brief Searches for a specific Vulkan validation layer.
 * 
 * This function searches through the available Vulkan validation layers 
 * to find a match with the provided validation layer name.
 * 
 * @param validation_layer_name The name of the validation layer to find.
 * 
 * @return 1 if the validation layer is found, 0 otherwise.
 * 
 * @note This function is typically used to check if a specific validation layer 
 * (e.g., "VK_LAYER_KHRONOS_validation") is available for use.
 */
extern uint8_t shFindValidationLayer(
	const char* validation_layer_name
);

/**
 * @brief Creates a Vulkan instance.
 * 
 * This function initializes a Vulkan instance with the specified application name, engine name, 
 * validation layers, and extensions.
 * 
 * @param application_name The name of the application.
 * @param engine_name The name of the engine.
 * @param enable_validation_layers Flag to enable (1) or disable (0) validation layers.
 * @param extension_count Number of Vulkan extensions to enable.
 * @param pp_extension_names Valid array of extension names to enable.
 * @param api_version The Vulkan API version to use.
 * @param p_instance Valid destination pointer to the newly created VkInstance.
 * 
 * @return 1 if the instance is created successfully, 0 otherwise.
 */
extern uint8_t shCreateInstance(
	const char*    application_name,
	const char*    engine_name,
	const uint8_t  enable_validation_layers,
	const uint32_t extension_count,
	const char**   pp_extension_names,
	uint32_t       api_version,
	VkInstance*    p_instance
);

/**
 * @brief Retrieves the queue family indices for a physical device.
 * 
 * This function retrieves and categorizes the queue families available for a specified physical device 
 * and surface, such as graphics, compute, transfer, and presentation (surface support).
 * 
 * @param physical_device The current physical device to query for queue families.
 * @param surface The Vulkan surface (for surface support queries).
 * @param p_queue_family_count Valid destination pointer to the total number of queue families available.
 * @param p_graphics_queue_family_count Valid destination pointer to Number of graphics-capable queue families.
 * @param p_surface_queue_family_count Valid destination pointer to Number of surface-capable queue families.
 * @param p_compute_queue_family_count Valid destination pointer to Number of compute-capable queue families.
 * @param p_transfer_queue_family_count Valid destination pointer to Number of transfer-capable queue families.
 * @param p_graphics_queue_family_indices Valid destination pointer to the indices of the graphics-capable queue families.
 * @param p_surface_queue_family_indices Valid destination pointer to the indices of the surface-capable queue families.
 * @param p_compute_queue_family_indices Valid destination pointer to the indices of the compute-capable queue families.
 * @param p_transfer_queue_family_indices Valid destination pointer to the indices of the transfer-capable queue families.
 * @param p_queue_families_properties Valid destination pointer to a `VkQueueFamilyProperties` structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Checks if a queue family supports presenting to a surface.
 * 
 * This function checks whether a queue family of a physical device supports presentation to a surface.
 * 
 * @param physical_device Valid physical device to query.
 * @param queue_family_index Index of the queue family to check.
 * @param surface Valid surface to check for support.
 * @param p_support Valid destination pointer to Flag indicating support (1 if supported, 0 otherwise).
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shGetQueueFamilySurfaceSupport(
	VkPhysicalDevice physical_device,
	uint32_t         queue_family_index,
	VkSurfaceKHR     surface,
	uint8_t*         p_support
);

/**
 * @brief Selects a physical device that meets specified requirements.
 * 
 * This function selects a Vulkan physical device based on specified requirements such as queue support, 
 * surface capabilities, and required device features. Then it retrieves vital information about the GPU.
 * 
 * @param instance Valid Vulkan instance.
 * @param surface Valid surface for presentation support.
 * @param requirements Queue family requirements (VkQueueFlags).
 * @param p_physical_device Valid destination pointer to the selected Vulkan physical device.
 * @param p_physical_device_properties Valid destination pointer to the properties of the selected physical device.
 * @param p_physical_device_features Valid destination pointer to the features of the selected physical device.
 * @param p_physical_device_memory_properties Valid destination pointer to the memory properties of the selected physical device.
 * 
 * @return 1 if a suitable device is found, 0 otherwise.
 */
extern uint8_t shSelectPhysicalDevice(
	VkInstance                        instance,
	VkSurfaceKHR                      surface,
	VkQueueFlags                      requirements,
	VkPhysicalDevice*                 p_physical_device,
	VkPhysicalDeviceProperties*       p_physical_device_properties,
	VkPhysicalDeviceFeatures*         p_physical_device_features,
	VkPhysicalDeviceMemoryProperties* p_physical_device_memory_properties
);

/**
 * @brief Queries if a queue family supports presenting to a surface.
 * 
 * This function queries whether a specific queue family of a physical device supports presenting 
 * to a Vulkan surface.
 * 
 * @param physical_device Valid physical device.
 * @param queue_family_index Index of the queue family to check.
 * @param surface Valid surface to check for support.
 * @param p_supported Valid destination pointer to Flag indicating support (1 if supported, 0 otherwise).
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shGetPhysicalDeviceSurfaceSupport(
	VkPhysicalDevice physical_device,
	uint32_t         queue_family_index,
	VkSurfaceKHR     surface,
	uint8_t*         p_supported
);

/**
 * @brief Retrieves the surface capabilities of a physical device.
 * 
 * This function queries the surface capabilities (such as min/max image count and extent) for a 
 * physical device and surface combination.
 * 
 * @param physical_device Valid physical device.
 * @param surface Valid Vulkan surface to query capabilities for.
 * @param p_surface_capabilities Valid destination pointer to a structure where the surface capabilities will be stored.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shGetPhysicalDeviceSurfaceCapabilities(
	VkPhysicalDevice          physical_device,
	VkSurfaceKHR              surface,
	VkSurfaceCapabilitiesKHR* p_surface_capabilities
);

/**
 * @brief Queries information for device queue creation.
 * 
 * This function sets up the necessary information for creating device queues, including the 
 * queue family index, queue count, priorities, and queue protection settings.
 * 
 * @param queue_family_index Index of the queue family.
 * @param queue_count Number of queues to create in the specified family.
 * @param p_queue_priorities Valid pointer to an array of queue priorities.
 * @param protected Flag indicating whether the queue should be protected (1 for protected, 0 for unprotected).
 * @param p_device_queue_info Valid destination pointer to a VkDeviceQueueCreateInfo structure where the queue info will be stored.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shQueryForDeviceQueueInfo(
	uint32_t                 queue_family_index,
	uint32_t                 queue_count,
	float*                   p_queue_priorities,
	uint8_t                  protected,
	VkDeviceQueueCreateInfo* p_device_queue_info
);

/**
 * @brief Sets up a Vulkan logical device.
 * 
 * This function creates a Vulkan logical device with specified extensions, queue creation info, and 
 * other configurations.
 * 
 * @param physical_device Valid Vulkan physical device.
 * @param p_device Valid destination pointer to the newly created VkDevice.
 * @param extension_count Number of Vulkan extensions to enable.
 * @param pp_extension_names Valid pointer to extension names to enable.
 * @param device_queue_count Number of device queues to create.
 * @param p_device_queue_infos Valid pointer to an array of VkDeviceQueueCreateInfo structures.
 * 
 * @return 1 if the logical device is created successfully, 0 otherwise.
 */
extern uint8_t shSetLogicalDevice(
	VkPhysicalDevice         physical_device,
	VkDevice*                p_device,
	uint32_t                 extension_count,
	char**                   pp_extension_names,
	uint32_t                 device_queue_count,
	VkDeviceQueueCreateInfo* p_device_queue_infos
);

/**
 * @brief Retrieves the Vulkan queues from a device.
 * 
 * This function retrieves the Vulkan queues from a logical device based on specified queue family indices.
 * 
 * @param device Valid Vulkan device.
 * @param queue_count Number of queues to retrieve.
 * @param p_queue_family_indices Valid pointer to an array of queue family indices.
 * @param p_queues Valid destination pointer to an array of VkQueue handles.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shGetDeviceQueues(
	VkDevice  device,
	uint32_t  queue_count,
	uint32_t* p_queue_family_indices,
	VkQueue*  p_queues
);

/**
 * @brief Creates a Vulkan swapchain.
 * 
 * This function sets up a Vulkan swapchain with the specified surface, image format, and other parameters.
 * 
 * @param device Valid Vulkan device.
 * @param physical_device Valid Vulkan physical device.
 * @param surface Valid Vulkan surface.
 * @param image_format Format of the images in the swapchain.
 * @param p_image_format Valid destination pointer to the format of the swapchain images.
 * @param swapchain_image_count Number of images in the swapchain.
 * @param image_sharing_mode Sharing mode for the swapchain images.
 * @param vsync Flag to enable (1) or disable (0) V-Sync.
 * @param p_swapchain_image_count Valid destination pointer to the number of swapchain images.
 * @param p_swapchain Valid destination pointer to the newly created VkSwapchainKHR.
 * 
 * @note @ref p_swapchain_image_count might be different from @ref swapchain_image_count when @ref swapchain_image_count is not supported! Please take this in consideration for your API implementations.
 * 
 * @return 1 if the swapchain is created successfully, 0 otherwise.
 */
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

/**
 * @brief Combines sample counts based on physical device properties.
 * 
 * This function determines the maximum number of samples that can be used for color and depth 
 * attachments, considering the physical device properties.
 * 
 * @param physical_device_properties Properties of the physical device.
 * @param sample_count Sample count to combine.
 * @param combine_color_sample Flag indicating whether to combine color samples.
 * @param combine_depth_sample Flag indicating whether to combine depth samples.
 * @param p_sample_count Destination pointer to the resulting maximum supported sample count for the current hardware.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCombineMaxSamples(
	VkPhysicalDeviceProperties physical_device_properties,
	uint32_t                   sample_count,
	uint8_t                    combine_color_sample,
	uint8_t                    combine_depth_sample,
	uint32_t*                  p_sample_count
);

/**
 * @brief Retrieves the images in a swapchain.
 * 
 * This function retrieves the Vulkan images present in a swapchain.
 * 
 * @param device Valid Vulkan device.
 * @param swapchain Valid Vulkan swapchain.
 * @param p_swapchain_image_count Valid destination pointer to the number of swapchain images.
 * @param p_swapchain_images Valid destination pointer to an array of VkImage handles.
 *  
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shGetSwapchainImages(
	VkDevice       device,
	VkSwapchainKHR swapchain,
	uint32_t*      p_swapchain_image_count,
	VkImage*       p_swapchain_images
);

/**
 * @brief Creates a Vulkan image view.
 * 
 * This function creates a Vulkan image view for a specified image, view type, format, and aspect.
 * 
 * @param device Valid Vulkan device.
 * @param image Valid Vulkan image.
 * @param view_type Type of the image view.
 * @param image_aspect Aspect flags of the image (e.g., color, depth).
 * @param mip_levels Number of mipmap levels.
 * @param format Format of the image.
 * @param p_image_view Valid destination pointer to the newly created VkImageView.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateImageView(
	VkDevice              device,
	VkImage               image,
	VkImageViewType       view_type,
	VkImageAspectFlagBits image_aspect,
	uint32_t              mip_levels,
	VkFormat              format,
	VkImageView*          p_image_view
);

/**
 * @brief Creates image views for swapchain images.
 * 
 * This function creates Vulkan image views for all images in a swapchain.
 * 
 * @param device Valid Vulkan device.
 * @param format Format of the image views.
 * @param swapchain_image_count Number of swapchain images.
 * @param p_swapchain_images Pointer to an array of VkImage handles.
 * @param p_swapchain_image_views Valid destination pointer to an array of VkImageView handles.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateSwapchainImageViews(
	VkDevice     device,
	VkFormat     format,
	uint32_t     swapchain_image_count,
	VkImage*     p_swapchain_images,
	VkImageView* p_swapchain_image_views
);

/**
 * @brief Creates a Vulkan command pool.
 * 
 * This function creates a command pool for a specified queue family.
 * 
 * @param device Valid Vulkan device.
 * @param queue_family_index Queue family index for the command pool.
 * @param p_cmd_pool Valid destination pointer to the newly created VkCommandPool.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateCommandPool(
	VkDevice       device,
	uint32_t       queue_family_index,
	VkCommandPool* p_cmd_pool
);

/**
 * @brief Allocates command buffers from a command pool.
 * 
 * This function allocates a specified number of command buffers from a command pool.
 * 
 * @param device Valid Vulkan device.
 * @param cmd_pool Valid pointer to the command pool from which to allocate.
 * @param cmd_buffer_count Number of command buffers to allocate.
 * @param p_cmd_buffer Valid destination pointer to an array of newly created VkCommandBuffer handles.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shAllocateCommandBuffers(
	VkDevice         device,
	VkCommandPool    cmd_pool,
	uint32_t         cmd_buffer_count,
	VkCommandBuffer* p_cmd_buffer
);

/**
 * @brief Creates a Vulkan render pass attachment description.
 * 
 * This function creates an attachment description to use in a render pass.
 * 
 * @param format Format of the attachment.
 * @param sample_count Sample count for the attachment.
 * @param load_treatment Load operation for the attachment.
 * @param store_treatment Store operation for the attachment.
 * @param stencil_load_treatment Load operation for stencil attachment.
 * @param stencil_store_treatment Store operation for stencil attachment.
 * @param initial_layout Initial layout of the attachment.
 * @param final_layout Final layout of the attachment.
 * @param p_attachment_description Valid destination pointer to the newly created VkAttachmentDescription.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Creates a Vulkan render pass attachment reference.
 * 
 * This function creates an attachment reference for use in a subpass of a render pass.
 * 
 * @param attachment_idx Index of the attachment in the render pass.
 * @param layout Layout of the attachment within the subpass.
 * @param p_attachment_reference Valid destination pointer to the newly created VkAttachmentReference.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateRenderpassAttachmentReference(
	uint32_t               attachment_idx,
	VkImageLayout          layout,
	VkAttachmentReference* p_attachment_reference
);

/**
 * @brief Creates a Vulkan subpass description.
 * 
 * This function creates a subpass description including input, color, depth/stencil, and resolve attachments.
 * 
 * @param bind_point Pipeline bind point for the subpass.
 * @param input_attachment_count Number of input attachments.
 * @param p_input_attachments_reference Valid pointer to an array of input attachment references.
 * @param color_attachment_count Number of color attachments.
 * @param p_color_attachments_reference Valid pointer to an array of color attachment references.
 * @param p_depth_stencil_attachment_reference Valid pointer to the depth/stencil attachment reference.
 * @param p_resolve_attachment_reference Valid pointer to the resolve attachment reference.
 * @param preserve_attachment_count Number of attachments to preserve.
 * @param p_preserve_attachments Valid pointer to an array of indices of attachments to preserve.
 * @param p_subpass Valid destination pointer to the newly created VkSubpassDescription.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Creates a Vulkan render pass.
 * 
 * This function creates a render pass with specified attachments and subpasses.
 * 
 * @param device Valid pointer to Vulkan device.
 * @param attachment_count Number of attachments in the render pass.
 * @param p_attachments_descriptions Valid pointer to an array of VkAttachmentDescription structures.
 * @param subpass_count Number of subpasses in the render pass.
 * @param p_subpasses Valid pointer to an array of VkSubpassDescription structures.
 * @param p_renderpass Valid destination pointer to the newly created VkRenderPass.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateRenderpass(
	VkDevice                 device,
	uint32_t                 attachment_count,
	VkAttachmentDescription* p_attachments_descriptions,
	uint32_t                 subpass_count,
	VkSubpassDescription*    p_subpasses,
	VkRenderPass*            p_renderpass
);

/**
 * @brief Creates a Vulkan framebuffer.
 * 
 * This function creates a framebuffer with specified image views and dimensions for use with a render pass.
 * 
 * @param device Valid Vulkan device.
 * @param renderpass Valid Vulkan render pass.
 * @param image_view_count Number of image views in the framebuffer.
 * @param p_image_views Valid pointer to an array of VkImageView handles.
 * @param x Width of the framebuffer.
 * @param y Height of the framebuffer.
 * @param z Depth of the framebuffer.
 * @param p_framebuffer Valid destination pointer to the newly created VkFramebuffer.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Waits for a Vulkan device to become idle.
 * 
 * This function waits until the Vulkan device has finished all its operations.
 * 
 * @param device Valid Vulkan device.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shWaitDeviceIdle(
	VkDevice device
);

/**
 * @brief Destroys a Vulkan swapchain.
 * 
 * This function destroys the specified Vulkan swapchain.
 * 
 * @param device Valid Vulkan device.
 * @param swapchain Valid VkSwapchainKHR to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroySwapchain(
	VkDevice       device,
	VkSwapchainKHR swapchain
);

/**
 * @brief Destroys Vulkan framebuffers.
 * 
 * This function destroys an array of Vulkan framebuffers.
 * 
 * @param device Valid Vulkan device.
 * @param framebuffer_count Number of framebuffers to destroy.
 * @param p_framebuffers Valid pointer to an array of VkFramebuffer handles.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyFramebuffers(
	VkDevice       device,
	uint32_t       framebuffer_count,
	VkFramebuffer* p_framebuffers
);

/**
 * @brief Destroys Vulkan image views.
 * 
 * This function destroys an array of Vulkan image views.
 * 
 * @param device Valid Vulkan device.
 * @param image_view_count Number of image views to destroy.
 * @param p_image_views Valid pointer to an array of VkImageView handles.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyImageViews(
	VkDevice     device,
	uint32_t     image_view_count,
	VkImageView* p_image_views
);

/**
 * @brief Destroys a Vulkan surface.
 * 
 * This function destroys a Vulkan surface associated with an instance.
 * 
 * @param instance Valid Vulkan instance.
 * @param surface Valid VkSurfaceKHR to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroySurface(
	VkInstance   instance,
	VkSurfaceKHR surface
);

/**
 * @brief Destroys Vulkan command buffers.
 * 
 * This function destroys an array of Vulkan command buffers from a command pool.
 * 
 * @param device Valid Vulkan device.
 * @param cmd_pool Valid Vulkan command pool.
 * @param cmd_buffer_count Number of command buffers to destroy.
 * @param p_cmd_buffers Valid pointer to an array of VkCommandBuffer handles.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyCommandBuffers(
	VkDevice         device,
	VkCommandPool    cmd_pool,
	uint32_t         cmd_buffer_count,
	VkCommandBuffer* p_cmd_buffers
);

/**
 * @brief Destroys a Vulkan command pool.
 * 
 * This function destroys a Vulkan command pool.
 * 
 * @param device Valid Vulkan device.
 * @param cmd_pool Valid Vulkan command pool to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyCommandPool(
	VkDevice      device,
	VkCommandPool cmd_pool
);

/**
 * @brief Destroys a Vulkan render pass.
 * 
 * This function destroys a Vulkan render pass.
 * 
 * @param device Valid Vulkan device.
 * @param render_pass Valid Vulkan render pass to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyRenderpass(
	VkDevice     device,
	VkRenderPass render_pass
);

/**
 * @brief Destroys a Vulkan device.
 * 
 * This function destroys a Vulkan device.
 * 
 * @param device Valid Vulkan device to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyDevice(
	VkDevice device
);

/**
 * @brief Destroys a Vulkan instance.
 * 
 * This function destroys a Vulkan instance.
 * 
 * @param instance Valid Vulkan instance to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyInstance(
	VkInstance instance
);

/**
 * @brief Resets a Vulkan command buffer.
 * 
 * This function resets a command buffer to the initial state.
 * 
 * @param cmd_buffer Valid Vulkan command buffer to reset.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shResetCommandBuffer(
	VkCommandBuffer cmd_buffer
);

/**
 * @brief Begins recording commands into a Vulkan command buffer.
 * 
 * This function begins recording commands into the specified command buffer.
 * 
 * @param cmd_buffer Valid Vulkan command buffer to begin recording.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shBeginCommandBuffer(
	VkCommandBuffer cmd_buffer
);

/**
 * @brief Ends recording commands into a Vulkan command buffer.
 * 
 * This function ends the recording of commands into the specified command buffer.
 * 
 * @param cmd_buffer Valid Vulkan command buffer to end recording.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shEndCommandBuffer(
	VkCommandBuffer cmd_buffer
);

/**
 * @brief Dispatches compute work from a Vulkan command buffer.
 * 
 * This function dispatches a compute workload with specified group counts.
 * 
 * @param cmd_buffer Valid Vulkan command buffer from which to dispatch.
 * @param group_count_x Number of groups in the x dimension.
 * @param group_count_y Number of groups in the y dimension.
 * @param group_count_z Number of groups in the z dimension.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCmdDispatch(
	VkCommandBuffer cmd_buffer,
	uint32_t        group_count_x,
	uint32_t        group_count_y,
	uint32_t        group_count_z
);

/**
 * @brief Submits command buffers to a Vulkan queue.
 * 
 * This function submits command buffers to a Vulkan queue and optionally waits for a fence and signals semaphores.
 * 
 * @param cmd_buffer_count Number of command buffers to submit.
 * @param p_cmd_buffers Valid pointer to an array of Vulkan command buffers.
 * @param queue Valid Vulkan queue to which to submit.
 * @param fence Valid Vulkan fence to signal upon completion.
 * @param semaphores_to_wait_for_count Number of semaphores to wait for.
 * @param p_semaphores_to_wait_for Valid pointer to an array of Vulkan semaphores to wait for.
 * @param wait_stage Pipeline stage flags to wait for.
 * @param signal_semaphore_count Number of semaphores to signal.
 * @param p_signal_semaphores Valid pointer to an array of Vulkan semaphores to signal.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Waits for a Vulkan queue to become idle.
 * 
 * This function waits until the Vulkan queue has finished all its operations.
 * 
 * @param queue Valid Vulkan queue to wait for.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shWaitForQueue(
	VkQueue queue
);

/**
 * @brief Creates Vulkan fences.
 * 
 * This function creates a specified number of Vulkan fences.
 * 
 * @param device Valid Vulkan device.
 * @param fence_count Number of fences to create.
 * @param signaled Whether to initialize fences as signaled.
 * @param p_fences Valid destination pointer to an array of newly created Vulkan fences.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateFences(
	VkDevice device,
	uint32_t fence_count,
	uint8_t  signaled,
	VkFence* p_fences
);

/**
 * @brief Creates Vulkan semaphores.
 * 
 * This function creates a specified number of Vulkan semaphores.
 * 
 * @param device Valid Vulkan device.
 * @param semaphore_count Number of semaphores to create.
 * @param p_semaphores Valid destination pointer to an array of newly created Vulkan semaphores.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateSemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores
);

/**
 * @brief Destroys Vulkan fences.
 * 
 * This function destroys a specified number of Vulkan fences.
 * 
 * @param device Valid Vulkan device.
 * @param fence_count Number of fences to destroy.
 * @param p_fences Valid pointer to an array of Vulkan fences to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyFences(
	VkDevice device,
	uint32_t fence_count,
	VkFence* p_fences
);

/**
 * @brief Destroys Vulkan semaphores.
 * 
 * This function destroys a specified number of Vulkan semaphores.
 * 
 * @param device Valid Vulkan device.
 * @param semaphore_count Number of semaphores to destroy.
 * @param p_semaphores Valid pointer to an array of Vulkan semaphores to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroySemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores
);

/**
 * @brief Resets Vulkan fences.
 * 
 * This function resets a specified number of Vulkan fences to their initial state.
 * 
 * @param device Valid Vulkan device.
 * @param fence_count Number of fences to reset.
 * @param p_fences Valid pointer to an array of Vulkan fences to reset.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shResetFences(
	VkDevice device,
	uint32_t fence_count,
	VkFence* p_fences
);

/**
 * @brief Resets Vulkan semaphores.
 * 
 * This function resets a specified number of Vulkan semaphores to their initial state.
 * 
 * @param device Valid Vulkan device.
 * @param semaphore_count Number of semaphores to reset.
 * @param p_semaphores Valid pointer to an array of Vulkan semaphores to reset.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shResetSemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores
);

/**
 * @brief Waits for Vulkan fences to become signaled.
 * 
 * This function waits for a specified number of Vulkan fences to become signaled.
 * 
 * @param device Valid Vulkan device.
 * @param fence_count Number of fences to wait for.
 * @param p_fences Valid pointer to an array of Vulkan fences to wait for.
 * @param wait_for_all Whether to wait for all fences to be signaled.
 * @param timeout_ns Timeout in nanoseconds to wait for the fences.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shWaitForFences(
	VkDevice device,
	uint32_t fence_count,
	VkFence* p_fences,
	uint8_t  wait_for_all,
	uint64_t timeout_ns
);

/**
 * @brief Waits for Vulkan semaphores to become signaled.
 * 
 * This function waits for a specified number of Vulkan semaphores to become signaled.
 * 
 * @param device Valid Vulkan device.
 * @param semaphore_count Number of semaphores to wait for.
 * @param p_semaphores Valid pointer to an array of Vulkan semaphores to wait for.
 * @param wait_for_all Whether to wait for all semaphores to be signaled.
 * @param timeout_ns Timeout in nanoseconds to wait for the semaphores.
 * @param p_semaphores_values Valid destination pointer to an array of values for each semaphore.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shWaitForSemaphores(
	VkDevice     device,
	uint32_t     semaphore_count,
	VkSemaphore* p_semaphores,
	uint8_t      wait_for_all,
	uint64_t     timeout_ns,
	uint64_t*    p_semaphores_values
);

/**
 * @brief Acquires an image from the Vulkan swapchain.
 * 
 * This function acquires an image from the specified swapchain and optionally signals a semaphore or fence.
 * 
 * @param device Valid Vulkan device.
 * @param swapchain Valid Vulkan swapchain from which to acquire an image.
 * @param timeout_ns Timeout in nanoseconds to wait for the image.
 * @param acquired_signal_semaphore Valid Vulkan semaphore to signal upon acquisition.
 * @param acquired_signal_fence Valid Vulkan fence to signal upon acquisition.
 * @param p_swapchain_image_index Valid destination pointer to the index of the acquired swapchain image.
 * @param p_swapchain_suboptimal Valid destination pointer to a flag indicating if the swapchain is suboptimal.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shAcquireSwapchainImage(
	VkDevice       device,
	VkSwapchainKHR swapchain,
	uint64_t       timeout_ns,
	VkSemaphore    acquired_signal_semaphore,
	VkFence        acquired_signal_fence,
	uint32_t*      p_swapchain_image_index,
	uint8_t*       p_swapchain_suboptimal
);

/**
 * @brief Begins a Vulkan render pass.
 * 
 * This function begins recording commands for a render pass in the specified command buffer.
 * 
 * @param graphics_cmd_buffer Valid Vulkan command buffer to begin the render pass.
 * @param renderpass Valid Vulkan render pass to begin.
 * @param render_offset_x X offset for the render area.
 * @param render_offset_y Y offset for the render area.
 * @param render_size_x Width of the render area.
 * @param render_size_y Height of the render area.
 * @param clear_value_count Number of clear values.
 * @param p_clear_values Valid pointer to an array of Vulkan clear values.
 * @param framebuffer Valid Vulkan framebuffer to use for the render pass.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Ends a Vulkan render pass.
 * 
 * This function ends the recording of commands for a render pass in the specified command buffer.
 * 
 * @param graphics_cmd_buffer Valid Vulkan command buffer to end the render pass.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shEndRenderpass(
	VkCommandBuffer graphics_cmd_buffer
);

/**
 * @brief Records a draw command into a Vulkan command buffer.
 * 
 * This function records a draw command into the specified command buffer.
 * 
 * @param graphics_cmd_buffer Valid Vulkan command buffer to record the draw command.
 * @param vertex_count Number of vertices to draw.
 * @param first_vertex Index of the first vertex to draw.
 * @param instance_count Number of instances to draw.
 * @param first_instance Index of the first instance to draw.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDraw(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        vertex_count,
	uint32_t        first_vertex,
	uint32_t        instance_count,
	uint32_t        first_instance
);

/**
 * @brief Records an indexed draw command into a Vulkan command buffer.
 * 
 * This function records an indexed draw command into the specified command buffer.
 * 
 * @param graphics_cmd_buffer Valid Vulkan command buffer to record the draw command.
 * @param index_count Number of indices to draw.
 * @param instance_count Number of instances to draw.
 * @param first_index Index of the first index to draw.
 * @param vertex_offset Offset added to each index to obtain the vertex index.
 * @param first_instance Index of the first instance to draw.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDrawIndexed(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        index_count,
	uint32_t        instance_count,
	uint32_t        first_index,
	int32_t         vertex_offset,
	uint32_t        first_instance
);

/**
 * @brief Presents an image from a Vulkan swapchain to the screen.
 * 
 * This function presents an image from the specified swapchain using the provided queue.
 * 
 * @param present_queue Valid Vulkan queue to present the image.
 * @param semaphores_to_wait_for_count Number of semaphores to wait for before presenting.
 * @param p_semaphores_to_wait_for Valid pointer to an array of Vulkan semaphores to wait for.
 * @param swapchain Valid Vulkan swapchain containing the image to present.
 * @param swapchain_image_idx Index of the swapchain image to present.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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


/**
 * @brief Creates a Vulkan buffer.
 * 
 * This function creates a Vulkan buffer with the specified size and usage.
 * 
 * @param device Valid Vulkan device.
 * @param size Size of the buffer in bytes.
 * @param usage Vulkan buffer usage flags indicating how the buffer will be used.
 * @param sharing_mode Vulkan sharing mode specifying how the buffer is shared between queues.
 * @param p_buffer Valid destination pointer to the newly created Vulkan buffer.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateBuffer(
	VkDevice           device,
	uint32_t           size,
	VkBufferUsageFlags usage,
	VkSharingMode      sharing_mode,
	VkBuffer*          p_buffer
);

/**
 * @brief Retrieves the memory type index that supports the specified memory properties.
 * 
 * This function finds the index of a memory type that matches the specified memory property flags.
 * 
 * @param device Valid Vulkan device.
 * @param physical_device Valid Vulkan physical device.
 * @param property_flags Vulkan memory property flags to match.
 * @param p_memory_type_index Valid destination pointer to the index of the memory type.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shGetMemoryType(
	VkDevice              device,
	VkPhysicalDevice      physical_device,
	VkMemoryPropertyFlags property_flags,
	uint32_t*             p_memory_type_index
);

/**
 * @brief Allocates memory for a Vulkan buffer.
 * 
 * This function allocates memory for a specified Vulkan buffer with the desired memory properties.
 * 
 * @param device Valid Vulkan device.
 * @param physical_device Valid Vulkan physical device.
 * @param buffer Valid Vulkan buffer for which memory is to be allocated.
 * @param property_flags Vulkan memory property flags for the allocated memory.
 * @param p_memory Valid destination pointer to the newly allocated Vulkan device memory.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shAllocateBufferMemory(
	VkDevice              device,
	VkPhysicalDevice      physical_device,
	VkBuffer              buffer,
	VkMemoryPropertyFlags property_flags,
	VkDeviceMemory*       p_memory
);

/**
 * @brief Copies data from one Vulkan buffer to another.
 * 
 * This function copies data from a source buffer to a destination buffer.
 * 
 * @param transfer_cmd_buffer Valid Vulkan command buffer for the copy operation.
 * @param src_buffer Valid Vulkan source buffer.
 * @param src_offset Offset in the source buffer to start copying from.
 * @param dst_offset Offset in the destination buffer to start copying to.
 * @param size Number of bytes to copy.
 * @param dst_buffer Valid Vulkan destination buffer.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCopyBuffer(
	VkCommandBuffer transfer_cmd_buffer,
	VkBuffer        src_buffer,
	uint32_t        src_offset,
	uint32_t        dst_offset,
	uint32_t        size,
	VkBuffer        dst_buffer
);

#define SH_MAX_STACK_BUFFER_REGION_COUNT 256

/**
 * @brief Copies multiple regions of data from one Vulkan buffer to another.
 * 
 * This function copies multiple regions of data from a source buffer to a destination buffer.
 * 
 * @param transfer_cmd_buffer Valid Vulkan command buffer for the copy operation.
 * @param src_buffer Valid Vulkan source buffer.
 * @param region_count Number of regions to copy.
 * @param p_src_offsets Valid pointer to an array of offsets in the source buffer.
 * @param p_dst_offsets Valid pointer to an array of offsets in the destination buffer.
 * @param p_sizes Valid pointer to an array of sizes for each region to copy.
 * @param dst_buffer Valid Vulkan destination buffer.
 * 
 * @return 1 if successful, 0 otherwise.
 */
uint8_t shCopyBufferRegions(
	VkCommandBuffer transfer_cmd_buffer,
	VkBuffer        src_buffer,
	uint32_t        region_count,
	uint32_t*       p_src_offsets,
	uint32_t*       p_dst_offsets,
	uint32_t*       p_sizes,
	VkBuffer        dst_buffer
);

/**
 * @brief Binds a Vulkan buffer to a specified memory offset.
 * 
 * This function binds a Vulkan buffer to a specific memory offset.
 * 
 * @param device Valid Vulkan device.
 * @param buffer Valid Vulkan buffer to bind.
 * @param offset Offset in the memory where the buffer will be bound.
 * @param buffer_memory Valid Vulkan device memory to bind to the buffer.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shBindBufferMemory(
	VkDevice       device,
	VkBuffer       buffer,
	uint32_t       offset,
	VkDeviceMemory buffer_memory
);

/**
 * @brief Reads data from a Vulkan memory object.
 * 
 * This function reads data from a specified offset in a Vulkan memory object into a user-provided buffer.
 * 
 * @param device Valid Vulkan device.
 * @param memory Valid Vulkan device memory to read from.
 * @param offset Offset in the memory to start reading from.
 * @param data_size Number of bytes to read.
 * @param p_data Valid destination pointer to a buffer to store the read data.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shReadMemory(
	VkDevice       device,
	VkDeviceMemory memory,
	uint32_t       offset,
	uint32_t       data_size,
	void*          p_data
);

/**
 * @brief Writes data to a Vulkan memory object.
 * 
 * This function writes data from a user-provided buffer to a specified offset in a Vulkan memory object.
 * 
 * @param device Valid Vulkan device.
 * @param memory Valid Vulkan device memory to write to.
 * @param offset Offset in the memory to start writing to.
 * @param data_size Number of bytes to write.
 * @param p_data Valid pointer to a buffer containing the data to write.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shWriteMemory(
	VkDevice       device,
	VkDeviceMemory memory,
	uint32_t       offset,
	uint32_t       data_size,
	void*          p_data
);

/**
 * @brief Clears memory associated with a Vulkan buffer.
 * 
 * This function clears the memory bound to a Vulkan buffer.
 * 
 * @param device Valid Vulkan device.
 * @param buffer Valid Vulkan buffer.
 * @param memory Valid Vulkan device memory bound to the buffer.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shClearBufferMemory(
	VkDevice       device,
	VkBuffer       buffer,
	VkDeviceMemory memory
);

/**
 * @brief Creates a Vulkan image.
 * 
 * This function creates a Vulkan image with the specified parameters.
 * 
 * @param device Valid Vulkan device.
 * @param type Vulkan image type.
 * @param x Width of the image.
 * @param y Height of the image.
 * @param z Depth of the image.
 * @param format Vulkan image format.
 * @param mip_levels Number of mipmap levels.
 * @param sample_count Vulkan sample count flag bits.
 * @param image_tiling Vulkan image tiling mode.
 * @param usage Vulkan image usage flags.
 * @param sharing_mode Vulkan sharing mode.
 * @param p_image Valid destination pointer to the newly created Vulkan image.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Allocates memory for a Vulkan image.
 * 
 * This function allocates memory for a specified Vulkan image with the desired memory properties.
 * 
 * @param device Valid Vulkan device.
 * @param physical_device Valid Vulkan physical device.
 * @param image Valid Vulkan image for which memory is to be allocated.
 * @param memory_property_flags Vulkan memory property flags for the allocated memory.
 * @param p_image_memory Valid destination pointer to the newly allocated Vulkan device memory.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shAllocateImageMemory(
	VkDevice              device,
	VkPhysicalDevice      physical_device,
	VkImage               image,
	VkMemoryPropertyFlags memory_property_flags,
	VkDeviceMemory*       p_image_memory
);

/**
 * @brief Binds a Vulkan image to a specified memory offset.
 * 
 * This function binds a Vulkan image to a specific memory offset.
 * 
 * @param device Valid Vulkan device.
 * @param image Valid Vulkan image to bind.
 * @param offset Offset in the memory where the image will be bound.
 * @param image_memory Valid Vulkan device memory to bind to the image.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shBindImageMemory(
	VkDevice       device,
	VkImage        image,
	uint32_t       offset,
	VkDeviceMemory image_memory
);

/**
 * @brief Clears memory associated with a Vulkan image.
 * 
 * This function clears the memory bound to a Vulkan image.
 * 
 * @param device Valid Vulkan device.
 * @param image Valid Vulkan image.
 * @param image_memory Valid Vulkan device memory bound to the image.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shClearImageMemory(
	VkDevice       device,
	VkImage        image,
	VkDeviceMemory image_memory
);

/**
 * @brief Retrieves memory budget properties for a Vulkan physical device.
 * 
 * This function retrieves memory budget properties for a Vulkan physical device.
 * 
 * @param physical_device Valid Vulkan physical device.
 * @param p_memory_budget_properties Valid destination pointer to Vulkan memory budget properties.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shGetMemoryBudgetProperties(
	VkPhysicalDevice                           physical_device,
	VkPhysicalDeviceMemoryBudgetPropertiesEXT* p_memory_budget_properties
);

/**
 * @brief Binds vertex buffers to a Vulkan command buffer.
 * 
 * This function binds one or more vertex buffers to a Vulkan command buffer.
 * 
 * @param graphics_cmd_buffer Valid Vulkan command buffer to bind vertex buffers to.
 * @param first_binding Binding index to start from.
 * @param binding_count Number of vertex buffers to bind.
 * @param p_vertex_buffers Valid pointer to an array of Vulkan vertex buffers.
 * @param p_vertex_offsets Valid pointer to an array of offsets for each vertex buffer.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shBindVertexBuffers(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        first_binding,
	uint32_t        binding_count,
	VkBuffer*       p_vertex_buffers,
	VkDeviceSize*   p_vertex_offsets
);

/**
 * @brief Binds an index buffer to a Vulkan command buffer.
 * 
 * This function binds an index buffer to a Vulkan command buffer.
 * 
 * @param graphics_cmd_buffer Valid Vulkan command buffer to bind the index buffer to.
 * @param index_offset Offset in the index buffer.
 * @param index_buffer Valid Vulkan index buffer to bind.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shBindIndexBuffer(
	VkCommandBuffer graphics_cmd_buffer,
	uint32_t        index_offset,
	VkBuffer        index_buffer
);

/**
 * @brief Sets the vertex input binding description.
 * 
 * This function sets the vertex input binding description with the specified parameters.
 * 
 * @param binding Binding index for the vertex input binding.
 * @param size Size of the vertex buffer in bytes.
 * @param input_rate Vertex input rate (e.g., per vertex or per instance).
 * @param p_vertex_input_binding Valid destination pointer to the Vulkan vertex input binding description.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shSetVertexBinding(
	uint32_t                           binding,
	uint32_t                           size,
	VkVertexInputRate                  input_rate,
	VkVertexInputBindingDescription*   p_vertex_input_binding
);

/**
 * @brief Sets the vertex attribute description.
 * 
 * This function sets the vertex attribute description with the specified parameters.
 * 
 * @param location Location of the vertex attribute.
 * @param binding Binding index of the vertex attribute.
 * @param format Vulkan format of the vertex attribute.
 * @param offset Offset in the vertex buffer where the attribute starts.
 * @param p_vertex_input_attribute Valid destination pointer to the Vulkan vertex input attribute description.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shSetVertexAttribute(
	uint32_t                           location,
	uint32_t                           binding,
	VkFormat                           format,
	uint32_t                           offset,
	VkVertexInputAttributeDescription* p_vertex_input_attribute
);

/**
 * @brief Sets the vertex input state for a pipeline.
 * 
 * This function sets the vertex input state, including bindings and attributes, for a Vulkan pipeline.
 * 
 * @param vertex_binding_count Number of vertex input bindings.
 * @param p_vertex_bindings Valid pointer to an array of Vulkan vertex input binding descriptions.
 * @param vertex_attribute_count Number of vertex input attributes.
 * @param p_vertex_attributes Valid pointer to an array of Vulkan vertex input attribute descriptions.
 * @param p_vertex_input_state Valid destination pointer to the Vulkan pipeline vertex input state create info.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shSetVertexInputState(
	uint32_t                              vertex_binding_count,
	VkVertexInputBindingDescription*      p_vertex_bindings,
	uint32_t                              vertex_attribute_count,
	VkVertexInputAttributeDescription*    p_vertex_attributes,
	VkPipelineVertexInputStateCreateInfo* p_vertex_input_state
);

/**
 * @brief Creates the input assembly state for a pipeline.
 * 
 * This function creates the input assembly state with the specified topology and restart enable flag.
 * 
 * @param primitive_topology Vulkan primitive topology type.
 * @param primitive_restart_enable Boolean flag to enable or disable primitive restart.
 * @param p_input_assembly Valid destination pointer to the Vulkan pipeline input assembly state create info.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateInputAssembly(
	VkPrimitiveTopology                     primitive_topology,
	VkBool32                                primitive_restart_enable,
	VkPipelineInputAssemblyStateCreateInfo* p_input_assembly
);

/**
 * @brief Creates the rasterization state for a pipeline.
 * 
 * This function creates the rasterization state with the specified polygon mode and cull mode.
 * 
 * @param polygon_mode Vulkan polygon mode for rasterization.
 * @param cull_mode Vulkan cull mode flag bits.
 * @param p_rasterizer Valid destination pointer to the Vulkan pipeline rasterization state create info.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateRasterizer(
	VkPolygonMode                           polygon_mode,
	VkCullModeFlagBits                      cull_mode,
	VkPipelineRasterizationStateCreateInfo* p_rasterizer
);

/**
 * @brief Sets the multisample state for a pipeline.
 * 
 * This function sets the multisample state, including sample count and minimum sample shading, for a Vulkan pipeline.
 * 
 * @param sample_count Vulkan sample count flag bits.
 * @param min_sample_shading_size Minimum sample shading size.
 * @param p_multisample_state Valid destination pointer to the Vulkan pipeline multisample state create info.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shSetMultisampleState(
	VkSampleCountFlagBits                 sample_count,
	float                                 min_sample_shading_size,
	VkPipelineMultisampleStateCreateInfo* p_multisample_state
);

/**
 * @brief Sets the viewport and scissor state for a pipeline.
 * 
 * This function sets the viewport and scissor parameters for a Vulkan pipeline.
 * 
 * @param viewport_pos_x X position of the viewport.
 * @param viewport_pos_y Y position of the viewport.
 * @param viewport_width Width of the viewport.
 * @param viewport_height Height of the viewport.
 * @param p_viewport Valid destination pointer to the Vulkan viewport.
 * @param scissors_pos_x X position of the scissor.
 * @param scissors_pos_y Y position of the scissor.
 * @param scissors_width Width of the scissor.
 * @param scissors_height Height of the scissor.
 * @param p_scissors Valid destination pointer to the Vulkan scissor rectangle.
 * @param p_viewport_state Valid destination pointer to the Vulkan pipeline viewport state create info.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Configures color blending and alpha blending states.
 * 
 * This function configures the color blending and alpha blending states for a Vulkan pipeline.
 * 
 * @param enable_color_blending Boolean flag to enable or disable color blending.
 * @param enable_alpha_blending Boolean flag to enable or disable alpha blending.
 * @param subpass_color_attachment_count Number of color attachments in the subpass.
 * @param p_color_blend_attachment_states Valid pointer to an array of Vulkan color blend attachment states.
 * @param p_color_blend_state Valid destination pointer to the Vulkan pipeline color blend state create info.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shColorBlendSettings(
	uint8_t                              enable_color_blending,
	uint8_t                              enable_alpha_blending,
	uint32_t                             subpass_color_attachment_count,
	VkPipelineColorBlendAttachmentState* p_color_blend_attachment_states,
	VkPipelineColorBlendStateCreateInfo* p_color_blend_state
);

/**
 * @brief Creates a Vulkan shader module.
 * 
 * This function creates a shader module from the provided shader code.
 * 
 * @param device Valid Vulkan device.
 * @param size Size of the shader code in bytes.
 * @param code Pointer to the shader code.
 * @param p_shader_module Valid destination pointer to the newly created Vulkan shader module.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateShaderModule(
	VkDevice        device,
	uint32_t        size,
	char*           code,
	VkShaderModule* p_shader_module
);

/**
 * @brief Creates a Vulkan shader stage.
 * 
 * This function creates a shader stage using the provided shader module and stage flag.
 * 
 * @param shader_module Valid Vulkan shader module.
 * @param shader_stage_flag Vulkan shader stage flag (e.g., vertex, fragment).
 * @param p_shader_stage Valid destination pointer to the Vulkan pipeline shader stage create info.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateShaderStage(
	VkShaderModule                   shader_module,
	VkShaderStageFlags               shader_stage_flag,
	VkPipelineShaderStageCreateInfo* p_shader_stage
);

/**
 * @brief Sets push constants for a shader stage.
 * 
 * This function sets push constants for a specified shader stage.
 * 
 * @param shader_stage Vulkan shader stage flags (e.g., vertex, fragment).
 * @param offset Offset in bytes from the start of the push constant range.
 * @param size Size of the push constants in bytes.
 * @param p_push_constant_range Valid destination pointer to the Vulkan push constant range.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shSetPushConstants(
	VkShaderStageFlags   shader_stage,
	uint32_t             offset,
	uint32_t             size,
	VkPushConstantRange* p_push_constant_range
);

/**
 * @brief Creates a descriptor set layout binding.
 * 
 * This function creates a descriptor set layout binding with the specified parameters.
 * 
 * @param binding Binding index for the descriptor set layout.
 * @param descriptor_type Vulkan descriptor type (e.g., uniform buffer, sampled image).
 * @param descriptor_set_count Number of descriptors in the set.
 * @param shader_stage Vulkan shader stage flags that use this descriptor.
 * @param p_binding Valid destination pointer to the Vulkan descriptor set layout binding.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateDescriptorSetLayoutBinding(
	uint32_t                      binding,
	VkDescriptorType              descriptor_type,
	uint32_t                      descriptor_set_count,
	VkShaderStageFlags            shader_stage,
	VkDescriptorSetLayoutBinding* p_binding
);

/**
 * @brief Creates a descriptor set layout.
 * 
 * This function creates a descriptor set layout with the specified bindings.
 * 
 * @param device Valid Vulkan device.
 * @param binding_count Number of descriptor set layout bindings.
 * @param p_bindings Valid pointer to an array of Vulkan descriptor set layout bindings.
 * @param p_descriptor_set_layout Valid destination pointer to the newly created Vulkan descriptor set layout.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateDescriptorSetLayout(
	VkDevice                      device,
	uint32_t                      binding_count,
	VkDescriptorSetLayoutBinding* p_bindings,
	VkDescriptorSetLayout*        p_descriptor_set_layout
);

/**
 * @brief Creates a descriptor pool.
 * 
 * This function creates a descriptor pool with the specified pool sizes.
 * 
 * @param device Valid Vulkan device.
 * @param pool_size_count Number of pool sizes.
 * @param p_pool_sizes Valid pointer to an array of Vulkan descriptor pool sizes.
 * @param p_descriptor_pool Valid destination pointer to the newly created Vulkan descriptor pool.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreateDescriptorPool(
	VkDevice              device,
	uint32_t              pool_size_count,
	VkDescriptorPoolSize* p_pool_sizes,
	VkDescriptorPool*     p_descriptor_pool
);

/**
 * @brief Sets buffer information for a descriptor buffer.
 * 
 * This function sets buffer information for a descriptor buffer.
 * 
 * @param buffer Valid Vulkan buffer.
 * @param buffer_offset Offset in the buffer.
 * @param buffer_size Size of the buffer.
 * @param p_buffer_info Valid destination pointer to the Vulkan descriptor buffer info.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shSetDescriptorBufferInfo(
	VkBuffer                buffer,
	uint32_t                buffer_offset,
	uint32_t                buffer_size,
	VkDescriptorBufferInfo* p_buffer_info
);

/**
 * @brief Allocates descriptor sets from a descriptor pool.
 * 
 * This function allocates descriptor sets of the specified type from the given descriptor pool.
 * 
 * @param device Valid Vulkan device.
 * @param descriptor_pool Valid Vulkan descriptor pool from which to allocate descriptor sets.
 * @param descriptor_type Vulkan descriptor type (e.g., uniform buffer, sampled image).
 * @param binding Binding index for the descriptor sets.
 * @param descriptor_set_unit_count Number of descriptor sets to allocate.
 * @param p_descriptor_set_layouts Valid pointer to an array of Vulkan descriptor set layouts.
 * @param p_descriptor_sets Valid destination pointer to an array of Vulkan descriptor sets.
 * @param p_buffer_infos Valid pointer to an array of Vulkan descriptor buffer info structures.
 * @param p_write_descriptor_sets Valid pointer to an array of Vulkan write descriptor set structures.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Creates a pipeline layout.
 * 
 * This function creates a Vulkan pipeline layout with the specified push constant ranges and descriptor set layouts.
 * 
 * @param device Valid Vulkan device.
 * @param push_constant_range_count Number of push constant ranges.
 * @param p_push_constants_range Valid pointer to an array of Vulkan push constant ranges.
 * @param src_descriptor_set_layout_count Number of descriptor set layouts.
 * @param p_src_descriptor_set_layouts Valid pointer to an array of Vulkan descriptor set layouts.
 * @param p_pipeline_layout Valid destination pointer to the newly created Vulkan pipeline layout.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shCreatePipelineLayout(
	VkDevice               device,
	uint32_t               push_constant_range_count,
	VkPushConstantRange*   p_push_constants_range,
	uint32_t               src_descriptor_set_layout_count,
	VkDescriptorSetLayout* p_src_descriptor_set_layouts,
	VkPipelineLayout*      p_pipeline_layout
);

/**
 * @brief Destroys a Vulkan descriptor pool.
 * 
 * This function destroys the specified Vulkan descriptor pool.
 * 
 * @param device Valid Vulkan device.
 * @param descriptor_pool Vulkan descriptor pool to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyDescriptorPool(
	VkDevice         device,
	VkDescriptorPool descriptor_pool
);

/**
 * @brief Destroys a Vulkan descriptor set layout.
 * 
 * This function destroys the specified Vulkan descriptor set layout.
 * 
 * @param device Valid Vulkan device.
 * @param descriptor_set_layout Vulkan descriptor set layout to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyDescriptorSetLayout(
	VkDevice              device,
	VkDescriptorSetLayout descriptor_set_layout
);

/**
 * @brief Destroys a Vulkan shader module.
 * 
 * This function destroys the specified Vulkan shader module.
 * 
 * @param device Valid Vulkan device.
 * @param shader_module Vulkan shader module to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyShaderModule(
	VkDevice       device,
	VkShaderModule shader_module
);

/**
 * @brief Destroys a Vulkan pipeline layout.
 * 
 * This function destroys the specified Vulkan pipeline layout.
 * 
 * @param device Valid Vulkan device.
 * @param pipeline_layout Vulkan pipeline layout to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyPipelineLayout(
	VkDevice         device,
	VkPipelineLayout pipeline_layout
);

/**
 * @brief Destroys a Vulkan pipeline.
 * 
 * This function destroys the specified Vulkan pipeline.
 * 
 * @param device Valid Vulkan device.
 * @param pipeline Vulkan pipeline to destroy.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shDestroyPipeline(
	VkDevice   device,
	VkPipeline pipeline
);



#define SH_MAX_PIPELINE_VERTEX_BINDING_COUNT           32
#define SH_MAX_PIPELINE_VERTEX_ATTRIBUTE_COUNT         32
											           
#define SH_MAX_PIPELINE_SHADER_STAGE_COUNT             6

#define SH_MAX_PIPELINE_SUBPASS_COLOR_ATTACHMENT_COUNT 9


/**
 * @brief Structure representing a Vulkan pipeline.
 * 
 * This structure holds all the necessary state and configuration for creating and managing a Vulkan pipeline.
 */
typedef struct ShVkPipeline {
	/*Vertex inputs*/
	uint32_t								vertex_binding_count; ///< Number of vertex bindings.
	VkVertexInputBindingDescription			vertex_bindings[SH_MAX_PIPELINE_VERTEX_BINDING_COUNT]; ///< Array of vertex input bindings.
	uint32_t								vertex_attribute_count; ///< Number of vertex attributes.
	VkVertexInputAttributeDescription		vertex_attributes[SH_MAX_PIPELINE_VERTEX_ATTRIBUTE_COUNT]; ///< Array of vertex input attributes.
	VkPipelineVertexInputStateCreateInfo	vertex_input_state_info; ///< Vertex input state information.
	VkPipelineInputAssemblyStateCreateInfo	input_assembly; ///< Input assembly state information.
	/*Shaders*/
	uint32_t                                shader_module_count; ///< Number of shader modules.
	VkShaderModule                          shader_modules[SH_MAX_PIPELINE_SHADER_STAGE_COUNT]; ///< Array of shader modules.
	uint32_t                                shader_stage_count; ///< Number of shader stages.
	VkPipelineShaderStageCreateInfo         shader_stages[SH_MAX_PIPELINE_SHADER_STAGE_COUNT]; ///< Array of shader stage create info.
	/*Push constants*/
	VkPushConstantRange                     push_constant_range; ///< Push constant range information.
	/*Rasterizer*/
	VkPipelineRasterizationStateCreateInfo	rasterizer; ///< Rasterizer state information.
	/*Viewport*/
	VkViewport								viewport; ///< Viewport information.
	VkRect2D								scissors; ///< Scissor rectangle information.
	VkPipelineViewportStateCreateInfo		viewport_state; ///< Viewport state information.
	/*Color blending*/
	VkPipelineColorBlendAttachmentState		color_blend_attachment_states[SH_MAX_PIPELINE_SUBPASS_COLOR_ATTACHMENT_COUNT]; ///< Array of color blend attachment states.
	VkPipelineColorBlendStateCreateInfo		color_blend_state; ///< Color blend state information.
	/*Multisample state*/
	VkPipelineMultisampleStateCreateInfo	multisample_state_info; ///< Multisample state information.
	/*Pipeline*/
	VkPipelineLayout                        pipeline_layout; ///< Vulkan pipeline layout.
	VkPipeline                              pipeline; ///< Vulkan pipeline.
} ShVkPipeline;



typedef struct ShVkPipelinePool ShVkPipelinePool;



/**
 * @brief Clears the pipeline structure.
 * 
 * This function resets all fields of the ShVkPipeline structure to their default values.
 * 
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to be cleared.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shClearPipeline(
	ShVkPipeline* p_pipeline
);

/**
 * @brief Sets vertex binding information in the pipeline.
 * 
 * This function sets the vertex binding description for a specific binding index in the ShVkPipeline structure.
 * 
 * @param binding Binding index.
 * @param size Size of the vertex binding.
 * @param input_rate Vertex input rate.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineSetVertexBinding(
	uint32_t          binding,
	uint32_t          size,
	VkVertexInputRate input_rate,
	ShVkPipeline*     p_pipeline
);

/**
 * @brief Sets vertex attribute information in the pipeline.
 * 
 * This function sets the vertex attribute description for a specific location in the ShVkPipeline structure.
 * 
 * @param location Location index.
 * @param binding Binding index.
 * @param format Vertex attribute format.
 * @param offset Offset of the vertex attribute.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineSetVertexAttribute(
	uint32_t      location,
	uint32_t      binding,
	VkFormat      format,
	uint32_t      offset,
	ShVkPipeline* p_pipeline
);

/**
 * @brief Sets vertex input state information in the pipeline.
 * 
 * This function sets the vertex input state information in the ShVkPipeline structure.
 * 
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineSetVertexInputState(
	ShVkPipeline* p_pipeline
);

/**
 * @brief Creates input assembly state information for the pipeline.
 * 
 * This function sets the input assembly state information in the ShVkPipeline structure.
 * 
 * @param primitive_topology Primitive topology used in the input assembly.
 * @param primitive_restart_enable Flag indicating if primitive restart is enabled.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineCreateInputAssembly(
	VkPrimitiveTopology primitive_topology,
	VkBool32            primitive_restart_enable,
	ShVkPipeline*       p_pipeline
);

/**
 * @brief Creates rasterizer state information for the pipeline.
 * 
 * This function sets the rasterizer state information in the ShVkPipeline structure.
 * 
 * @param polygon_mode Polygon mode used in rasterization.
 * @param cull_mode Culling mode used in rasterization.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineCreateRasterizer(
	VkPolygonMode      polygon_mode,
	VkCullModeFlagBits cull_mode,
	ShVkPipeline*      p_pipeline
);

/**
 * @brief Sets multisample state information in the pipeline.
 * 
 * This function sets the multisample state information in the ShVkPipeline structure.
 * 
 * @param sample_count Number of samples used in multisampling.
 * @param min_sample_shading_size Minimum sample shading size.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineSetMultisampleState(
	VkSampleCountFlagBits sample_count,
	float                 min_sample_shading_size,
	ShVkPipeline*         p_pipeline
);

/**
 * @brief Sets viewport and scissor information in the pipeline.
 * 
 * This function sets the viewport and scissor information in the ShVkPipeline structure.
 * 
 * @param viewport_pos_x X position of the viewport.
 * @param viewport_pos_y Y position of the viewport.
 * @param viewport_width Width of the viewport.
 * @param viewport_height Height of the viewport.
 * @param scissors_pos_x X position of the scissors rectangle.
 * @param scissors_pos_y Y position of the scissors rectangle.
 * @param scissors_width Width of the scissors rectangle.
 * @param scissors_height Height of the scissors rectangle.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Configures color blending settings in the pipeline.
 * 
 * This function sets color blending and alpha blending settings in the ShVkPipeline structure.
 * 
 * @param enable_color_blending Flag indicating if color blending is enabled.
 * @param enable_alpha_blending Flag indicating if alpha blending is enabled.
 * @param subpass_color_attachment_count Number of color attachments for the subpass.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineColorBlendSettings(
	uint8_t       enable_color_blending,
	uint8_t       enable_alpha_blending,
	uint32_t      subpass_color_attachment_count,
	ShVkPipeline* p_pipeline
);

/**
 * @brief Creates a shader module and adds it to the pipeline.
 * 
 * This function creates a Vulkan shader module and updates the ShVkPipeline structure.
 * 
 * @param device Valid Vulkan device.
 * @param size Size of the shader code.
 * @param code Pointer to the shader code.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineCreateShaderModule(
	VkDevice      device,
	uint32_t      size,
	char*         code,
	ShVkPipeline* p_pipeline
);

/**
 * @brief Creates a shader stage and adds it to the pipeline.
 * 
 * This function creates a Vulkan shader stage and updates the ShVkPipeline structure.
 * 
 * @param shader_stage Vulkan shader stage flags (e.g., vertex, fragment).
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineCreateShaderStage(
	VkShaderStageFlags shader_stage,
	ShVkPipeline*      p_pipeline
);

/**
 * @brief Sets push constants in the pipeline.
 * 
 * This function sets the push constant range for a specific shader stage in the ShVkPipeline structure.
 * 
 * @param shader_stage Vulkan shader stage flags (e.g., vertex, fragment).
 * @param offset Offset of the push constants.
 * @param size Size of the push constants.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineSetPushConstants(
	VkShaderStageFlags shader_stage,
	uint32_t           offset,
	uint32_t           size,
	ShVkPipeline*      p_pipeline
);

/**
 * @brief Creates a pipeline layout and updates the pipeline structure.
 * 
 * This function creates a Vulkan pipeline layout using the specified descriptor set layouts and updates the ShVkPipeline structure.
 * 
 * @param device Valid Vulkan device.
 * @param first_descriptor_set_layout Index of the first descriptor set layout.
 * @param descriptor_set_layout_count Number of descriptor set layouts.
 * @param p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to update.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineCreateLayout(
	VkDevice          device,
	uint32_t          first_descriptor_set_layout,
	uint32_t          descriptor_set_layout_count,
	ShVkPipelinePool* p_pipeline_pool,
	ShVkPipeline*     p_pipeline
);

/**
 * @brief Sets up a compute pipeline.
 * 
 * This function sets up a Vulkan compute pipeline using the ShVkPipeline structure.
 * 
 * @param device Valid Vulkan device.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to setup.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shSetupComputePipeline(
	VkDevice      device,
	ShVkPipeline* p_pipeline
);

/**
 * @brief Sets up a graphics pipeline.
 * 
 * This function sets up a Vulkan graphics pipeline using the ShVkPipeline structure and the specified render pass.
 * 
 * @param device Valid Vulkan device.
 * @param render_pass Valid Vulkan render pass.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to setup.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shSetupGraphicsPipeline(
	VkDevice      device,
	VkRenderPass  render_pass,
	ShVkPipeline* p_pipeline
);

/**
 * @brief Pushes constants to a pipeline using the command buffer.
 * 
 * This function updates the push constants for a pipeline within a command buffer.
 * 
 * @param cmd_buffer Valid Vulkan command buffer.
 * @param p_data Pointer to the data to be pushed to the constants.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure containing push constants information.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelinePushConstants(
	VkCommandBuffer cmd_buffer,
	void*           p_data,
	ShVkPipeline*   p_pipeline
);

/**
 * @brief Binds a pipeline to a command buffer.
 * 
 * This function binds a pipeline (graphics or compute) to the command buffer for subsequent commands.
 * 
 * @param cmd_buffer Valid Vulkan command buffer.
 * @param bind_point Pipeline bind point (e.g., VK_PIPELINE_BIND_POINT_GRAPHICS).
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure to be bound.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shBindPipeline(
	VkCommandBuffer     cmd_buffer,
	VkPipelineBindPoint bind_point,
	ShVkPipeline*       p_pipeline
);

/**
 * @brief Binds descriptor set units to a pipeline within a command buffer.
 * 
 * This function binds descriptor set units to a pipeline and command buffer, specifying dynamic offsets if needed.
 * 
 * @param cmd_buffer Valid Vulkan command buffer.
 * @param first_descriptor_set Index of the first descriptor set.
 * @param first_descriptor_set_unit_idx Index of the first descriptor set unit.
 * @param descriptor_set_unit_count Number of descriptor set units to bind.
 * @param bind_point Pipeline bind point (e.g., VK_PIPELINE_BIND_POINT_GRAPHICS).
 * @param dynamic_descriptors_count Number of dynamic descriptors.
 * @param p_dynamic_offsets Array of dynamic offsets for the descriptors.
 * @param p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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

/**
 * @brief Destroys shader modules associated with the pipeline.
 * 
 * This function destroys a range of shader modules that are part of the ShVkPipeline structure.
 * 
 * @param device Valid Vulkan device.
 * @param first_module Index of the first shader module to destroy.
 * @param module_count Number of shader modules to destroy.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineDestroyShaderModules(
	VkDevice      device,
	uint32_t      first_module,
	uint32_t      module_count,
	ShVkPipeline* p_pipeline
);

/**
 * @brief Destroys the pipeline layout associated with the pipeline.
 * 
 * This function destroys the pipeline layout used by the ShVkPipeline structure.
 * 
 * @param device Valid Vulkan device.
 * @param p_pipeline Valid destination pointer to the ShVkPipeline structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelineDestroyLayout(
	VkDevice      device,
	ShVkPipeline* p_pipeline
);



#define SH_PIPELINE_POOL_MAX_PIPELINE_COUNT   64
#define SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT 64


/**
 * @brief Represents a collection of Vulkan pipeline objects and their related resources.
 * 
 * This structure holds arrays of Vulkan pipelines, descriptor set layouts, descriptor pools,
 * and other related resources required to manage and use Vulkan pipelines in a pipeline pool.
 */
typedef struct ShVkPipelinePool {
	ShVkPipeline                 pipelines[SH_PIPELINE_POOL_MAX_PIPELINE_COUNT]; ///< Array of Vulkan pipelines managed by the pool.

	uint32_t                     descriptor_set_layout_binding_count; ///< Total number of descriptor set layout bindings in the pool.
	uint32_t                     src_descriptor_set_layout_count; ///< Number of source descriptor set layouts for copying or setup.
	uint32_t                     descriptor_pool_count; ///< Total number of descriptor pools created and managed by the pool.
	uint32_t                     descriptor_count; ///< Combined total of all descriptors in the descriptor pools.
	uint32_t                     write_descriptor_set_count; ///< Total number of write descriptor sets used for updates.
	uint32_t                     descriptor_set_unit_count; ///< Number of descriptor set units, equal to write_descriptor_set_count.
	
	VkDescriptorSetLayoutBinding descriptor_set_layout_bindings[SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT]; ///< Descriptor set layout bindings.	

	VkDescriptorSetLayout        descriptor_set_layouts[SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT]; ///< Descriptor set layouts.	

	VkDescriptorPool             descriptor_pools[SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT]; ///< Descriptor pools.	
	
	VkDescriptorSet              descriptor_sets[SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT]; ///< Descriptor sets.	

	VkDescriptorBufferInfo       descriptor_buffer_infos[SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT]; ///< Descriptor buffer info.	
	
	VkWriteDescriptorSet         write_descriptor_sets[SH_MAX_PIPELINE_POOL_DESCRIPTOR_COUNT]; ///< Write descriptor sets.

} ShVkPipelinePool;




/**
 * @brief Allocates a new ShVkPipelinePool structure.
 * 
 * This macro allocates heap memory for a new ShVkPipelinePool structure and initializes it to zero.
 * 
 * @return Pointer to the newly allocated ShVkPipelinePool structure, or NULL if allocation fails.
 */
#define shAllocatePipelinePool() ((ShVkPipelinePool*)calloc(1, sizeof(ShVkPipelinePool)))

/**
 * @brief Frees the memory of an ShVkPipelinePool structure.
 * 
 * This macro frees the memory allocated on the heap for an ShVkPipelinePool structure.
 * 
 * @param ptr Pointer to the ShVkPipelinePool structure to be freed.
 */
#define shFreePipelinePool free


/**
 * @brief Creates a descriptor set layout binding for the pipeline pool.
 * 
 * This function initializes a descriptor set layout binding in the pipeline pool.
 * 
 * @param binding Binding index in the descriptor set layout.
 * @param descriptor_type Type of the descriptor (e.g., VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER).
 * @param descriptor_set_count Number of descriptor sets.
 * @param shader_stage Shader stages that use this descriptor.
 * @param[in,out] p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelinePoolCreateDescriptorSetLayoutBinding(
	uint32_t           binding,
	VkDescriptorType   descriptor_type,
	uint32_t           descriptor_set_count,
	VkShaderStageFlags shader_stage,
	ShVkPipelinePool*  p_pipeline_pool
);

/**
 * @brief Creates descriptor set layouts in the pipeline pool.
 * 
 * This function initializes descriptor set layouts in the pipeline pool.
 * 
 * @param device Valid Vulkan device.
 * @param first_binding_idx Index of the first binding in the layout.
 * @param binding_count Number of bindings.
 * @param set_layout_idx Index of the set layout to initialize.
 * @param[in,out] p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelinePoolCreateDescriptorSetLayout(
	VkDevice          device,
	uint32_t          first_binding_idx,
	uint32_t          binding_count,
	uint32_t          set_layout_idx,//set_idx
	ShVkPipelinePool* p_pipeline_pool
);

/**
 * @brief Copies descriptor set layouts within the pipeline pool.
 * 
 * This function copies descriptor set layouts from source indices to destination indices in the pipeline pool.
 * 
 * @param src_set_layout_idx Index of the source descriptor set layout.
 * @param first_dst_set_layout_idx Index of the first destination descriptor set layout.
 * @param dst_set_layout_count Number of destination descriptor set layouts.
 * @param[in,out] p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelinePoolCopyDescriptorSetLayout(
	uint32_t          src_set_layout_idx,
	uint32_t          first_dst_set_layout_idx,
	uint32_t          dst_set_layout_count,
	ShVkPipelinePool* p_pipeline_pool
);

/**
 * @brief Creates a descriptor pool in the pipeline pool.
 * 
 * This function initializes a descriptor pool in the pipeline pool.
 * 
 * @param device Valid Vulkan device.
 * @param pool_idx Index of the pool to initialize.
 * @param descriptor_type Type of the descriptor (e.g., VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER).
 * @param descriptor_count Number of descriptors in the pool.
 * @param[in,out] p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelinePoolCreateDescriptorPool(
	VkDevice          device,
	uint32_t          pool_idx,
	VkDescriptorType  descriptor_type,
	uint32_t          descriptor_count,
	ShVkPipelinePool* p_pipeline_pool
);

/**
 * @brief Allocates descriptor set units in the pipeline pool.
 * 
 * This function allocates descriptor set units for the pipeline pool.
 * 
 * @param device Valid Vulkan device.
 * @param binding Binding index in the descriptor set layout.
 * @param pool_idx Index of the descriptor pool.
 * @param descriptor_type Type of the descriptor (e.g., VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER).
 * @param first_descriptor_set_unit Index of the first descriptor set unit to allocate.
 * @param descriptor_set_unit_count Number of descriptor set units to allocate.
 * @param[in,out] p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelinePoolAllocateDescriptorSetUnits(
	VkDevice          device,
	uint32_t          binding,
	uint32_t          pool_idx,
	VkDescriptorType  descriptor_type,
	uint32_t          first_descriptor_set_unit,
	uint32_t          descriptor_set_unit_count,
	ShVkPipelinePool* p_pipeline_pool
);

/**
 * @brief Sets descriptor buffer infos in the pipeline pool.
 * 
 * This function sets the buffer information for descriptors in the pipeline pool.
 * 
 * @param first_descriptor Index of the first descriptor to set.
 * @param descriptor_count Number of descriptors to update.
 * @param buffer Vulkan buffer to associate with the descriptors.
 * @param buffer_offset Offset into the buffer.
 * @param buffer_size Size of the buffer.
 * @param[in,out] p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelinePoolSetDescriptorBufferInfos(
	uint32_t          first_descriptor,
	uint32_t          descriptor_count,
	VkBuffer          buffer,
	uint32_t          buffer_offset,
	uint32_t          buffer_size,
	ShVkPipelinePool* p_pipeline_pool
);

/**
 * @brief Destroys descriptor set layouts in the pipeline pool.
 * 
 * This function destroys a range of descriptor set layouts in the pipeline pool.
 * 
 * @param device Valid Vulkan device.
 * @param first_set_layout Index of the first descriptor set layout to destroy.
 * @param set_layout_count Number of descriptor set layouts to destroy.
 * @param[in,out] p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelinePoolDestroyDescriptorSetLayouts(
	VkDevice          device,
	uint32_t          first_set_layout,
	uint32_t          set_layout_count,
	ShVkPipelinePool* p_pipeline_pool
);

/**
 * @brief Destroys descriptor pools in the pipeline pool.
 * 
 * This function destroys a range of descriptor pools in the pipeline pool.
 * 
 * @param device Valid Vulkan device.
 * @param first_pool Index of the first descriptor pool to destroy.
 * @param pool_count Number of descriptor pools to destroy.
 * @param[in,out] p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
extern uint8_t shPipelinePoolDestroyDescriptorPools(
	VkDevice          device,
	uint32_t          first_pool,
	uint32_t          pool_count,
	ShVkPipelinePool* p_pipeline_pool
);

/**
 * @brief Updates descriptor set units in the pipeline pool.
 * 
 * This function updates the descriptor set units within the pipeline pool.
 * 
 * @param device Valid Vulkan device.
 * @param first_descriptor_set_unit Index of the first descriptor set unit to update.
 * @param descriptor_set_unit_count Number of descriptor set units to update.
 * @param[in,out] p_pipeline_pool Valid pointer to the ShVkPipelinePool structure.
 * 
 * @return 1 if successful, 0 otherwise.
 */
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
