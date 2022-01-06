#include "shvulkan/shVkDrawLoop.h"
#include "shvulkan/shVkCore.h"
#include "shvulkan/shVkPipelineData.h"
#include "shvulkan/shVkMemoryInfo.h"

#include <assert.h>

void shFrameReset(const ShVkCore core) {
	vkWaitForFences(core.device, 1, &core.render_fence, 1, 1000000000);
	vkResetFences(core.device, 1, &core.render_fence);

	vkResetCommandBuffer(core.graphics_cmd_buffer, 0);
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

	vkBeginCommandBuffer(core.graphics_cmd_buffer, &cmdBufferBeginInfo);

	vkCmdBeginRenderPass(core.graphics_cmd_buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void shBindPipeline(const VkCommandBuffer graphics_cmd_buffer, const ShVkPipelineData pipe_data) {
	vkCmdBindPipeline(graphics_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe_data.pipeline);
}

void shBindVertexBuffer(const ShVkCore core, VkBuffer* p_vertex_buffer) {
	const VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(core.graphics_cmd_buffer, 0, 1, p_vertex_buffer, &offset);
}

void shBindIndexBuffer(const ShVkCore core, VkBuffer* p_index_buffer) {
	vkCmdBindIndexBuffer(core.graphics_cmd_buffer, *p_index_buffer, 0, VK_INDEX_TYPE_UINT32);
}

void shPushConstants(const VkCommandBuffer graphics_cmd_buffer, const ShVkPipelineData pipe_data, const void* p_push_constants_data) {
	if (pipe_data.push_constant_range.size > 0 || p_push_constants_data != NULL) {
		vkCmdPushConstants(graphics_cmd_buffer, pipe_data.main_pipeline_layout, pipe_data.push_constant_range.stageFlags, pipe_data.push_constant_range.offset, pipe_data.push_constant_range.size, p_push_constants_data);
	}
}

void shBindDescriptorSets(const ShVkCore core, ShVkPipelineData pipe_data) {
	vkUpdateDescriptorSets(core.device, 
		pipe_data.uniform_buffer_count, pipe_data.p_write_descriptor_sets, 
		0, NULL);
	vkCmdBindDescriptorSets(core.graphics_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipe_data.main_pipeline_layout, 
		0, pipe_data.uniform_buffer_count, pipe_data.p_descriptor_sets, 
		0, NULL);
}

void shDraw(const VkCommandBuffer graphics_cmd_buffer, const uint32_t count_div_stride, const uint8_t indexed) {

	if (indexed) {
		vkCmdDrawIndexed(graphics_cmd_buffer, count_div_stride, 1, 0, 0, 0);
	}
	else {
		vkCmdDraw(graphics_cmd_buffer, count_div_stride, 1, 0, 0);
	}
}

void shFrameEnd(const ShVkCore core, const uint32_t swapchain_image_index) {

	vkCmdEndRenderPass(core.graphics_cmd_buffer);
	vkEndCommandBuffer(core.graphics_cmd_buffer);

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
		&core.graphics_cmd_buffer,			//pCommandBuffers;
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
		&swapchain_image_index,				//pImageIndices;
		NULL								//pResults;
	};

	vkQueuePresentKHR(core.graphics_queue.queue, &presentInfo);
}