#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkDrawLoop.h"
#include "shvulkan/shVkCore.h"
#include "shvulkan/shVkPipelineData.h"
#include "shvulkan/shVkMemoryInfo.h"
#include "shvulkan/shVkCheck.h"



void shAcquireNextImage(ShVkCore* p_core, VkSemaphore semaphore, uint32_t* p_swapchain_image_idx) {
	shVkError(p_swapchain_image_idx == NULL, "invalid pointer to swapchain image index", return);
	vkAcquireNextImageKHR(p_core->device, p_core->swapchain, UINT64_MAX, semaphore, 0, p_swapchain_image_idx);
}

void shBeginRenderPass(ShVkCore* p_core, VkCommandBuffer cmd_buffer, VkClearColorValue clear_color, uint32_t swapchain_image_idx) {
	VkClearValue clear_values[2] = { 0 };
	clear_values[0].color = clear_color;
	clear_values[1].depthStencil = (VkClearDepthStencilValue){ 1.0f, 0 };
	VkRenderPassBeginInfo renderPassBeginInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,           //sType;
		NULL,                                               //pNext;
		p_core->render_pass,                                //renderPass;
		p_core->p_frame_buffers[swapchain_image_idx],       //framebuffer;
		{                                                   //
			{0, 0},                                         //
			{p_core->surface.width, p_core->surface.height} //
		},                                                  //renderArea;
		1,                                                  //clearValueCount;
		clear_values                                        //pClearValues;
	};
	if (p_core->depth_image_view != VK_NULL_HANDLE) {
		renderPassBeginInfo.clearValueCount = 2;
	}

	vkCmdBeginRenderPass(cmd_buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

//void shFrameEnd(ShVkCore* p_core, VkCommandBuffer cmd_buffer, VkFence fence, VkSemaphore semaphore, const uint32_t swapchain_image_idx) {

void shGraphicsQueueSubmit(ShVkCore* p_core, VkCommandBuffer cmd_buffer, VkSemaphore semaphore, VkFence fence) {

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO, //sType;
		NULL,                          //pNext;
		1,                             //waitSemaphoreCount;
		&semaphore,                    //pWaitSemaphores;
		&waitStage,                    //pWaitDstStageMask;
		1,                             //commandBufferCount;
		&cmd_buffer,                   //pCommandBuffers;
		1,                             //signalSemaphoreCount;
		&semaphore,                    //pSignalSemaphores;
	};

	vkQueueSubmit(p_core->graphics_queue.queue, 1, &submitInfo, fence);

}

void shPresentImage(ShVkCore* p_core, VkSemaphore semaphore, const uint32_t swapchain_image_idx) {
	VkPresentInfoKHR presentInfo = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           //sType;
		NULL,                                         //pNext;
		1,                                            //waitSemaphoreCount;
		&semaphore,                                   //pWaitSemaphores;
		1,                                            //swapchainCount;
		&p_core->swapchain,                           //pSwapchains;
		&swapchain_image_idx,                         //pImageIndices;
		NULL                                          //pResults;
	};

	vkQueuePresentKHR(p_core->graphics_queue.queue, &presentInfo);
}



#ifdef __cplusplus
}
#endif//__cplusplus