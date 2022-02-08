#ifndef SH_VKCHECK_H
#define SH_VKCHECK_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <vulkan/vulkan.h>

extern int shCheckValidationLayers(const char* validation_layer);

extern const char* shTranslateVkResult(const VkResult vk_result);

extern void shCheckVkResult(VkResult result, const char* error_msg);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VKCHECK_H