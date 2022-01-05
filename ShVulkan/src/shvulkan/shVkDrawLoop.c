#include "shvulkan/shVkDrawLoop.h"
#include "shvulkan/shVkCore.h"
#include "shvulkan/shVkPipelineData.h"
#include "shvulkan/shVkMemoryInfo.h"

#include <assert.h>

void shFrameReset(const ShVkCore core) {
	vkWaitForFences(core.device, 1, &core.render_fence, 1, 1000000000);
	vkResetFences(core.device, 1, &core.render_fence);

	vkResetCommandBuffer(core.cmd_buffers[0], 0);
}

void shFrameBegin(const ShVkCore core, uint32_t* p_swapchain_image_index) {
	assert(p_swapchain_image_index != NULL);
	vkAcquireNextImageKHR(core.device, core.swapchain, 1000000000, core.present_semaphore, 0, p_swapchain_image_index);

	VkCommandBufferBeginInfo cmdBufferBeginInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	//sType;
		NULL,											//pNext;
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,	//flags; //submit cmd buffer once, before the next reset
		NULL											//pInheritanceInfo;
	};

	VkClearValue clear_values[2];
	clear_values[0].color = (VkClearColorValue){ 0.1f, 0.1f, 0.1f };
	clear_values[1].depthStencil = (VkClearDepthStencilValue){ 1.0f, 0 };
	VkRenderPassBeginInfo renderPassBeginInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,		//sType;
		NULL,											//pNext;
		core.render_pass,								//renderPass;
		core.p_frame_buffers[*p_swapchain_image_index],	//framebuffer;
		{												//
			{0, 0},										//
			{core.surface.width, core.surface.height}	//
		},												//renderArea;
		1,												//clearValueCount;
		clear_values									//pClearValues;
	};
	if (core.depth_image_view != VK_NULL_HANDLE) {
		renderPassBeginInfo.clearValueCount = 2;
	}

	vkBeginCommandBuffer(core.cmd_buffers[0], &cmdBufferBeginInfo);

	vkCmdBeginRenderPass(core.cmd_buffers[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void shBindPipeline(const VkCommandBuffer graphicsCmdBuffer, const ShVkPipelineData pipeData) {
	vkCmdBindPipeline(graphicsCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeData.pipeline);
}

void shBindVertexBuffer(const ShVkCore core, VkBuffer* p_vertex_buffer) {
	const VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(core.cmd_buffers[0], 0, 1, p_vertex_buffer, &offset);
}

void shBindIndexBuffer(const ShVkCore core, VkBuffer* p_index_buffer) {
	vkCmdBindIndexBuffer(core.cmd_buffers[0], *p_index_buffer, 0, VK_INDEX_TYPE_UINT32);
}

void shPushConstants(const VkCommandBuffer graphicsCmdBuffer, const ShVkPipelineData pipeData, const void* pPushConstantsData) {
	if (pipeData.push_constant_range.size > 0 || pPushConstantsData != NULL) {
		vkCmdPushConstants(graphicsCmdBuffer, pipeData.main_pipeline_layout, pipeData.push_constant_range.stageFlags, pipeData.push_constant_range.offset, pipeData.push_constant_range.size, pPushConstantsData);
	}
}

void shBindDescriptorSets(const ShVkCore core, ShVkPipelineData pipeData) {
	vkUpdateDescriptorSets(core.device, 
		pipeData.uniform_buffer_count, pipeData.p_write_descriptor_sets, 
		0, NULL);
	vkCmdBindDescriptorSets(core.cmd_buffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipeData.main_pipeline_layout, 
		0, pipeData.uniform_buffer_count, pipeData.p_descriptor_sets, 
		0, NULL);
}

void shDraw(const VkCommandBuffer graphicsCmdBuffer, const uint32_t count_ds, const uint8_t indexed) {

	if (indexed) {
		vkCmdDrawIndexed(graphicsCmdBuffer, count_ds, 1, 0, 0, 0);
	}
	else {
		vkCmdDraw(graphicsCmdBuffer, count_ds, 1, 0, 0);
	}
}

void shFrameEnd(const ShVkCore core, const uint32_t swapchainImageIndex) {

	vkCmdEndRenderPass(core.cmd_buffers[0]);
	vkEndCommandBuffer(core.cmd_buffers[0]);

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	//submit to GPU queue
	// wait for the presentsemaphore, signal to the render semaphore
	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,	//sType;
		NULL,							//pNext;
		1,								//waitSemaphoreCount;
		&core.present_semaphore,		//pWaitSemaphores;
		&waitStage,						//pWaitDstStageMask;
		1,								//commandBufferCount;
		&core.cmd_buffers[0],			//pCommandBuffers;
		1,								//signalSemaphoreCount;
		&core.render_semaphore,			//pSignalSemaphores;
	};

	vkQueueSubmit(core.graphics_queue.queue, 1, &submitInfo, core.render_fence);

	// Present the ready image
	VkPresentInfoKHR presentInfo = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,	//sType;
		NULL,								//pNext;
		1,									//waitSemaphoreCount;
		&core.render_semaphore, 			//pWaitSemaphores;
		1,									//swapchainCount;
		&core.swapchain,					//pSwapchains;
		&swapchainImageIndex,				//pImageIndices;
		NULL								//pResults;
	};

	vkQueuePresentKHR(core.graphics_queue.queue, &presentInfo);
}