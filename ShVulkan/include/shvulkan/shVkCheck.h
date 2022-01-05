#ifndef SH_VKCHECK_H
#define SH_VKCHECK_H

#include <vulkan/vulkan.h>

#ifndef NDEBUG
extern int shCheckValidationLayer(const char* validation_layer);
#endif//NDEBUG

extern const char* shTranslateVkResult(const VkResult vk_result);

extern void shCheckVkResult(VkResult result, const char* error_msg);

#endif//SH_VKCHECK_H