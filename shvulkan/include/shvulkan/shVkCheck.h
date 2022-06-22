#ifndef SH_VKCHECK_H
#define SH_VKCHECK_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>

#include <stdint.h>
#include <stdio.h>

extern uint8_t shCheckValidationLayers(const char* validation_layer);

extern const char* shTranslateVkResult(const VkResult vk_result);

#define shVkError(condition, error_msg, failure_expression)\
	if ((int)(condition)) {\
		printf("shvulkan error: %s\n", (const char*)(error_msg));\
		failure_expression;\
	}

static uint8_t shVkWarning(int condition, const char* msg) {
	if ((int)(condition)) { printf("shvulkan warning: %s\n", msg); return 1; }
	return 0;
}

#define shVkResultError(result, error_msg, failure_expression)\
	if ((VkResult)(result) != VK_SUCCESS) {\
		printf("shvulkan error: %s, %s\n", error_msg, shTranslateVkResult((VkResult)(result)));\
		exit(-1);\
	}



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VKCHECK_H