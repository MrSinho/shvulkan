#ifndef SH_VKCHECK_H
#define SH_VKCHECK_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>

#include <assert.h>

#ifndef NDEBUG
#include <stdio.h>
#endif//NDEBUG

#ifndef NDEBUG
#define shVkAssert(condition, error_msg)\
	if (!(int)(condition)) { printf("shvulkan error: %s\n", error_msg); }\
	assert((int)(condition) && error_msg)
#define shVkAssertResult(result, error_msg)\
	if ((VkResult)(result) != VK_SUCCESS) { printf("shvulkan error: %s %s\n", error_msg, shTranslateVkResult(result)); }\
	assert((int)(result) == VK_SUCCESS && error_msg)
#else
#define shVkAssert(condition, error_msg) assert((int)(condition) && error_msg)
#define shVkAssertResult(result, error_msg) assert((VkResult)(result != VK_SUCCESS) && error_msg)
#endif//NDEBUG


extern int shCheckValidationLayers(const char* validation_layer);

extern const char* shTranslateVkResult(const VkResult vk_result);

extern void shVkCheckResult(VkResult result, const char* error_msg);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VKCHECK_H