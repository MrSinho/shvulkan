#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <memory.h>
#include <stdlib.h>

#include "shvulkan/shVkCore.h"
#include "shvulkan/shVkCheck.h"

#ifdef _MSC_VER
#pragma warning (disable: 6385 6386 6255 6001)
#endif // _MSC_VER

void shCreateInstance(ShVkCore* p_core, const char* application_name, const char* engine_name, const uint8_t enable_validation_layers, const uint32_t extension_count, const char** extension_names) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
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
	
	shVkAssertResult(
		vkCreateInstance(&instance_create_info, VK_NULL_HANDLE, &p_core->instance),
		"error creating vkinstance"
	);
}

void shCreateWindowSurface(ShVkCore* p_core, void* window_handle) {
#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,	// sType
		NULL,												// pNext
		0,													// flags
		(HINSTANCE)GetModuleHandle(NULL),					// hinstance;
		(HWND)window_handle									// hwnd;
	};
	vkCreateWin32SurfaceKHR(p_core->instance, &surface_create_info, NULL, &p_core->surface.surface);
#elif defined __linux__
#define VK_USE_PLATFORM_XLIB_KHR

#endif//_WIN32
}

void shSelectPhysicalDevice(ShVkCore* p_core, const VkQueueFlags requirements) {
	shVkAssert(p_core != NULL, "invalid arguments ");
	p_core->required_queue_flags = requirements;

	uint32_t pDeviceCount = 0;
	vkEnumeratePhysicalDevices(p_core->instance, &pDeviceCount, NULL);

	VkPhysicalDevice* physical_devices = (VkPhysicalDevice*)calloc(pDeviceCount, sizeof(VkPhysicalDevice));
	shVkAssert(physical_devices != NULL, "invalid physical devices pointer");

	vkEnumeratePhysicalDevices(p_core->instance, &pDeviceCount, physical_devices);
	
	if (pDeviceCount == 0) {
		shVkAssert(VK_ERROR_UNKNOWN, 
			"no Vulkan compatible gpu has been found"
		);
	}

	uint32_t* graphics_queue_family_indices = (uint32_t*)calloc(pDeviceCount, sizeof(uint32_t));
	shVkAssert(graphics_queue_family_indices != NULL, "invalid graphics queue family indices pointer");

	uint32_t* surface_queue_family_indices = (uint32_t*)calloc(pDeviceCount, sizeof(uint32_t));
	shVkAssert(surface_queue_family_indices != NULL, "invalid surface queue family indices pointer");

	uint32_t suitableDeviceCount = 0;

	for (uint32_t i = 0; i < pDeviceCount; i++) { //DEVICE LOOP

		uint32_t queue_family_property_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &queue_family_property_count, NULL);
		VkQueueFamilyProperties* p_queue_family_properties = (VkQueueFamilyProperties*)calloc(queue_family_property_count, sizeof(VkQueueFamilyProperties));
		shVkAssert(p_queue_family_properties != NULL, "invalid queue family properties pointer");
		vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &queue_family_property_count, p_queue_family_properties);

		VkBool32 surfaceSupport = 0;

		for (uint32_t j = 0; j < queue_family_property_count; j++) {
			if (p_core->required_queue_flags & VK_QUEUE_GRAPHICS_BIT) {
				shVkAssert(p_core->surface.width != 0 && p_core->surface.height != 0, "invalid surface parameters ");
				if (!surfaceSupport) {
					shVkAssert(p_core->surface.surface != NULL, "invalid surface ");
					vkGetPhysicalDeviceSurfaceSupportKHR(physical_devices[i], j, p_core->surface.surface, &surfaceSupport);
					if (surfaceSupport) {
						surface_queue_family_indices[i] = j;
					}
				}
				if (p_queue_family_properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					graphics_queue_family_indices[i] = j;
				}
				if (p_queue_family_properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT && surfaceSupport) {
					suitableDeviceCount++;
				}
			}
			else if (p_core->required_queue_flags & VK_QUEUE_COMPUTE_BIT && p_queue_family_properties[j].queueFlags & VK_QUEUE_COMPUTE_BIT) {
				suitableDeviceCount++;
				break;
			}
		}
		free(p_queue_family_properties);
	}

	if (suitableDeviceCount == 0) {
		shVkAssert(VK_ERROR_UNKNOWN,
			"no suitable GPU has been found"
		);
	}

	uint32_t* scores = malloc(suitableDeviceCount * sizeof(uint32_t));
	shVkAssert(scores != NULL, "invalid scores pointer");
	for (uint32_t i = 0; i < suitableDeviceCount; i++) {

		scores[i] = 0;

		VkPhysicalDeviceProperties physical_device_properties;
		vkGetPhysicalDeviceProperties(physical_devices[i], &physical_device_properties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(physical_devices[i], &deviceFeatures);

		if (p_core->required_queue_flags & VK_QUEUE_COMPUTE_BIT) {
			scores[i] += physical_device_properties.limits.maxMemoryAllocationCount;
			scores[i] += physical_device_properties.limits.maxComputeSharedMemorySize;
			scores[i] += physical_device_properties.limits.maxComputeWorkGroupInvocations;
		}
		if (p_core->required_queue_flags & VK_QUEUE_GRAPHICS_BIT) {
			scores[i] += physical_device_properties.limits.maxMemoryAllocationCount;
			scores[i] += physical_device_properties.limits.maxVertexInputAttributes;
			scores[i] += physical_device_properties.limits.maxVertexInputBindings;
			scores[i] += physical_device_properties.limits.maxVertexInputAttributeOffset;
			scores[i] += physical_device_properties.limits.maxVertexInputBindingStride;
			scores[i] += physical_device_properties.limits.maxVertexOutputComponents;
			scores[i] += physical_device_properties.limits.maxComputeWorkGroupInvocations;
		}

	}

	if (suitableDeviceCount > 1) {
		for (uint32_t i = 1; i < suitableDeviceCount; i++) {
			if (scores[i] > scores[i - 1]) {
				p_core->physical_device = physical_devices[i];
				p_core->graphics_queue.queue_family_index = graphics_queue_family_indices[i];
			}
			else {
				p_core->physical_device = physical_devices[i - 1];
				p_core->graphics_queue.queue_family_index = graphics_queue_family_indices[i - 1];
			}
		}
	}
	else {
		p_core->physical_device = physical_devices[0];
		p_core->graphics_queue.queue_family_index = graphics_queue_family_indices[0];
	}

	free(scores);
	free(graphics_queue_family_indices);
	free(surface_queue_family_indices);
	free(physical_devices);

	vkGetPhysicalDeviceProperties(p_core->physical_device, &p_core->physical_device_properties);
	vkGetPhysicalDeviceFeatures(p_core->physical_device, &p_core->physical_device_features);
	vkGetPhysicalDeviceMemoryProperties(p_core->physical_device, &p_core->physical_device_memory_properties);
}

void shSetQueueInfo(const uint32_t queue_family_index, const float* priority, VkDeviceQueueCreateInfo * p_queue_info) {
	shVkAssert(p_queue_info != NULL, "invalid queue info pointer");
	VkDeviceQueueCreateInfo queue_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		queue_family_index,							//queueFamilyIndex;
		1,											//queueCount;
		priority,									//pQueuePriorities;
	};
	*p_queue_info = queue_create_info;
}

void shSetLogicalDevice(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	const float queue_priority = 1.0f;
	uint32_t queue_info_count = 0;
	VkDeviceQueueCreateInfo queues_info[2];
	if (p_core->required_queue_flags & VK_QUEUE_GRAPHICS_BIT) {
		shSetQueueInfo(p_core->graphics_queue.queue_family_index, &queue_priority, &queues_info[0]);
		queue_info_count++;
	}
	if (p_core->required_queue_flags & VK_QUEUE_COMPUTE_BIT) {
		shSetQueueInfo(p_core->compute_queue.queue_family_index, &queue_priority, (queue_info_count == 1) ? &queues_info[1] : &queues_info[0]);
		queue_info_count++;
		if (p_core->required_queue_flags & VK_QUEUE_GRAPHICS_BIT) {
			(p_core->graphics_queue.queue_family_index == p_core->compute_queue.queue_family_index) && (queue_info_count--);
		}
	}
	
	const char* extension_names[2] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_MEMORY_BUDGET_EXTENSION_NAME };
	VkDeviceCreateInfo deviceCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,														//sType;
		NULL,																						//pNext;
		0,																							//flags;
		queue_info_count,																			//queueCreateInfoCount;
		queues_info,																				//pQueueCreateInfos;
		0, 																							//enabledLayerCount;
		NULL,																						//ppEnabledLayerNames;
		p_core->required_queue_flags == SH_VK_CORE_COMPUTE ? 1 : 2,									//enabledExtensionCount;
		p_core->required_queue_flags == SH_VK_CORE_COMPUTE ? &extension_names[1] : extension_names,	//ppEnabledExtensionNames;
		NULL																						//pEnabledFeatures;
	};
	
	if(vkCreateDevice(p_core->physical_device, &deviceCreateInfo, NULL, &p_core->device) != VK_SUCCESS) {
		deviceCreateInfo.enabledExtensionCount--;
		shVkAssertResult(
			vkCreateDevice(p_core->physical_device, &deviceCreateInfo, NULL, &p_core->device), 
			"error creating logical device"
		);
	}
}

void shCreateSwapchain(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");

    shVkAssertResult(
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

	{
		uint32_t format_count = 0;
		shVkAssertResult(
			vkGetPhysicalDeviceSurfaceFormatsKHR(p_core->physical_device, p_core->surface.surface, &format_count, NULL),
			"error getting surface available format count"
		);
		VkSurfaceFormatKHR* p_formats = calloc(format_count, sizeof(VkSurfaceFormatKHR));
		shVkAssert(p_formats != NULL, "invalid surface formats pointer");
		shVkAssertResult(
			vkGetPhysicalDeviceSurfaceFormatsKHR(p_core->physical_device, p_core->surface.surface, &format_count, p_formats),
			"error getting surface available formats"
		);
		uint8_t format_found = 0;
		for (uint32_t i = 0; i < format_count; i++) {
			if (p_formats[i].format == SH_SWAPCHAIN_IMAGE_FORMAT) {
				format_found++;
				swapchain_create_info.imageColorSpace = p_formats[i].colorSpace;
				break;
			}
		}
		if (!format_found) {
			p_core->swapchain_image_format = p_formats[0].format;
			swapchain_create_info.imageFormat = p_core->swapchain_image_format;
			swapchain_create_info.imageColorSpace = p_formats[0].colorSpace;
		}
	}

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
	
	shVkAssertResult(
		vkCreateSwapchainKHR(p_core->device, &swapchain_create_info, NULL, &p_core->swapchain),
		"error creating swapchain"
	);
}

void shGetSwapchainImages(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	vkGetSwapchainImagesKHR(p_core->device, p_core->swapchain, &p_core->swapchain_image_count, NULL);
	p_core->p_swapchain_images = (VkImage*)malloc(p_core->swapchain_image_count * sizeof(VkImage));
	shVkAssert(p_core->p_swapchain_images != NULL, "invalid swapchain images pointer");
	vkGetSwapchainImagesKHR(p_core->device, p_core->swapchain, &p_core->swapchain_image_count, p_core->p_swapchain_images);
}

void shCreateImageView(ShVkCore* p_core, const VkImage image, const ShVkImageType type, VkImageView* p_image_view) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	shVkAssert(p_image_view != NULL, "invalid image view pointer");
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
	shVkAssertResult(
		vkCreateImageView(p_core->device, &imageViewCreateInfo, NULL, p_image_view),
		"error creating image view"
	);
}

void shCreateSwapchainImageViews(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	p_core->p_swapchain_image_views = (VkImageView*)malloc(p_core->swapchain_image_count * sizeof(VkImageView));
	shVkAssert(p_core->p_swapchain_image_views != NULL, "invalid swapchain image views pointer");
	for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
		shCreateImageView(p_core, p_core->p_swapchain_images[i], SH_SWAPCHAIN_IMAGE, &p_core->p_swapchain_image_views[i]);
	}
}

void shCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex, VkCommandPool* p_cmd_pool) {
	shVkAssert(p_cmd_pool != NULL, "invalid command pool pointer ");
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,			//sType;
		NULL,												//pNext;
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,	//flags;
		queueFamilyIndex									//queueFamilyIndex;
	};
	
	shVkAssertResult(
		vkCreateCommandPool(device, &cmdPoolCreateInfo, NULL, p_cmd_pool), 
		"error creating command pool"
	);
}

void shCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool, VkCommandBuffer* p_cmd_buffer) {
	shVkAssert(p_cmd_buffer != NULL, "invalid command buffer pointer ");
	VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,	//sType;
		NULL,											//pNext;
		cmdPool,										//commandPool;
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,				//level;
		1												//commandBufferCount;
	};

	shVkAssertResult(
		vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, p_cmd_buffer), 
		"error creating command buffer"
	);
}

void shCreateCommandData(ShVkCore* p_core, const VkQueueFlagBits usage, const uint32_t thread_count, ShVkCommand** pp_commands) {
	shVkAssert(thread_count != 0, "invalid thread count");

	*pp_commands = calloc(thread_count, sizeof(ShVkCommand));
	ShVkCommand* p_commands = *pp_commands;
	shVkAssert(p_commands != NULL, "invalid commands pointer");

	for (uint32_t thread = 0; thread < thread_count; thread++) {
		if (usage == VK_QUEUE_GRAPHICS_BIT) {
			shCreateCmdPool((p_core)->device, (p_core)->graphics_queue.queue_family_index, &p_commands[thread].cmd_pool);
		}
		else if (usage == VK_QUEUE_COMPUTE_BIT) {
			shCreateCmdPool((p_core)->device, (p_core)->compute_queue.queue_family_index, &p_commands[thread].cmd_pool);
		}
		shCreateCmdBuffer((p_core)->device, p_commands[thread].cmd_pool, &p_commands[thread].cmd_buffer);
	}
	p_core->thread_count = thread_count;
}

void shCreateRenderPass(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
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

	shVkAssertResult(
		vkCreateRenderPass(p_core->device, &renderPassCreateInfo, NULL, &p_core->render_pass),
		"error creating render pass"
	);
}

void shSetFramebuffers(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
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
	shVkAssert(p_core->p_frame_buffers != NULL, "invalid frame buffers pointer");

	for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
		VkImageView attachments[2] = {
			p_core->p_swapchain_image_views[i], p_core->depth_image_view 
		};
		if (p_core->depth_image_view != VK_NULL_HANDLE) {
			framebufferCreateInfo.attachmentCount = 2;
		}
		framebufferCreateInfo.pAttachments = attachments;
		shVkAssertResult(
			vkCreateFramebuffer(p_core->device, &framebufferCreateInfo, NULL, &p_core->p_frame_buffers[i]),
			"error creating framebuffer"
		);
	}
}

void shSetSyncObjects(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	VkFenceCreateInfo fence_create_info = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		VK_FENCE_CREATE_SIGNALED_BIT			//flags;
	};

	VkSemaphoreCreateInfo semaphore_create_info = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0											//flags;
	};

	p_core->p_render_semaphores = calloc(p_core->thread_count, sizeof(VkSemaphore));
	
	if (p_core->p_graphics_commands != NULL) {
		for (uint32_t thread = 0; thread < p_core->thread_count; thread++) {
			shVkAssertResult(
				vkCreateFence(p_core->device, &fence_create_info, NULL, &p_core->p_graphics_commands[thread].fence),
				"error creating fence"
			);
			
			shVkAssert(p_core->p_render_semaphores != NULL, "invalid render semaphores pointer");
			shVkAssertResult(
				vkCreateSemaphore(p_core->device, &semaphore_create_info, NULL, &p_core->p_render_semaphores[thread]),
				"error creating render semaphore"
			);
		}
	}
	

	if (p_core->p_compute_commands != NULL) {
		for (uint32_t thread = 0; thread < p_core->thread_count; thread++) {
			shVkAssertResult(
				vkCreateFence(p_core->device, &fence_create_info, NULL, &p_core->p_compute_commands[thread].fence),
				"error creating fence"
			);
		}
	}
	
}

void shSwapchainRelease(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	vkDeviceWaitIdle(p_core->device);

	for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
		vkDestroyFramebuffer(p_core->device, p_core->p_frame_buffers[i], NULL);
		vkDestroyImageView(p_core->device, p_core->p_swapchain_image_views[i], NULL);
	}
	vkDestroySwapchainKHR(p_core->device, p_core->swapchain, NULL);
	p_core->swapchain_image_count = 0;
	free(p_core->p_frame_buffers);
	free(p_core->p_swapchain_image_views);
	free(p_core->p_swapchain_images);
}

void shDepthBufferRelease(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	vkDestroyImageView(p_core->device, p_core->depth_image_view, NULL);
	vkDestroyImage(p_core->device, p_core->depth_image, NULL);
	vkFreeMemory(p_core->device, p_core->depth_image_memory, NULL);
}

void shSurfaceRelease(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	vkDeviceWaitIdle(p_core->device);
	vkDestroySurfaceKHR(p_core->instance, p_core->surface.surface, NULL);
}

void shCmdRelease(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	vkDeviceWaitIdle(p_core->device);

	if (p_core->p_graphics_commands != NULL) {
		for (uint32_t thread_idx = 0; thread_idx < p_core->thread_count; thread_idx++) {
			vkDestroyFence(p_core->device, p_core->p_graphics_commands[thread_idx].fence, NULL);
			vkDestroySemaphore(p_core->device, p_core->p_render_semaphores[thread_idx], NULL);
			vkFreeCommandBuffers(p_core->device, p_core->p_graphics_commands[thread_idx].cmd_pool, 1, &p_core->p_graphics_commands[thread_idx].cmd_buffer);
			vkDestroyCommandPool(p_core->device, p_core->p_graphics_commands[thread_idx].cmd_pool, NULL);
		}
	}

	if (p_core->p_compute_commands != NULL) {
		for (uint32_t thread_idx = 0; thread_idx < p_core->thread_count; thread_idx++) {
			vkDestroyFence(p_core->device, p_core->p_compute_commands[thread_idx].fence, NULL);
			vkFreeCommandBuffers(p_core->device, p_core->p_compute_commands[thread_idx].cmd_pool, 1, &p_core->p_compute_commands[thread_idx].cmd_buffer);
			vkDestroyCommandPool(p_core->device, p_core->p_compute_commands[thread_idx].cmd_pool, NULL);
		}
	}
}

void shRenderPassRelease(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	vkDeviceWaitIdle(p_core->device);
	vkDestroyRenderPass(p_core->device, p_core->render_pass, NULL);
}

void shDeviceRelease(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	vkDeviceWaitIdle(p_core->device);
	vkDestroyDevice(p_core->device, NULL);
}

void shInstanceRelease(ShVkCore* p_core) {	
	shVkAssert(p_core != NULL, "invalid core pointer ");
	vkDestroyInstance(p_core->instance, NULL);
}

void shVulkanRelease(ShVkCore* p_core) {
	shVkAssert(p_core != NULL, "invalid core pointer ");
	vkDeviceWaitIdle(p_core->device);
	if (p_core->swapchain != VK_NULL_HANDLE) { 
		shSwapchainRelease(p_core); 
	}
	if (p_core->depth_image_memory != VK_NULL_HANDLE) { 
		shDepthBufferRelease(p_core); 
	}
	if (p_core->surface.surface != VK_NULL_HANDLE) { 
		shSurfaceRelease(p_core); 
	}
	shCmdRelease(p_core); 
	if (p_core->render_pass != VK_NULL_HANDLE) { 
		shRenderPassRelease(p_core); 
	}
	if (p_core->device != VK_NULL_HANDLE) {
		shDeviceRelease(p_core); 
	}
	if (p_core->instance != VK_NULL_HANDLE) {
		shInstanceRelease(p_core); 
	}
}



void shBeginCommandBuffer(const VkCommandBuffer cmd_buffer) {
	VkCommandBufferBeginInfo command_buffer_begin_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		NULL,
		0,
		NULL
	};
	vkBeginCommandBuffer(cmd_buffer, &command_buffer_begin_info);
}

void shQueueSubmit(VkCommandBuffer* cmd_buffer, const VkQueue queue, VkFence fence) {
	VkSubmitInfo submit_info = {
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			NULL,
			0,
			NULL,
			NULL,
			1,
			cmd_buffer
	};
	vkQueueSubmit(queue, 1, &submit_info, fence);
}

#ifdef __cplusplus
}
#endif//__cplusplus
