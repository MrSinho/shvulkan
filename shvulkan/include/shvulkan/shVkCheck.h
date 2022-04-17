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


extern int shCheckValidationLayers(const char* validation_layer);

extern const char* shTranslateVkResult(const VkResult vk_result);

extern void shVkCheckResult(VkResult result, const char* error_msg);

#ifndef NDEBUG
static void shVkAssert(int condition, const char* error_msg) {
	if (!condition) {
		printf("shvulkan error: %s\n", error_msg);
	}
	assert(condition && error_msg);
}
static void shVkAssertResult(VkResult result, const char* error_msg) {
	if (result != VK_SUCCESS) {
		printf("shvulkan error: %s %s\n", error_msg, shTranslateVkResult(result));
	}
	assert(result == VK_SUCCESS && error_msg);
}
#else
#define shVkAssert(condition, error_msg) assert((int)(condition) && error_msg)
#define shVkAssertResult(result, error_msg) assert((VkResult)(result != VK_SUCCESS) && error_msg)
#endif//NDEBUG

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VKCHECK_H