#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <memory.h>
#include <stdlib.h>
#include <assert.h>

#include "shvulkan/shVkCore.h"
#include "shvulkan/shVkCheck.h"

#ifdef _MSC_VER
#pragma warning (disable: 6011 6385 6386 6255)
#endif // _MSC_VER

void shCreateInstance(ShVkCore* p_core, const char* application_name, const char* engine_name, const uint8_t enable_validation_layers, const uint32_t extension_count, const char** extension_names) {
	assert(p_core != NULL);
	VkApplicationInfo application_info = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,	//sType;
		NULL,								//pNext;
		application_name,					//pApplicationName;
		VK_MAKE_VERSION(0, 1, 0),			//applicationVersion;
		engine_name,						//pEngineName;
		VK_MAKE_VERSION(0, 1, 0),			//engineVersion;
		SH_VULKAN_VERSION,					//apiVersion;
	};


	VkInstanceCreateInfo instance_create_info = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		&application_info,						//pApplicationInfo;
		0,										//enabledLayerCount;
		NULL,									//ppEnabledLayerNames;
		extension_count,						//enabledExtensionCount;
		extension_names							//ppEnabledExtensionNames;
	};

	if (enable_validation_layers) {
		const char* khronos_validation = "VK_LAYER_KHRONOS_validation";
		if (shCheckValidationLayers(khronos_validation)) {
			instance_create_info.enabledLayerCount = 1;
			instance_create_info.ppEnabledLayerNames = &khronos_validation;
		}
	}
	
	shCheckVkResult(
		vkCreateInstance(&instance_create_info, VK_NULL_HANDLE, &p_core->instance),
		"error creating vkinstance"
	);
}

void shGetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* p_surface_capabilities) {
	assert(p_surface_capabilities != NULL);
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface, p_surface_capabilities);
}

void shSelectPhysicalDevice(ShVkCore* p_core, const VkQueueFlags requirements) {
	assert(p_core != NULL);
	p_core->required_queue_flags = requirements;

	uint32_t pDeviceCount = 0;
	vkEnumeratePhysicalDevices(p_core->instance, &pDeviceCount, NULL);

	VkPhysicalDevice *pDevices = (VkPhysicalDevice*)malloc(pDeviceCount*sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(p_core->instance, &pDeviceCount, pDevices);
	
	if (pDeviceCount == 0) {
		shCheckVkResult(VK_ERROR_UNKNOWN, 
			"no Vulkan compatible gpu has been found"
		);
	}

	uint32_t* graphicsQueueFamilyIndices = (uint32_t*)malloc(pDeviceCount * sizeof(uint32_t));
	uint32_t* surfaceQueueFamilyIndices  = (uint32_t*)malloc(pDeviceCount * sizeof(uint32_t));
	uint32_t suitableDeviceCount = 0;

	for (uint32_t i = 0; i < pDeviceCount; i++) { //DEVICE LOOP

		uint32_t queueFamilyPropertyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(pDevices[i], &queueFamilyPropertyCount, NULL);
		VkQueueFamilyProperties* pQueueFamilyProperties = (VkQueueFamilyProperties*)malloc(queueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(pDevices[i], &queueFamilyPropertyCount, pQueueFamilyProperties);
		
		VkBool32 surfaceSupport = 0;

		for (uint32_t j = 0; j < queueFamilyPropertyCount; j++) {
			if (p_core->required_queue_flags & VK_QUEUE_GRAPHICS_BIT) {
				assert(p_core->surface.width != 0 && p_core->surface.height != 0);
				if (!surfaceSupport) {
					assert(p_core->surface.surface != NULL);
					vkGetPhysicalDeviceSurfaceSupportKHR(pDevices[i], j, p_core->surface.surface, &surfaceSupport);
					if (surfaceSupport) {
						surfaceQueueFamilyIndices[i] = j;
					}
				}
				if (pQueueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					graphicsQueueFamilyIndices[i] = j;
				}
				if (pQueueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT && surfaceSupport) {
					suitableDeviceCount++;
				}
			}
			else if (p_core->required_queue_flags & VK_QUEUE_COMPUTE_BIT && pQueueFamilyProperties[j].queueFlags & VK_QUEUE_COMPUTE_BIT) {
				suitableDeviceCount++;
				break;
			}
		}
		free(pQueueFamilyProperties);
	}

	if (suitableDeviceCount == 0) {
		shCheckVkResult(VK_ERROR_UNKNOWN,
			"no suitable GPU has been found"
		);
	}

	uint32_t *scores = malloc(suitableDeviceCount * sizeof(uint32_t));
	for (uint32_t i = 0; i < suitableDeviceCount; i++) {

		scores[i] = 0;

		VkPhysicalDeviceProperties pDeviceProperties;
		vkGetPhysicalDeviceProperties(pDevices[i], &pDeviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(pDevices[i], &deviceFeatures);
		
		if (p_core->required_queue_flags & VK_QUEUE_COMPUTE_BIT) {
			scores[i] += pDeviceProperties.limits.maxMemoryAllocationCount;
			scores[i] += pDeviceProperties.limits.maxComputeSharedMemorySize;
			scores[i] += pDeviceProperties.limits.maxComputeWorkGroupInvocations;
		}
		if (p_core->required_queue_flags & VK_QUEUE_GRAPHICS_BIT) {
			scores[i] += pDeviceProperties.limits.maxMemoryAllocationCount;
			scores[i] += pDeviceProperties.limits.maxVertexInputAttributes;
			scores[i] += pDeviceProperties.limits.maxVertexInputBindings;
			scores[i] += pDeviceProperties.limits.maxVertexInputAttributeOffset;
			scores[i] += pDeviceProperties.limits.maxVertexInputBindingStride;
			scores[i] += pDeviceProperties.limits.maxVertexOutputComponents;
			scores[i] += pDeviceProperties.limits.maxComputeWorkGroupInvocations;
		}

	}

	if (suitableDeviceCount > 1) {
		for (uint32_t i = 1; i < suitableDeviceCount; i++) {
			if (scores[i] > scores[i - 1]) {
				p_core->physical_device = pDevices[i];
				p_core->graphics_queue.queue_family_index = graphicsQueueFamilyIndices[i];
			}
			else {
				p_core->physical_device = pDevices[i - 1 ];
				p_core->graphics_queue.queue_family_index = graphicsQueueFamilyIndices[i - 1];
			}
		}
	}
	else {
		p_core->physical_device = pDevices[0];
		p_core->graphics_queue.queue_family_index = graphicsQueueFamilyIndices[0];
	}
	
	free(scores);
	free(graphicsQueueFamilyIndices);
	free(surfaceQueueFamilyIndices);
	free(pDevices);

	vkGetPhysicalDeviceProperties(p_core->physical_device, &p_core->physical_device_properties);
	vkGetPhysicalDeviceFeatures(p_core->physical_device, &p_core->physical_device_features);
}

void shSetQueueInfo(const uint32_t queueFamilyIndex, const float* priority, VkDeviceQueueCreateInfo * p_queue_info) {
	assert(p_queue_info != NULL);
	VkDeviceQueueCreateInfo queue_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		queueFamilyIndex,							//queueFamilyIndex;
		1,											//queueCount;
		priority,									//pQueuePriorities;
	};
	*p_queue_info = queue_create_info;
}

void shSetLogicalDevice(ShVkCore* p_core) {
	assert(p_core != NULL);
	const float queue_priority = 1.0f;
	uint32_t queue_info_count = 0;
	VkDeviceQueueCreateInfo queues_info[2];
	if (p_core->required_queue_flags & VK_QUEUE_GRAPHICS_BIT) {
		shSetQueueInfo(p_core->graphics_queue.queue_family_index, &queue_priority, &queues_info[0]);
		queue_info_count++;
	}
	if (p_core->required_queue_flags & VK_QUEUE_COMPUTE_BIT) {
		shSetQueueInfo(p_core->compute_queue.queue_family_index, &queue_priority, (queue_info_count == 1) ? &queues_info[1] : &queues_info[0]);
		(p_core->graphics_queue.queue_family_index != p_core->compute_queue.queue_family_index) ? queue_info_count += 1 : 0;
	}
	
	const char* swapchain_extension_name = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	VkDeviceCreateInfo deviceCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		queue_info_count,						//queueCreateInfoCount;
		queues_info,							//pQueueCreateInfos;
		0, 										//enabledLayerCount;
		NULL,									//ppEnabledLayerNames;
		0, 										//enabledExtensionCount;
		NULL,									//ppEnabledExtensionNames;
		NULL									//pEnabledFeatures;
	};
	if (p_core->required_queue_flags & VK_QUEUE_GRAPHICS_BIT) {
		deviceCreateInfo.enabledExtensionCount = 1;
		deviceCreateInfo.ppEnabledExtensionNames = &swapchain_extension_name;
	}

	shCheckVkResult(
		vkCreateDevice(p_core->physical_device, &deviceCreateInfo, NULL, &p_core->device),
		"error creating logical device"
	);
}

void shCreateSwapchain(ShVkCore* p_core) {
	assert(p_core != NULL);

    shCheckVkResult(
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(p_core->physical_device, p_core->surface.surface, &p_core->surface.surface_capabilities),
		"error getting surface capabilities"
	);

	p_core->swapchain_image_format = SH_SWAPCHAIN_IMAGE_FORMAT;
	VkSwapchainCreateInfoKHR swapchain_create_info = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,				//sType;
		NULL,														//pNext;
		0,															//flags;
		p_core->surface.surface,									//surface;
		p_core->surface.surface_capabilities.minImageCount,			//minImageCount;
		p_core->swapchain_image_format,								//imageFormat;
		VK_COLORSPACE_SRGB_NONLINEAR_KHR,							//imageColorSpace;
		(VkExtent2D) { p_core->surface.surface_capabilities.currentExtent.width,
		p_core->surface.surface_capabilities.currentExtent.height},	//imageExtent;
		1,															//imageArrayLayers;
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,						//imageUsage;
		VK_SHARING_MODE_EXCLUSIVE,									//imageSharingMode;
		0,															//queueFamilyIndexCount;
		NULL,														//pQueueFamilyIndices;
		p_core->surface.surface_capabilities.currentTransform,		//preTransform;
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,							//compositeAlpha;
		VK_PRESENT_MODE_FIFO_KHR,									//presentMode;
		1,															//clipped;
		0,															//oldSwapchain;
	};

    VkCompositeAlphaFlagBitsKHR composite_alpha_flags[4] = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };
    for (uint32_t i = 0; i < 4; i++) {
        if (p_core->surface.surface_capabilities.supportedCompositeAlpha & composite_alpha_flags[i]) {
			swapchain_create_info.compositeAlpha = composite_alpha_flags[i];
            break;
        }
    }
	
	uint32_t present_mode_count;
	VkPresentModeKHR present_modes[7];
	shCheckVkResult(
		vkGetPhysicalDeviceSurfacePresentModesKHR(p_core->physical_device, p_core->surface.surface, &present_mode_count, present_modes),
		"error getting surface present modes"
	);
	swapchain_create_info.presentMode = present_modes[0];

	shCheckVkResult(
		vkCreateSwapchainKHR(p_core->device, &swapchain_create_info, NULL, &p_core->swapchain),
		"error creating swapchain"
	);

}

void shGetSwapchainImages(ShVkCore* p_core) {
	assert(p_core != NULL);
	vkGetSwapchainImagesKHR(p_core->device, p_core->swapchain, &p_core->swapchain_image_count, NULL);
	p_core->p_swapchain_images = (VkImage*)malloc(p_core->swapchain_image_count * sizeof(VkImage));
	vkGetSwapchainImagesKHR(p_core->device, p_core->swapchain, &p_core->swapchain_image_count, p_core->p_swapchain_images);
}

void shCreateImageView(ShVkCore* p_core, const VkImage image, const shImageType type, VkImageView* p_image_view) {
	assert(p_core != NULL && p_image_view != NULL);
	VkImageViewCreateInfo imageViewCreateInfo = {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,	//sType;
			NULL,										//pNext;
			0,											//flags;
			image,										//image;
			VK_IMAGE_VIEW_TYPE_2D,						//viewType;
			0,											//format;
			VK_COMPONENT_SWIZZLE_IDENTITY,				//components;
			0
	};
	//aspectMask;
	if (type == SH_SWAPCHAIN_IMAGE) {
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;	
		imageViewCreateInfo.format = p_core->swapchain_image_format;
	}
	else if (type == SH_DEPTH_IMAGE) {
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		imageViewCreateInfo.format = SH_DEPTH_IMAGE_FORMAT;
	}
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;							//baseMipLevel;
	imageViewCreateInfo.subresourceRange.levelCount = 1;							//levelCount;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;						//baseArrayLayer;
	imageViewCreateInfo.subresourceRange.layerCount = 1;							//layerCount;
	shCheckVkResult(
		vkCreateImageView(p_core->device, &imageViewCreateInfo, NULL, p_image_view),
		"error creating image view"
	);
}

void shCreateSwapchainImageViews(ShVkCore* p_core) {
	assert(p_core != NULL);
	p_core->p_swapchain_image_views = (VkImageView*)malloc(p_core->swapchain_image_count * sizeof(VkImageView));
	for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
		shCreateImageView(p_core, p_core->p_swapchain_images[i], SH_SWAPCHAIN_IMAGE, &p_core->p_swapchain_image_views[i]);
	}
}

void shCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex, VkCommandPool* p_cmd_pool) {
	assert(p_cmd_pool != NULL);
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,			//sType;
		NULL,												//pNext;
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,	//flags;
		queueFamilyIndex									//queueFamilyIndex;
	};
	
	shCheckVkResult(
		vkCreateCommandPool(device, &cmdPoolCreateInfo, NULL, p_cmd_pool), 
		"error creating command pool"
	);
}

void shCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool, VkCommandBuffer* p_cmd_buffer) {
	assert(p_cmd_buffer != NULL);
	VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,	//sType;
		NULL,											//pNext;
		cmdPool,										//commandPool;
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,				//level;
		1												//commandBufferCount;
	};

	shCheckVkResult(
		vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, p_cmd_buffer), 
		"error creating command buffer"
	);
}

void shCreateRenderPass(ShVkCore* p_core) {
	assert(p_core != NULL);
	VkAttachmentDescription colorAttachmentDescription = {
		0,									//flags;
		p_core->swapchain_image_format,		//format;
		VK_SAMPLE_COUNT_1_BIT,				//samples;
		VK_ATTACHMENT_LOAD_OP_CLEAR,		//loadOp;
		VK_ATTACHMENT_STORE_OP_STORE,		//storeOp;
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,	//stencilLoadOp;
		VK_ATTACHMENT_STORE_OP_DONT_CARE,	//stencilStoreOp;
		VK_IMAGE_LAYOUT_UNDEFINED,			//initialLayout;
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR		//finalLayout;
	};

	VkAttachmentReference colorAttachmentReference = {
		0,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkAttachmentDescription depthAttachmentDescription = {
		0,													//flags;
		SH_DEPTH_IMAGE_FORMAT,								//format;
		VK_SAMPLE_COUNT_1_BIT,								//samples;
		VK_ATTACHMENT_LOAD_OP_CLEAR,						//loadOp;
		VK_ATTACHMENT_STORE_OP_DONT_CARE,					//storeOp;
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,					//stencilLoadOp;
		VK_ATTACHMENT_STORE_OP_DONT_CARE,					//stencilStoreOp;
		VK_IMAGE_LAYOUT_UNDEFINED,							//initialLayout;
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,	//finalLayout;
	};

	VkAttachmentReference depthAttachmentReference = {
		1, 													//attachment
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL	//layout
	};

	VkSubpassDescription subpassDescription = {
		0,									//flags;
		VK_PIPELINE_BIND_POINT_GRAPHICS,	//pipelineBindPoint;
		0,									//inputAttachmentCount;
		NULL,								//pInputAttachments;
		1,									//colorAttachmentCount;
		&colorAttachmentReference,			//pColorAttachments;
		NULL,								//pResolveAttachments;
		NULL,								//pDepthStencilAttachment;
		0,									//preserveAttachmentCount;
		NULL								//pPreserveAttachments;
	};
	if (p_core->depth_image_view != VK_NULL_HANDLE) {
		subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;
	}

	VkAttachmentDescription attachment_descriptions[2] = {
		colorAttachmentDescription, depthAttachmentDescription
	};

	VkSubpassDependency subpassDependency = {
		0,																							//srcSubpass;
		0,																							//dstSubpass;
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,	//srcStageMask;
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,	//dstStageMask;
		0, 																							//srcAccessMask;
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,		//dstAccessMask;
		VK_DEPENDENCY_BY_REGION_BIT																	//dependencyFlags;
	};

	VkRenderPassCreateInfo renderPassCreateInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		1,											//attachmentCount;
		attachment_descriptions,					//pAttachments;
		1,											//subpassCount;
		&subpassDescription,						//pSubpasses;
		0,											//dependencyCount;
		NULL										//pDependencies;
	};
	if (p_core->depth_image_view != VK_NULL_HANDLE) {
		renderPassCreateInfo.attachmentCount = 2;
		renderPassCreateInfo.dependencyCount = 1;
		renderPassCreateInfo.pDependencies = &subpassDependency;
	}

	shCheckVkResult(
		vkCreateRenderPass(p_core->device, &renderPassCreateInfo, NULL, &p_core->render_pass),
		"error creating render pass"
	);
}

void shSetFramebuffers(ShVkCore* p_core) {
	assert(p_core != NULL);
	VkFramebufferCreateInfo framebufferCreateInfo = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		p_core->render_pass,						//renderPass;
		1,											//attachmentCount;
		NULL,										//pAttachments;
		p_core->surface.width,						//width;
		p_core->surface.height,						//height;
		1											//layers;
	};

	p_core->p_frame_buffers = (VkFramebuffer*)malloc(p_core->swapchain_image_count * sizeof(VkFramebuffer));
	for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
		VkImageView attachments[2] = {
			p_core->p_swapchain_image_views[i], p_core->depth_image_view 
		};
		if (p_core->depth_image_view != VK_NULL_HANDLE) {
			framebufferCreateInfo.attachmentCount = 2;
		}
		framebufferCreateInfo.pAttachments = attachments;
		shCheckVkResult(
			vkCreateFramebuffer(p_core->device, &framebufferCreateInfo, NULL, &p_core->p_frame_buffers[i]),
			"error creating framebuffer"
		);
	}
}

void shSetSyncObjects(ShVkCore* p_core) {
	assert(p_core != NULL);
	VkFenceCreateInfo render_fenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		VK_FENCE_CREATE_SIGNALED_BIT			//flags;
	};

	shCheckVkResult(
		vkCreateFence(p_core->device, &render_fenceCreateInfo, NULL, &p_core->render_fence),
		"error creating fence"
	);

	VkSemaphoreCreateInfo semaphoreCreateInfo = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0											//flags;
	};

	shCheckVkResult(
		vkCreateSemaphore(p_core->device, &semaphoreCreateInfo, NULL, &p_core->render_semaphore),
		"error creating render semaphore"
	);
	shCheckVkResult(
		vkCreateSemaphore(p_core->device, &semaphoreCreateInfo, NULL, &p_core->present_semaphore),
		"error creating present semaphore"
	);
}

void shSwapchainRelease(ShVkCore* p_core) {
	assert(p_core != NULL);
	vkDeviceWaitIdle(p_core->device);

	for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
		vkDestroyFramebuffer(p_core->device, p_core->p_frame_buffers[i], NULL);
		vkDestroyImageView(p_core->device, p_core->p_swapchain_image_views[i], NULL);
	}
	vkDestroySwapchainKHR(p_core->device, p_core->swapchain, NULL);
}

void shDepthBufferRelease(ShVkCore* p_core) {
	assert(p_core != NULL);
	vkDestroyImageView(p_core->device, p_core->depth_image_view, NULL);
	vkDestroyImage(p_core->device, p_core->depth_image, NULL);
	vkFreeMemory(p_core->device, p_core->depth_image_memory, NULL);
}

void shSurfaceRelease(ShVkCore* p_core) {
	assert(p_core != NULL);
	vkDeviceWaitIdle(p_core->device);

	p_core->swapchain_image_count = 0;
	free(p_core->p_frame_buffers);
	free(p_core->p_swapchain_image_views);
	free(p_core->p_swapchain_images);

	vkDestroySurfaceKHR(p_core->instance, p_core->surface.surface, NULL);
}

void shCmdRelease(ShVkCore* p_core) {
	assert(p_core != NULL);
	vkDeviceWaitIdle(p_core->device);

	vkDestroySemaphore(p_core->device, p_core->present_semaphore, NULL);
	vkDestroySemaphore(p_core->device, p_core->render_semaphore, NULL);
	vkDestroyFence(p_core->device, p_core->render_fence, NULL);

	vkFreeCommandBuffers(p_core->device, p_core->graphics_cmd_pool, 1, &p_core->graphics_cmd_buffer);
	vkDestroyCommandPool(p_core->device, p_core->graphics_cmd_pool, NULL);
}

void shRenderPassRelease(ShVkCore* p_core) {
	assert(p_core != NULL);
	vkDeviceWaitIdle(p_core->device);
	vkDestroyRenderPass(p_core->device, p_core->render_pass, NULL);
}

void shDeviceRelease(ShVkCore* p_core) {
	assert(p_core != NULL);
	vkDeviceWaitIdle(p_core->device);
	vkDestroyDevice(p_core->device, NULL);
}

void shInstanceRelease(ShVkCore* p_core) {	
	assert(p_core != NULL);
	vkDestroyInstance(p_core->instance, NULL);
}

void shVulkanRelease(ShVkCore* p_core) {
	assert(p_core != NULL);
	vkDeviceWaitIdle(p_core->device);
	shSwapchainRelease(p_core);
	shDepthBufferRelease(p_core);
	shSurfaceRelease(p_core);
	shCmdRelease(p_core);
	shRenderPassRelease(p_core);
	shDeviceRelease(p_core);
	shInstanceRelease(p_core);
}

#ifdef __cplusplus
}
#endif//__cplusplus