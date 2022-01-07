#include "shvulkan/shVkDrawLoop.h"
#include "shvulkan/shVkCore.h"
#include "shvulkan/shVkPipelineData.h"
#include "shvulkan/shVkMemoryInfo.h"

#include <assert.h>

void shFrameReset(ShVkCore* p_core) {
	vkWaitForFences(p_core->device, 1, &p_core->render_fence, 1, 1000000000);
	vkResetFences(p_core->device, 1, &p_core->render_fence);

	vkResetCommandBuffer(p_core->graphics_cmd_buffer, 0);
}

void shFrameBegin(ShVkCore* p_core, uint32_t* p_swapchain_image_index) {
	assert(p_swapchain_image_index != NULL);
	vkAcquireNextImageKHR(p_core->device, p_core->swapchain, 1000000000, p_core->present_semaphore, 0, p_swapchain_image_index);

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

	vkBeginCommandBuffer(p_core->graphics_cmd_buffer, &cmdBufferBeginInfo);

	vkCmdBeginRenderPass(p_core->graphics_cmd_buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void shBindPipeline(ShVkCore* p_core, ShVkPipelineData* p_pipe_data) {
	vkCmdBindPipeline(p_core->graphics_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipe_data->pipeline);
}

void shBindVertexBuffer(ShVkCore* p_core, VkBuffer* p_vertex_buffer) {
	const VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(p_core->graphics_cmd_buffer, 0, 1, p_vertex_buffer, &offset);
}

void shBindIndexBuffer(ShVkCore* p_core, VkBuffer* p_index_buffer) {
	vkCmdBindIndexBuffer(p_core->graphics_cmd_buffer, *p_index_buffer, 0, VK_INDEX_TYPE_UINT32);
}

void shPushConstants(ShVkCore* p_core, const void* p_push_constants_data, ShVkPipelineData* p_pipe_data) {
	if (p_pipe_data->push_constant_range.size > 0 || p_push_constants_data != NULL) {
		vkCmdPushConstants(p_core->graphics_cmd_buffer, p_pipe_data->main_pipeline_layout, p_pipe_data->push_constant_range.stageFlags, p_pipe_data->push_constant_range.offset, p_pipe_data->push_constant_range.size, p_push_constants_data);
	}
}

void shBindDescriptorSets(ShVkCore* p_core, ShVkPipelineData* p_pipe_data) {
	vkUpdateDescriptorSets(p_core->device, 
		p_pipe_data->uniform_buffer_count, p_pipe_data->write_descriptor_sets, 
		0, NULL);
	vkCmdBindDescriptorSets(p_core->graphics_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
		p_pipe_data->main_pipeline_layout, 
		0, p_pipe_data->uniform_buffer_count, p_pipe_data->descriptor_sets, 
		0, NULL);
}

void shDraw(ShVkCore* p_core, const uint32_t count_div_stride, const uint8_t indexed) {

	if (indexed) {
		vkCmdDrawIndexed(p_core->graphics_cmd_buffer, count_div_stride, 1, 0, 0, 0);
	}
	else {
		vkCmdDraw(p_core->graphics_cmd_buffer, count_div_stride, 1, 0, 0);
	}
}

void shFrameEnd(ShVkCore* p_core, const uint32_t swapchain_image_index) {

	vkCmdEndRenderPass(p_core->graphics_cmd_buffer);
	vkEndCommandBuffer(p_core->graphics_cmd_buffer);

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	//submit to GPU queue
	// wait for the presentsemaphore, signal to the render semaphore
	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,	//sType;
		NULL,							//pNext;
		1,								//waitSemaphoreCount;
		&p_core->present_semaphore,		//pWaitSemaphores;
		&waitStage,						//pWaitDstStageMask;
		1,								//commandBufferCount;
		&p_core->graphics_cmd_buffer,	//pCommandBuffers;
		1,								//signalSemaphoreCount;
		&p_core->render_semaphore,		//pSignalSemaphores;
	};

	vkQueueSubmit(p_core->graphics_queue.queue, 1, &submitInfo, p_core->render_fence);

	// Present the ready image
	VkPresentInfoKHR presentInfo = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,	//sType;
		NULL,								//pNext;
		1,									//waitSemaphoreCount;
		&p_core->render_semaphore, 			//pWaitSemaphores;
		1,									//swapchainCount;
		&p_core->swapchain,					//pSwapchains;
		&swapchain_image_index,				//pImageIndices;
		NULL								//pResults;
	};

	vkQueuePresentKHR(p_core->graphics_queue.queue, &presentInfo);
}