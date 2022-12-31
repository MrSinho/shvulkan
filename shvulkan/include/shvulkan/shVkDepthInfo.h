#ifndef SH_DEPTH_INFO_H
#define SH_DEPTH_INFO_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>

#define SH_DEPTH_IMAGE_FORMAT VK_FORMAT_D32_SFLOAT



#define shCreateDepthImage(p_core)\
	shCreateImage(\
		(p_core)->device,\
		VK_IMAGE_TYPE_2D,\
		(p_core)->surface.width, (p_core)->surface.height, 1,\
		SH_DEPTH_IMAGE_FORMAT, 1, (p_core)->sample_count, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,\
		&(p_core)->depth_image\
	)

#define shAllocateDepthMemory(p_core)\
	shAllocateImageMemory((p_core)->device, (p_core)->physical_device, (p_core)->depth_image, &(p_core)->depth_image_memory)

#define shBindDepthMemory(p_core)\
	shBindImageMemory((p_core)->device, (p_core)->depth_image, 0, (p_core)->depth_image_memory)



#define shInitDepthData(p_core)\
	shCreateDepthImage(p_core);\
	shAllocateDepthMemory(p_core);\
	shBindDepthMemory(p_core);\
	shCreateDepthImageView(p_core)//defined at shVkCore.h

#define shClearDepthBuffer(p_core)\
	shDestroyImageView((p_core)->device, (p_core)->depth_image_view);\
	shClearImageMemory((p_core)->device, (p_core)->depth_image, (p_core)->depth_image_memory)



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_DEPTH_INFO_H