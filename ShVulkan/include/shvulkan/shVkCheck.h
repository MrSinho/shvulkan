#ifndef SH_VKCHECK_H
#define SH_VKCHECK_H

#include <vulkan/vulkan.h>

extern int shCheckValidationLayers(const char* validation_layer);

extern const char* shTranslateVkResult(const VkResult vk_result);

extern void shCheckVkResult(VkResult result, const char* error_msg);

#endif//SH_VKCHECK_H