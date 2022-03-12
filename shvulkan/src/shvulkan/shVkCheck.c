#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkCheck.h"

#include <stdlib.h>
#include <assert.h>

#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#pragma warning (disable: 6385)
#endif//_MSC_VER

int shCheckValidationLayers(const char* validation_layer) {
	assert(validation_layer != NULL);
	uint32_t available_layer_count = 0;
	vkEnumerateInstanceLayerProperties(&available_layer_count, NULL);

	VkLayerProperties* p_available_layer_properties = (VkLayerProperties*)calloc(available_layer_count, sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&available_layer_count, p_available_layer_properties);

	if (p_available_layer_properties != NULL) {
		puts("Installed validation layers: ");
		for (uint32_t i = 0; i < available_layer_count; i++) {
			puts(p_available_layer_properties[i].layerName);
			if (strcmp(p_available_layer_properties[i].layerName, validation_layer) == 0) {
				free(p_available_layer_properties);
				return 1;
			}
		}
	}
	free(p_available_layer_properties);

	return 0;
}

const char* shTranslateVkResult(const VkResult vk_result) {
	switch (vk_result) {
	case VK_SUCCESS:return "VK_SUCCESS";
	case VK_NOT_READY:return "VK_NOT_READY";
	case VK_TIMEOUT:return "VK_TIMEOUT";
	case VK_EVENT_SET:return "VK_EVENT_SET";
	case VK_EVENT_RESET:return "VK_EVENT_RESET";
	case VK_INCOMPLETE:return "VK_INCOMPLETE";
	case VK_ERROR_OUT_OF_HOST_MEMORY:return "VK_ERROR_OUT_OF_HOST_MEMORY";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
	case VK_ERROR_INITIALIZATION_FAILED:return "VK_ERROR_INITIALIZATION_FAILED";
	case VK_ERROR_DEVICE_LOST:return "VK_ERROR_DEVICE_LOST";
	case VK_ERROR_MEMORY_MAP_FAILED:return "VK_ERROR_MEMORY_MAP_FAILED";
	case VK_ERROR_LAYER_NOT_PRESENT:return "VK_ERROR_LAYER_NOT_PRESENT";
	case VK_ERROR_EXTENSION_NOT_PRESENT:return "VK_ERROR_EXTENSION_NOT_PRESENT";
	case VK_ERROR_FEATURE_NOT_PRESENT:return "VK_ERROR_FEATURE_NOT_PRESENT";
	case VK_ERROR_INCOMPATIBLE_DRIVER:return "VK_ERROR_INCOMPATIBLE_DRIVER";
	case VK_ERROR_TOO_MANY_OBJECTS:return "VK_ERROR_TOO_MANY_OBJECTS";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:return "VK_ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_SURFACE_LOST_KHR:return "VK_ERROR_SURFACE_LOST_KHR";
	case VK_SUBOPTIMAL_KHR:return "VK_SUBOPTIMAL_KHR";
	case VK_ERROR_OUT_OF_DATE_KHR:return "VK_ERROR_OUT_OF_DATE_KHR";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case VK_ERROR_VALIDATION_FAILED_EXT:return "VK_ERROR_VALIDATION_FAILED_EXT";
	case VK_RESULT_MAX_ENUM:return "VK_RESULT_MAX_ENUM";
	}
	return "unknown vkresult";
}

void shCheckVkResult(VkResult result, const char* error_msg) {
	if (result != VK_SUCCESS) {
#ifndef NDEBUG
		printf("ShVulkan error: %s, %s\n", error_msg, shTranslateVkResult(result));
#endif // NDEBUG
		system("pause");
		exit(-1);
	}
}

#ifdef __cplusplus
}
#endif//__cplusplus