#ifndef SH_OUTPUT_INFO_H
#define SH_OUTPUT_INFO_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>


//the resolved image is not multisampled
#define shSetOutputImage(p_core)\
	shCreateImage(\
        (p_core)->device,\
		VK_IMAGE_TYPE_2D,\
		(p_core)->surface.width, (p_core)->surface.height, 1,\
		(p_core)->swapchain_image_format, 1, (p_core)->sample_count,\
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,\
		&(p_core)->output_image\
	) &&\
	shAllocateImageMemory((p_core)->device, (p_core)->physical_device, (p_core)->output_image, &(p_core)->output_memory) &&\
	shBindImageMemory((p_core)->device, (p_core)->output_image, 0, (p_core)->output_memory) &&\
	shCreateImageView(p_core,\
		(p_core)->output_image,\
		VK_IMAGE_VIEW_TYPE_2D,\
		VK_IMAGE_ASPECT_COLOR_BIT,\
		1,\
		(p_core)->swapchain_image_format,\
		&(p_core)->output_image_view\
	)

#define shClearOutputImage(p_core)\
    shClearImageMemory((p_core)->device, (p_core)->output_image, (p_core)->output_memory);\
	shDestroyImageView((p_core)->device, (p_core)->output_image_view);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_OUTPUT_INFO_H
