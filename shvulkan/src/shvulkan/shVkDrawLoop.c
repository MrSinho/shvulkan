#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shvulkan/shVkDrawLoop.h"
#include "shvulkan/shVkCore.h"
#include "shvulkan/shVkPipelineData.h"
#include "shvulkan/shVkMemoryInfo.h"
#include "shvulkan/shVkCheck.h"



void shFrameReset(ShVkCore* p_core) {
	vkWaitForFences(p_core->device, 1, &p_core->p_graphics_commands[0].fence, 1, 1000000000);
	vkResetFences(p_core->device, 1, &p_core->p_graphics_commands[0].fence);

	vkResetCommandBuffer(p_core->p_graphics_commands[0].cmd_buffer, 0);
}

void shFrameBegin(ShVkCore* p_core, uint32_t* p_swapchain_image_index) {
	shVkAssert(p_swapchain_image_index != NULL, "invalid pointer to swapchain image index");
	vkAcquireNextImageKHR(p_core->device, p_core->swapchain, 1000000000, p_core->p_render_semaphores[0], 0, p_swapchain_image_index);

	VkCommandBufferBeginInfo cmdBufferBeginInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	//sType;
		NULL,											//pNext;
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,	//flags; //submit cmd buffer once, before the next reset
		NULL											//pInheritanceInfo;
	};

	VkClearValue clear_values[2];
	clear_values[0].color = (VkClearColorValue){ 0.0f, 0.0f, 0.0f };
	clear_values[1].depthStencil = (VkClearDepthStencilValue){ 1.0f, 0 };
	VkRenderPassBeginInfo renderPassBeginInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,		//sType;
		NULL,											//pNext;
		p_core->render_pass,								//renderPass;
		p_core->p_frame_buffers[*p_swapchain_image_index],	//framebuffer;
		{												//
			{0, 0},										//
			{p_core->surface.width, p_core->surface.height}	//
		},												//renderArea;
		1,												//clearValueCount;
		clear_values									//pClearValues;
	};
	if (p_core->depth_image_view != VK_NULL_HANDLE) {
		renderPassBeginInfo.clearValueCount = 2;
	}

	vkBeginCommandBuffer(p_core->p_graphics_commands[0].cmd_buffer, &cmdBufferBeginInfo);

	vkCmdBeginRenderPass(p_core->p_graphics_commands[0].cmd_buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void shFrameEnd(ShVkCore* p_core, const uint32_t swapchain_image_index) {

	vkCmdEndRenderPass(p_core->p_graphics_commands[0].cmd_buffer);
	vkEndCommandBuffer(p_core->p_graphics_commands[0].cmd_buffer);

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,	//sType;
		NULL,							//pNext;
		1,								//waitSemaphoreCount;
		&p_core->p_render_semaphores[0],//pWaitSemaphores;
		&waitStage,						//pWaitDstStageMask;
		1,								//commandBufferCount;
		&p_core->p_graphics_commands[0].cmd_buffer,	//pCommandBuffers;
		1,								//signalSemaphoreCount;
		&p_core->p_render_semaphores[0],//pSignalSemaphores;
	};

	vkQueueSubmit(p_core->graphics_queue.queue, 1, &submitInfo, p_core->p_graphics_commands[0].fence);

	VkPresentInfoKHR presentInfo = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,	//sType;
		NULL,								//pNext;
		1,									//waitSemaphoreCount;
		&p_core->p_render_semaphores[0], 			//pWaitSemaphores;
		1,									//swapchainCount;
		&p_core->swapchain,					//pSwapchains;
		&swapchain_image_index,				//pImageIndices;
		NULL								//pResults;
	};

	vkQueuePresentKHR(p_core->graphics_queue.queue, &presentInfo);
}

#ifdef __cplusplus
}
#endif//__cplusplus