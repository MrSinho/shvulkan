#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDrawLoop.h>
#include <shvulkan/shVkCheck.h>
#include <shvulkan/shVkDescriptorStructureMap.h>



#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include <math.h>



#ifndef NDEBUG
#define VALIDATION_LAYERS_ENABLED 1
#else
#define VALIDATION_LAYERS_ENABLED 0
#endif//NDEBUG



void setupPipeline(VkDevice device, VkPhysicalDevice physical_device, uint32_t width, uint32_t height, VkRenderPass render_pass, ShVkFixedStates* p_fixed_states, ShVkPipeline* p_pipeline);

GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title);

const char* readBinary(const char* path, uint32_t* p_size);



#ifndef alignas
#include "../../external/stdalign.in.h"
#endif//alignas



typedef struct Model {
	alignas(16) float model[4][4];
} Model;
SH_VULKAN_GENERATE_DESCRIPTOR_STRUCTURE_MAP(Model) //required only for dynamic descriptors



typedef struct Light {
	alignas(16) float position[4];
	alignas(16) float color[4];
} Light;



#define THREAD_COUNT 1

#define QUAD_VERTEX_COUNT 32
#define QUAD_INDEX_COUNT 6



int main(void) {

	ShVkCore core = { 0 };
	const char* application_name = "shvulkan example";
	uint32_t width = 720;
	uint32_t height = 480;
	GLFWwindow* window = createWindow(width, height, application_name);
	{
		uint32_t instance_extension_count = 0;
		const char** pp_instance_extensions = glfwGetRequiredInstanceExtensions(&instance_extension_count);
		shCreateInstance(&core, application_name, "shvulkan engine", VALIDATION_LAYERS_ENABLED, instance_extension_count, pp_instance_extensions);
		glfwCreateWindowSurface(core.instance, window, NULL, &core.surface.surface);
		core.surface.width = width;
		core.surface.height = height;

		shSelectPhysicalDevice(&core, VK_QUEUE_GRAPHICS_BIT);
		shSetLogicalDevice(&core);
		shInitSwapchainData(&core);
		shInitDepthData(&core);
		shCreateRenderPass(&core);
		shSetFramebuffers(&core);
		shCreateGraphicsCommandBuffers(&core, THREAD_COUNT);
		shSetSyncObjects(&core);
		shGetGraphicsQueue(&core);
	}

	VkDevice			device				= core.device;
	VkPhysicalDevice	physical_device		= core.physical_device;
	VkCommandBuffer		cmd_buffer			= core.p_graphics_commands[0].cmd_buffer;
	VkFence				fence				= core.p_graphics_commands[0].fence;

	{
		uint32_t available_vram, process_used_vram = 0;
		shGetMemoryBudgetProperties(core.physical_device, &available_vram, &process_used_vram, NULL);
	}


	float quad[QUAD_VERTEX_COUNT] = {
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};
	uint32_t indices[QUAD_INDEX_COUNT] = {
		0, 1, 2,
		2, 3, 0
	};
	VkBuffer quad_vertex_buffer, quad_index_buffer;
	VkDeviceMemory quad_vertex_buffer_memory, quad_index_buffer_memory;
	{
		VkBuffer vertex_staging_buffer = NULL;
		VkBuffer index_staging_buffer = NULL;
		VkDeviceMemory vertex_staging_buffer_memory = NULL;
		VkDeviceMemory index_staging_buffer_memory = NULL;
		{
			uint32_t buffer_size = QUAD_VERTEX_COUNT * 4;

			shCreateBuffer(device, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &vertex_staging_buffer);
			shAllocateMemory(device, core.physical_device, vertex_staging_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vertex_staging_buffer_memory);
			shWriteMemory(device, vertex_staging_buffer_memory, 0, buffer_size, quad);
			shBindMemory(device, vertex_staging_buffer, 0, vertex_staging_buffer_memory);

			shCreateVertexBuffer(device, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, &quad_vertex_buffer);
			shAllocateVertexBufferMemory(device, core.physical_device, quad_vertex_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &quad_vertex_buffer_memory);
			shBindVertexBufferMemory(device, quad_vertex_buffer, 0, quad_vertex_buffer_memory);
		}

		{
			uint32_t buffer_size = QUAD_INDEX_COUNT * 4;

			shCreateBuffer(device, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &index_staging_buffer);
			shAllocateMemory(device, core.physical_device, index_staging_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &index_staging_buffer_memory);
			shWriteMemory(device, index_staging_buffer_memory, 0, buffer_size, indices);
			shBindMemory(device, index_staging_buffer, 0, index_staging_buffer_memory);

			shCreateIndexBuffer(device, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, &quad_index_buffer);
			shAllocateIndexBufferMemory(device, core.physical_device, quad_index_buffer, &quad_index_buffer_memory);
			shBindIndexBufferMemory(device, quad_index_buffer, 0, quad_index_buffer_memory);
		}

		{
			shResetFence(device, &fence);

			shBeginCommandBuffer(cmd_buffer);
			shCopyBuffer(cmd_buffer, vertex_staging_buffer, 0, 0, QUAD_VERTEX_COUNT * 4, quad_vertex_buffer);
			shCopyBuffer(cmd_buffer, index_staging_buffer, 0, 0, QUAD_INDEX_COUNT * 4, quad_index_buffer);
			shEndCommandBuffer(cmd_buffer);

			shQueueSubmit(1, &cmd_buffer, core.graphics_queue.queue, fence);
			shWaitForFence(device, &fence);

			shClearBufferMemory(device, vertex_staging_buffer, vertex_staging_buffer_memory);
			shClearBufferMemory(device, index_staging_buffer, index_staging_buffer_memory);
		}


	}

	VkBuffer triangle_vertex_buffer = NULL;
	VkDeviceMemory triangle_vertex_buffer_memory = NULL;
#define TRIANGLE_VERTEX_COUNT 24
	float triangle[TRIANGLE_VERTEX_COUNT] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};
	{
		const uint32_t buffer_size = TRIANGLE_VERTEX_COUNT * 4;
		shCreateVertexBuffer(device, buffer_size, 0, &triangle_vertex_buffer);
		shAllocateVertexBufferMemory(device, physical_device, triangle_vertex_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &triangle_vertex_buffer_memory);
		shWriteVertexBufferMemory(device, triangle_vertex_buffer_memory, 0, buffer_size, triangle);
		shBindVertexBufferMemory(device, triangle_vertex_buffer, 0, triangle_vertex_buffer_memory);
	}
	
	float projection[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	float view[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	void* push_constants_data[128 / sizeof(void*)];
	memcpy(push_constants_data, projection, 64);
	memcpy(&push_constants_data[64 / sizeof(void*)], view, 64);


	float model0_data[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	float model1_data[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	ModelDescriptorStructureMap model_map = shVkCreateModelDescriptorStructures(core.physical_device_properties, 2);
	for (uint32_t i = 0; i < model_map.structure_count; i++) {
		Model* p_model = shVkGetModelDescriptorStructure(model_map, i, 0);
		if (i == 0) {
			memcpy(p_model->model, model0_data, 64);
		}
		else {
			memcpy(p_model->model, model1_data, 64);
		}
	}
	shVkMapModelDecriptorStructures(&model_map);

	Light light = {
		{ 0.0f,  2.0f, 0.0f, 1.0f }, //light position
		{ 0.0f, 0.45f, 0.9f, 1.0f }	 //light color
	};


	ShVkPipeline pipeline			= { 0 };
	ShVkFixedStates	fixed_states	= { 0 };
	setupPipeline(
		core.device, 
		core.physical_device, 
		core.surface.width,
		core.surface.height,
		core.render_pass,
		&fixed_states,
		&pipeline
	);


	uint32_t frame_index = 0;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glfwGetWindowSize(window, &width, &height);
		if (width != core.surface.width || height != core.surface.height) {
			shWaitDeviceIdle(device);

			shSwapchainRelease(&core);
			shSurfaceRelease(&core);
			shDepthBufferRelease(&core);

			glfwCreateWindowSurface(core.instance, window, NULL, &core.surface.surface);
			core.surface.width = width;
			core.surface.height = height;
			shInitSwapchainData(&core);
			shInitDepthData(&core);
			shSetFramebuffers(&core);

			shPipelineClearDescriptorBufferMemory(device, 0, &pipeline);
			shPipelineClearDescriptorBufferMemory(device, 1, &pipeline);
			shPipelineRelease(device, &pipeline);
			shFixedStatesRelease(&fixed_states);

			setupPipeline(device, physical_device, width, height, core.render_pass, &fixed_states, &pipeline);
		}

		shFrameReset(&core, 0);

		shFrameBegin(
			&core, 
			0, 
			(VkClearColorValue){ { 0.1f, 0.1f , 0.1f, 1.0f } }, 
			&frame_index
		);

		shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);

		shPipelinePushConstants(cmd_buffer, push_constants_data, &pipeline);

		shPipelineUpdateDescriptorSets(device, &pipeline);

		shPipelineWriteDescriptorBufferMemory(device, 0, 0, 0, &light, &pipeline);
		shPipelineBindDescriptorSet(cmd_buffer, 0, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);
		
		
		Model* p_model0 = shVkGetModelDescriptorStructure(model_map, 0, 1);
		shPipelineWriteDynamicDescriptorBufferMemory(device, 1, p_model0->model, &pipeline);
		shPipelineBindDynamicDescriptorSet(cmd_buffer, 1, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);
		
		shBindVertexBuffer(cmd_buffer, 0, 0, &quad_vertex_buffer);
		shBindIndexBuffer(cmd_buffer, 0, &quad_index_buffer);
		shDrawIndexed(cmd_buffer, QUAD_INDEX_COUNT);
		
		Model* p_model1 = shVkGetModelDescriptorStructure(model_map, 1, 1);
		shPipelineWriteDynamicDescriptorBufferMemory(device, 1, p_model1->model, &pipeline);
		shPipelineBindDynamicDescriptorSet(cmd_buffer, 1, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);
		
		triangle[9] = (float)sin(glfwGetTime());
		shWriteVertexBufferMemory(device, triangle_vertex_buffer_memory, 0, TRIANGLE_VERTEX_COUNT * 4, triangle);
		shBindVertexBuffer(cmd_buffer, 0, 0, &triangle_vertex_buffer);
		shDraw(cmd_buffer, TRIANGLE_VERTEX_COUNT / (fixed_states.vertex_binding_description.stride / 4));

		shEndPipeline(&pipeline);
		shFrameEnd(&core, 0, frame_index);
	}
	glfwTerminate();

	shVkReleaseModelDescriptorStructureMap(&model_map);

	shPipelineClearDescriptorBufferMemory(device, 0, &pipeline);
	shPipelineClearDescriptorBufferMemory(device, 1, &pipeline);
	
	shPipelineRelease(device, &pipeline);

	shClearVertexBufferMemory(device, triangle_vertex_buffer, triangle_vertex_buffer_memory);
	shClearVertexBufferMemory(device, quad_vertex_buffer, quad_vertex_buffer_memory);
	shClearIndexBufferMemory(device, quad_index_buffer, quad_index_buffer_memory);

	shVulkanRelease(&core);

	return 0;
}


void setupPipeline(VkDevice device, VkPhysicalDevice physical_device, uint32_t width, uint32_t height, VkRenderPass render_pass, ShVkFixedStates* p_fixed_states, ShVkPipeline* p_pipeline) {
	assert(p_pipeline != NULL && p_fixed_states != NULL);

	uint32_t memory_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	
	shSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, 128, &p_pipeline->push_constant_range);
	
	shPipelineCreateDescriptorBuffer(device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 0, sizeof(Light), p_pipeline);
	shPipelineCreateDynamicDescriptorBuffer(device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 1, sizeof(Model), 2, p_pipeline);
	
	shPipelineAllocateDescriptorBufferMemory(device, physical_device, memory_flags, 0, p_pipeline);
	shPipelineAllocateDescriptorBufferMemory(device, physical_device, memory_flags, 1, p_pipeline);
	
	shPipelineBindDescriptorBufferMemory(device, 0, 0, p_pipeline);
	shPipelineBindDescriptorBufferMemory(device, 1, 0, p_pipeline);
	
	shPipelineDescriptorSetLayout(device, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, p_pipeline);
	shPipelineDescriptorSetLayout(device, 1, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, p_pipeline);
	
	shPipelineCreateDescriptorPool(device, 0, p_pipeline);
	shPipelineCreateDescriptorPool(device, 1, p_pipeline);
	
	shPipelineAllocateDescriptorSet(device, 0, p_pipeline);
	shPipelineAllocateDescriptorSet(device, 1, p_pipeline);
	
	uint32_t vertex_shader_size = 0;
	uint32_t fragment_shader_size = 0;
	char* vertex_code = (char*)readBinary("../examples/shaders/bin/mesh.vert.spv", &vertex_shader_size);
	char* fragment_code = (char*)readBinary("../examples/shaders/bin/mesh.frag.spv", &fragment_shader_size);
	shPipelineCreateShaderModule(device, vertex_shader_size, vertex_code, p_pipeline);
	shPipelineCreateShaderStage(device, VK_SHADER_STAGE_VERTEX_BIT, p_pipeline);
	
	shPipelineCreateShaderModule(device, fragment_shader_size, fragment_code, p_pipeline);
	shPipelineCreateShaderStage(device, VK_SHADER_STAGE_FRAGMENT_BIT, p_pipeline);
	
	free(vertex_code);
	free(fragment_code);
	
	shSetVertexInputAttribute(0, SH_VEC3_SIGNED_FLOAT, 0, 12, p_fixed_states);
	shSetVertexInputAttribute(1, SH_VEC3_SIGNED_FLOAT, 12, 12, p_fixed_states);
	shSetVertexInputAttribute(2, SH_VEC2_SIGNED_FLOAT, 24, 8, p_fixed_states);
	
	shFixedStatesSetVertexInputRate(VK_VERTEX_INPUT_RATE_VERTEX, 0, p_fixed_states);
	shFixedStatesSetVertexInputState(p_fixed_states);
	
	shSetFixedStates(device, width, height, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, p_fixed_states);
	shSetupGraphicsPipeline(device, render_pass, *p_fixed_states, p_pipeline);
}



GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title) {
	shVkError(!glfwInit(), "error initializing glfw", return NULL);
	shVkError(glfwVulkanSupported() == GLFW_FALSE, "vulkan not supported by glfw", return NULL);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	return glfwCreateWindow(width, height, title, NULL, NULL);
}



#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER



const char* readBinary(const char* path, uint32_t* p_size) {
	FILE* stream = fopen(path, "rb");
	if (stream == NULL) {
		return NULL;
	}
	fseek(stream, 0, SEEK_END);
	uint32_t code_size = ftell(stream);
	fseek(stream, 0, SEEK_SET);
	char* code = (char*)calloc(1, code_size);
	if (code == NULL) {
		fclose(stream);
		return NULL;
	}
	fread(code, code_size, 1, stream);
	*p_size = code_size;
	fclose(stream);
	return code;
}



#ifdef __cplusplus
}
#endif//__cplusplus