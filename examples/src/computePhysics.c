#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDrawLoop.h>
#include <shvulkan/shVkCheck.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include <math.h>

#ifndef NDEBUG
#define VALIDATION_LAYERS_ENABLED 1
#else
#define VALIDATION_LAYERS_ENABLED 0
#endif//NDEBUG

GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title);

const char* readBinary(const char* path, uint32_t* p_size);


int main(void) {

	const char* application_name = "shvulkan compute physics";
	const uint32_t width = 720;
	const uint32_t height = 720;
	GLFWwindow* window = createWindow(width, height, application_name);

	ShVkCore core = { 0 };
	uint32_t extension_count = 2;
	const char** extension_names = glfwGetRequiredInstanceExtensions(&extension_count);
	shCreateInstance(&core, application_name, "shvulkan engine", VALIDATION_LAYERS_ENABLED, extension_count, extension_names);
	shVkAssertResult(
		glfwCreateWindowSurface(core.instance, window, NULL, &core.surface.surface),
		"error creating window surface"
	);
	core.surface.width = width;
	core.surface.height = height;
	shSelectPhysicalDevice(&core, SH_VK_CORE_GRAPHICS | SH_VK_CORE_COMPUTE);
	shSetLogicalDevice(&core, SH_VK_CORE_GRAPHICS | SH_VK_CORE_COMPUTE);
	shGetGraphicsQueue(&core);
	shInitSwapchainData(&core);
	shInitDepthData(&core);
	shCreateRenderPass(&core);
	shSetFramebuffers(&core);
	shSetSyncObjects(&core);
	shCreateCommandBuffers(&core, SH_VK_CORE_GRAPHICS | SH_VK_CORE_COMPUTE, 2);
	
	float canvas[12] = {
		-0.5f,-0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	uint32_t canvas_indices[6] = {
		0, 1, 2,
		2, 3, 0
	};
	VkBuffer canvas_vertex_buffer, canvas_index_buffer;
	VkDeviceMemory canvas_vertex_buffer_memory, canvas_index_buffer_memory;
	shCreateVertexBuffer(core.device, 12 * 4, &canvas_vertex_buffer);
	shCreateIndexBuffer(core.device, 6 * 4, &canvas_index_buffer);
	
	shAllocateVertexBufferMemory(core.device, core.physical_device, canvas_vertex_buffer, &canvas_vertex_buffer_memory);
	shAllocateIndexBufferMemory(core.device, core.physical_device, canvas_index_buffer, &canvas_index_buffer_memory);
	
	shBindVertexBufferMemory(core.device, canvas_vertex_buffer, canvas_vertex_buffer_memory);
	shBindIndexBufferMemory(core.device, canvas_index_buffer, canvas_index_buffer_memory);

	shWriteVertexBufferMemory(core.device, canvas_vertex_buffer_memory, 12 * 4, canvas);
	shWriteIndexBufferMemory(core.device, canvas_index_buffer_memory, 6 * 4, canvas_indices);
	
	ShVkPipeline graphics_pipeline = { 0 };
	{
		uint32_t vertex_shader_size = 0;
		uint32_t fragment_shader_size = 0;
		const char* vertex_code = readBinary("../examples/shaders/bin/Canvas.vert.spv", &vertex_shader_size);
		const char* fragment_code = readBinary("../examples/shaders/bin/Canvas.frag.spv", &fragment_shader_size);
		shPipelineCreateShaderModule(core.device, vertex_shader_size, vertex_code, &graphics_pipeline);
		shPipelineCreateShaderStage(core.device, VK_SHADER_STAGE_VERTEX_BIT, &graphics_pipeline);

		shPipelineCreateShaderModule(core.device, fragment_shader_size, fragment_code, &graphics_pipeline);
		shPipelineCreateShaderStage(core.device, VK_SHADER_STAGE_FRAGMENT_BIT, &graphics_pipeline);

		ShVkFixedStates fixed_states = { 0 };
		shSetVertexInputAttribute(0, SH_VEC3_SIGNED_FLOAT, 0, 12, &fixed_states);

		shSetFixedStates(core.device, core.surface.width, core.surface.height, SH_FIXED_STATES_POLYGON_MODE_FACE | SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, &fixed_states);
		shSetupGraphicsPipeline(core.device, core.render_pass, fixed_states, &graphics_pipeline);
	}
	

	ShVkPipeline compute_pipeline = { 0 };
	{
		uint32_t shader_size = 0;
		const char* code = readBinary("../examples/shaders/bin/Physics.comp.spv", &shader_size);
		shPipelineCreateShaderModule(core.device, shader_size, code, &graphics_pipeline);
		shPipelineCreateShaderStage(core.device, VK_SHADER_STAGE_VERTEX_BIT, &graphics_pipeline);


	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		shFrameReset(&core);
		uint32_t frame_index = 0;
		shFrameBegin(&core, &frame_index);
		
		shBindPipeline(core.graphics_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, &graphics_pipeline);

		shBindVertexBuffer(core.graphics_cmd_buffer, &canvas_vertex_buffer);
		shBindIndexBuffer(core.graphics_cmd_buffer, &canvas_index_buffer);
		
		shDrawIndexed(core.graphics_cmd_buffer, 6);

		shEndPipeline(&graphics_pipeline);
		shFrameEnd(&core, frame_index);
	}

	shPipelineRelease(core.device, &graphics_pipeline);

	shClearVertexBufferMemory(core.device, canvas_vertex_buffer, canvas_vertex_buffer_memory);
	shClearIndexBufferMemory(core.device, canvas_index_buffer, canvas_index_buffer_memory);

	shVulkanRelease(&core);

	return 0;
}



GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title) {
	assert(glfwInit());
	assert(glfwVulkanSupported() != GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	return glfwCreateWindow(width, height, title, NULL, NULL);
}

#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER
#include <stdlib.h>
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