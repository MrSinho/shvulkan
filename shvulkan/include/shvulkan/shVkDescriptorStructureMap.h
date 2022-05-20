#ifndef SH_VK_DESCRIPTOR_STRUCTURE_MAP_H
#define SH_VK_DESCRIPTOR_STRUCTURE_MAP_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkMemoryInfo.h"
#include "shvulkan/shVkCheck.h"

#include <memory.h>
#include <stdlib.h>

#define SH_VULKAN_GENERATE_DESCRIPTOR_STRUCTURE_MAP(STRUCT)\
typedef struct STRUCT##DescriptorStructureMap {\
	uint32_t	structure_count;\
	uint32_t	structure_size;\
	STRUCT**	pp_##STRUCT;\
	void*		p_##STRUCT##_map;\
} STRUCT##DescriptorStructureMap;\
static uint32_t shVkGet ## STRUCT ## DescriptorStructureSize(const VkPhysicalDeviceProperties physical_device_properties) {\
	if ((uint32_t)(sizeof(STRUCT)) < (uint32_t)physical_device_properties.limits.minUniformBufferOffsetAlignment) {\
		return (uint32_t)physical_device_properties.limits.minUniformBufferOffsetAlignment;\
	}\
	else {\
		uint32_t size = (uint32_t)sizeof(STRUCT);\
		for (; size > (uint32_t)physical_device_properties.limits.minUniformBufferOffsetAlignment;) {\
			size -= (uint32_t)physical_device_properties.limits.minUniformBufferOffsetAlignment;\
		}\
		return (uint32_t)(sizeof(STRUCT)) + (uint32_t)physical_device_properties.limits.minUniformBufferOffsetAlignment - size;\
	}\
}\
static STRUCT##DescriptorStructureMap shVkCreate ## STRUCT ## DescriptorStructures(const VkPhysicalDeviceProperties physical_device_properties, const uint32_t structure_count) {\
	STRUCT##DescriptorStructureMap map = { structure_count };\
	map.pp_##STRUCT = calloc(structure_count, sizeof(STRUCT*));\
	for (uint32_t i = 0; i < map.structure_count; i++) {\
		map.pp_##STRUCT[i] = (STRUCT*)calloc(1, sizeof(STRUCT));\
		shVkAssert(map.pp_##STRUCT[i] != NULL, "invalid descriptor structure pointer");\
	}\
	shVkAssert(map.pp_##STRUCT != NULL, "invalid descriptor structure ppointer");\
	map.structure_size = shVkGet## STRUCT ##DescriptorStructureSize(physical_device_properties);\
	map.p_##STRUCT##_map = (STRUCT*)calloc(structure_count, map.structure_size);\
	shVkAssert(map.p_##STRUCT##_map != NULL, "invalid descriptor structure map pointer");\
	return map;\
}\
static STRUCT* shVkGet ## STRUCT ## DescriptorStructure(const STRUCT##DescriptorStructureMap map, const uint32_t idx, const uint8_t mapped) {\
	return mapped ? (STRUCT*)(&((char*)map.p_##STRUCT##_map)[idx * map.structure_size]) : map.pp_##STRUCT[idx];\
}\
static void shVkMap ## STRUCT ## DecriptorStructures(STRUCT##DescriptorStructureMap* p_map) {\
	shVkAssert(p_map != NULL, "invalid descriptor structure map handle pointer");\
	for (uint32_t i = 0; i < p_map->structure_count; i++) {\
		memcpy(&((char*)p_map->p_##STRUCT##_map)[i * p_map->structure_size], p_map->pp_##STRUCT[i], sizeof(STRUCT));\
	}\
}\
static void shVkRelease ## STRUCT ## DescriptorStructureMap(STRUCT##DescriptorStructureMap* p_map) {\
	shVkAssert(p_map != NULL, "invalid descriptor structure map pointer");\
	p_map->structure_size = 0;\
	p_map->structure_count = 0;\
	free(p_map->pp_##STRUCT);\
	free(p_map->p_##STRUCT##_map);\
}


#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VK_DESCRIPTOR_STRUCTURE_MAP_H