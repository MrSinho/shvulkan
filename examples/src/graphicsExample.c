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

	ShVkCore core = { 0 };
	const char* application_name = "shvulkan example";
	const uint32_t width = 720;
	const uint32_t height = 480;
	GLFWwindow* window = createWindow(width, height, application_name);
	{
		uint32_t extension_count = 2;
		const char** extension_names = glfwGetRequiredInstanceExtensions(&extension_count);
		shCreateInstance(&core, application_name, "shvulkan engine", VALIDATION_LAYERS_ENABLED, extension_count, extension_names);
		shVkAssertResult(
			glfwCreateWindowSurface(core.instance, window, NULL, &core.surface.surface),
			"error creating window surface"
		);
		core.surface.width = width;
		core.surface.height = height;
		shSelectPhysicalDevice(&core, SH_VK_CORE_GRAPHICS);
		shSetLogicalDevice(&core, SH_VK_CORE_GRAPHICS);
		shGetGraphicsQueue(&core);
		shInitSwapchainData(&core);
		shInitDepthData(&core);
		shCreateRenderPass(&core);
		shSetFramebuffers(&core);
		shSetSyncObjects(&core);
		shCreateGraphicsCommandBuffer(&core);
	}



	VkBuffer triangle_vertex_buffer;
	VkDeviceMemory triangle_vertex_buffer_memory; 
	#define TRIANGLE_VERTEX_COUNT 24
	float triangle[TRIANGLE_VERTEX_COUNT] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};
	{
		shCreateVertexBuffer(core.device, TRIANGLE_VERTEX_COUNT * 4, &triangle_vertex_buffer);
		shAllocateVertexBufferMemory(core.device, core.physical_device, triangle_vertex_buffer, &triangle_vertex_buffer_memory);
		shBindVertexBufferMemory(core.device, triangle_vertex_buffer, triangle_vertex_buffer_memory);
		shWriteVertexBufferMemory(core.device, triangle_vertex_buffer_memory, TRIANGLE_VERTEX_COUNT * 4, triangle);
	}


	
	#define QUAD_VERTEX_COUNT 32
	float quad[QUAD_VERTEX_COUNT] = {
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};
	#define QUAD_INDEX_COUNT 6
	uint32_t indices[QUAD_INDEX_COUNT] = {
		0, 1, 2,
		2, 3, 0
	};
	VkBuffer quad_vertex_buffer, quad_index_buffer;
	VkDeviceMemory quad_vertex_buffer_memory, quad_index_buffer_memory;
	{
		shCreateVertexBuffer(core.device, QUAD_VERTEX_COUNT * 4, &quad_vertex_buffer);
		shCreateIndexBuffer(core.device, QUAD_INDEX_COUNT * 4, &quad_index_buffer);

		shAllocateVertexBufferMemory(core.device, core.physical_device, quad_vertex_buffer, &quad_vertex_buffer_memory);
		shAllocateIndexBufferMemory(core.device, core.physical_device, quad_index_buffer, &quad_index_buffer_memory);

		shBindVertexBufferMemory(core.device, quad_vertex_buffer, quad_vertex_buffer_memory);
		shBindIndexBufferMemory(core.device, quad_index_buffer, quad_index_buffer_memory);

		shWriteVertexBufferMemory(core.device, quad_vertex_buffer_memory, QUAD_VERTEX_COUNT * 4, quad);
		shWriteIndexBufferMemory(core.device, quad_index_buffer_memory, QUAD_INDEX_COUNT * 4, indices);
	}
	
	
	
	ShVkPipeline pipeline = { 0 };
	ShVkFixedStates fixed_states = { 0 };
	{
		shSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, 128, &pipeline);

		shPipelineCreateDescriptorBuffer(core.device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 0, 32, &pipeline);
		shPipelineCreateDynamicDescriptorBuffer(core.device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 1, 64, 128, &pipeline);

		shPipelineAllocateDescriptorBufferMemory(core.device, core.physical_device, 0, &pipeline);
		shPipelineAllocateDescriptorBufferMemory(core.device, core.physical_device, 1, &pipeline);

		shPipelineBindDescriptorBufferMemory(core.device, 0, &pipeline);
		shPipelineBindDescriptorBufferMemory(core.device, 1, &pipeline);

		shPipelineDescriptorSetLayout(core.device, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, &pipeline);
		shPipelineDescriptorSetLayout(core.device, 1, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, &pipeline);

		shPipelineCreateDescriptorPool(core.device, 0, &pipeline);
		shPipelineCreateDescriptorPool(core.device, 1, &pipeline);

		shPipelineAllocateDescriptorSet(core.device, 0, &pipeline);
		shPipelineAllocateDescriptorSet(core.device, 1, &pipeline);

		uint32_t vertex_shader_size = 0;
		uint32_t fragment_shader_size = 0;
		const char* vertex_code = readBinary("../examples/shaders/bin/mesh.vert.spv", &vertex_shader_size);
		const char* fragment_code = readBinary("../examples/shaders/bin/mesh.frag.spv", &fragment_shader_size);
		shPipelineCreateShaderModule(core.device, vertex_shader_size, vertex_code, &pipeline);
		shPipelineCreateShaderStage(core.device, VK_SHADER_STAGE_VERTEX_BIT, &pipeline);

		shPipelineCreateShaderModule(core.device, fragment_shader_size, fragment_code, &pipeline);
		shPipelineCreateShaderStage(core.device, VK_SHADER_STAGE_FRAGMENT_BIT, &pipeline);

		shSetVertexInputAttribute(0, SH_VEC3_SIGNED_FLOAT, 0, 12, &fixed_states);
		shSetVertexInputAttribute(1, SH_VEC3_SIGNED_FLOAT, 12, 12, &fixed_states);
		shSetVertexInputAttribute(2, SH_VEC2_SIGNED_FLOAT, 24, 8, &fixed_states);

		shSetFixedStates(core.device, core.surface.width, core.surface.height, SH_FIXED_STATES_POLYGON_MODE_FACE | SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, &fixed_states);
		shSetupGraphicsPipeline(core.device, core.render_pass, fixed_states, &pipeline);
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
	void* push_constants_data[128/sizeof(void*)];
	memcpy(push_constants_data, projection, 64);
	memcpy(&push_constants_data[64/sizeof(void*)], view, 64);
	
	//uniform buffer data
	float light_data[8] = {
		0.0f,  2.0f, 0.0f, 1.0f, //light position
		0.0f, 0.45f, 0.9f, 1.0f // light color
	};
	
	//type uniform buffer data
	float model0[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	float model1[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	uint32_t available_vram, process_used_vram = 0;
	shGetMemoryBudgetProperties(core.physical_device, &available_vram, &process_used_vram, NULL);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		shFrameReset(&core);
		uint32_t frame_index = 0;
		shFrameBegin(&core, &frame_index);
		
		shBindPipeline(core.graphics_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);

		shPipelinePushConstants(core.graphics_cmd_buffer, push_constants_data, &pipeline);

		shPipelineUpdateDescriptorSets(core.device, &pipeline);

		shPipelineWriteDescriptorBufferMemory(core.device, 0, light_data, &pipeline);
		shPipelineBindDescriptorSet(core.graphics_cmd_buffer, 0, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);
		
		shPipelineWriteDynamicDescriptorBufferMemory(core.device, 1, model0, &pipeline);
		shPipelineBindDynamicDescriptorSet(core.graphics_cmd_buffer, 1, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);

		shBindVertexBuffer(core.graphics_cmd_buffer, &quad_vertex_buffer);
		shBindIndexBuffer(core.graphics_cmd_buffer, &quad_index_buffer);
		shDrawIndexed(core.graphics_cmd_buffer, QUAD_INDEX_COUNT);

		shPipelineWriteDynamicDescriptorBufferMemory(core.device, 1, model1, &pipeline);
		shPipelineBindDynamicDescriptorSet(core.graphics_cmd_buffer, 1, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);

		triangle[9] = (float)sin(glfwGetTime());
		shWriteVertexBufferMemory(core.device, triangle_vertex_buffer_memory, TRIANGLE_VERTEX_COUNT * 4, triangle);
		shBindVertexBuffer(core.graphics_cmd_buffer, &triangle_vertex_buffer);
		shDraw(core.graphics_cmd_buffer, TRIANGLE_VERTEX_COUNT / (fixed_states.vertex_binding_description.stride / 4));

		shEndPipeline(&pipeline);
		shFrameEnd(&core, frame_index);
	}

	shPipelineClearDescriptorBufferMemory(core.device, 0, &pipeline);
	shPipelineClearDescriptorBufferMemory(core.device, 1, &pipeline);
	
	shPipelineRelease(core.device, &pipeline);

	shClearVertexBufferMemory(core.device, triangle_vertex_buffer, triangle_vertex_buffer_memory);
	shClearVertexBufferMemory(core.device, quad_vertex_buffer, quad_vertex_buffer_memory);
	shClearIndexBufferMemory(core.device, quad_index_buffer, quad_index_buffer_memory);

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