#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkCheck.h"

#include <stdlib.h>

#include <stdio.h>
#include <string.h>

uint8_t shCheckValidationLayers(const char* validation_layer) {
	shVkError(validation_layer == NULL, "invalid validation layer");
	uint32_t available_layer_count = 0;
	vkEnumerateInstanceLayerProperties(&available_layer_count, NULL);

    if (available_layer_count != 0) {
        VkLayerProperties* p_available_layer_properties = (VkLayerProperties*)calloc(available_layer_count, sizeof(VkLayerProperties));
        shVkError(p_available_layer_properties == NULL, "shCheckValidationLayers: invalid available layer properties memory block");
        vkEnumerateInstanceLayerProperties(&available_layer_count, p_available_layer_properties);

        if (p_available_layer_properties != NULL) {
            //puts("Installed validation layers:");
            for (uint32_t i = 0; i < available_layer_count; i++) {
                //puts(p_available_layer_properties[i].layerName);
                if (strcmp(p_available_layer_properties[i].layerName, validation_layer) == 0) {
                    free(p_available_layer_properties);
                    return 1;
                }
            }
        }
        free(p_available_layer_properties);
    }

	return 0;
}

const char* shTranslateVkResult(const VkResult vk_result) {
	switch (vk_result) {
    case VK_SUCCESS:
        return "VK_SUCCESS";
    case VK_NOT_READY:
        return "VK_NOT_READY";
    case VK_TIMEOUT:
        return "VK_TIMEOUT";
    case VK_EVENT_SET:
        return "VK_EVENT_SET";
    case VK_EVENT_RESET:
        return "VK_EVENT_RESET";
    case VK_INCOMPLETE:
        return "VK_INCOMPLETE";
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_INITIALIZATION_FAILED:
        return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST:
        return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED:
        return "VK_ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT:
        return "VK_ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return "VK_ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS:
        return "VK_ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case VK_ERROR_FRAGMENTED_POOL:
        return "VK_ERROR_FRAGMENTED_POOL";
    case VK_ERROR_UNKNOWN:
        return "VK_ERROR_UNKNOWN";
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        return "VK_ERROR_OUT_OF_POOL_MEMORY";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case VK_ERROR_FRAGMENTATION:
        return "VK_ERROR_FRAGMENTATION";
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
        return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
    case VK_ERROR_SURFACE_LOST_KHR:
        return "VK_ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_SUBOPTIMAL_KHR:
        return "VK_SUBOPTIMAL_KHR";
    case VK_ERROR_OUT_OF_DATE_KHR:
        return "VK_ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_VALIDATION_FAILED_EXT:
        return "VK_ERROR_VALIDATION_FAILED_EXT";
    case VK_ERROR_INVALID_SHADER_NV:
        return "VK_ERROR_INVALID_SHADER_NV";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
        return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
    case VK_ERROR_NOT_PERMITTED_EXT:
        return "VK_ERROR_NOT_PERMITTED_EXT";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
    //case VK_THREAD_IDLE_KHR:
    //    return "VK_THREAD_IDLE_KHR";
    //case VK_THREAD_DONE_KHR:
    //    return "VK_THREAD_DONE_KHR";
    //case VK_OPERATION_DEFERRED_KHR:
    //    return "VK_OPERATION_DEFERRED_KHR";
    //case VK_OPERATION_NOT_DEFERRED_KHR:
    //    return "VK_OPERATION_NOT_DEFERRED_KHR";
    //case VK_PIPELINE_COMPILE_REQUIRED_EXT:
    //    return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
    //case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
    //    return "VK_ERROR_OUT_OF_POOL_MEMORY_KHR";
    //case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR:
    //    return "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR";
    //case VK_ERROR_FRAGMENTATION_EXT:
    //    return "VK_ERROR_FRAGMENTATION_EXT";
    //case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:
    //    return "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT";
    //case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR:
    //    return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR";
    //case VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT:
    //    return "VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT";
    case VK_RESULT_MAX_ENUM:
        return "VK_RESULT_MAX_ENUM";
    default: 
        return "unknown vkresult";
	}
	return "unknown vkresult";
}



#ifdef __cplusplus
}
#endif//__cplusplus
