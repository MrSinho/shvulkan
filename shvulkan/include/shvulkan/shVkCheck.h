#ifndef SH_VKCHECK_H
#define SH_VKCHECK_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>

#include <assert.h>

#include <stdio.h>


extern int shCheckValidationLayers(const char* validation_layer);

extern const char* shTranslateVkResult(const VkResult vk_result);

extern void shVkCheckResult(VkResult result, const char* error_msg);

#define shVkAssert(condition, error_msg)\
	assert((int)(condition) &&\
	"shvulkan error: " &&\
	(const char*)(error_msg))

#define shVkAssertResult(result, error_msg)\
	if ((VkResult)(result) != VK_SUCCESS) {\
		printf("shvulkan error: %s, %s\n", error_msg, shTranslateVkResult((VkResult)(result)));\
		assert((VkResult)(result) == VK_SUCCESS);\
	}\

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VKCHECK_H