#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDrawLoop.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <memory.h>
#include <errno.h>

GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title);
const char* readBinary(const char* path, uint32_t* p_size);

int main(void) {

	/*
	* NOTE: the example uses GLFW to create the window, and for creating the window surface
	*/
	const char* application_name = "shVulkan example";
	const uint32_t width = 720;
	const uint32_t height = 480;
	GLFWwindow* window = createWindow(width, height, application_name);
	//GET INSTANCE EXTENSIONS FOR AN OUTPUT ON THE SCREEN
	uint32_t extensions_count = 0;
	const char** extensions_names = glfwGetRequiredInstanceExtensions(&extensions_count);
	
	ShVkCore core = { 0 };
	//INITIALIZE VULKAN INSTANCE
	shCreateInstance(&core, application_name, "shVulkan Engine", extensions_count, extensions_names);
	//CREATE WINDOW SURFACE
	VkResult r = glfwCreateWindowSurface(core.instance, window, NULL, &core.surface.surface);
	core.surface.width = width;
	core.surface.height = height;

	shSelectPhysicalDevice(&core, SH_VK_CORE_GRAPHICS);
	shSetLogicalDevice(&core);
	shGetGraphicsQueue(&core);
	shInitSwapchainData(&core);
	shInitDepthData(&core);
	shCreateRenderPass(&core);
	shSetFramebuffers(&core);
	shSetSyncObjects(&core);
	shCreateGraphicsCommandBuffer(&core);
	
	#define TRIANGLE_VERTEX_COUNT 15
	float triangle[TRIANGLE_VERTEX_COUNT] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	};
	VkBuffer triangle_vertex_buffer;
	VkDeviceMemory triangle_vertex_buffer_memory;
	shCreateVertexBuffer(&core, TRIANGLE_VERTEX_COUNT * 4, &triangle_vertex_buffer);
	shAllocateVertexBuffer(&core, triangle_vertex_buffer, &triangle_vertex_buffer_memory);
	shWriteVertexBufferMemory(&core, triangle_vertex_buffer_memory, TRIANGLE_VERTEX_COUNT * 4, triangle);
	#define QUAD_VERTEX_COUNT 20
	float quad[QUAD_VERTEX_COUNT] = {
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
	};
	#define QUAD_INDEX_COUNT 6
	uint32_t indices[QUAD_INDEX_COUNT] = {
		0, 1, 2,
		2, 3, 0
	};
	VkBuffer quad_vertex_buffer, quad_index_buffer;
	VkDeviceMemory quad_vertex_buffer_memory, quad_index_buffer_memory;
	shCreateVertexBuffer(&core, QUAD_VERTEX_COUNT * 4, &quad_vertex_buffer);
	shCreateIndexBuffer(&core, QUAD_INDEX_COUNT * 4, &quad_index_buffer);
	shAllocateVertexBuffer(&core, quad_vertex_buffer, &quad_vertex_buffer_memory);
	shAllocateIndexBuffer(&core, quad_index_buffer, &quad_index_buffer_memory);
	shWriteVertexBufferMemory(&core, quad_vertex_buffer_memory, QUAD_VERTEX_COUNT * 4, quad);
	shWriteIndexBufferMemory(&core, quad_index_buffer_memory, QUAD_INDEX_COUNT * 4, indices);
	
	
	ShVkPipelineData pipeline = { 0 };
	
	{//PUSH CONSTANTS SETUP
		#define PUSH_CONSTANTS_SIZE 128
	}
	shSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, PUSH_CONSTANTS_SIZE, &pipeline);
	
	pipeline.uniform_buffer_count = 1;
	pipeline.dynamic_uniform_buffer_count = 1;
	shCreateUniformBuffer(&core, 64, 0 , &pipeline);
	shAllocateUniformBuffer(&core, 0, &pipeline);
	shCreateDescriptorPool(&core, 0, &pipeline);
	shDescriptorSetLayout(&core, 0, VK_SHADER_STAGE_FRAGMENT_BIT, &pipeline);
	shAllocateDescriptorSet(&core, 0, &pipeline);

	shCreateDynamicUniformBuffer(&core, 64, 1, &pipeline);
	shAllocateUniformBuffer(&core, 1, &pipeline);
	shCreateDescriptorPool(&core, 1, &pipeline);
	shDescriptorSetLayout(&core, 1, VK_SHADER_STAGE_VERTEX_BIT, &pipeline);
	shAllocateDescriptorSet(&core, 1, &pipeline);

	#define SHADERS_COUNT 2
	pipeline.shader_stage_count  = SHADERS_COUNT;
	pipeline.shader_module_count = SHADERS_COUNT;
	uint32_t vertex_shader_size = 0;
	uint32_t fragment_shader_size = 0;
	const char* vertex_code = readBinary("../examples/shaders/bin/mesh.vert.spv", &vertex_shader_size);
	const char* fragment_code = readBinary("../examples/shaders/bin/mesh.frag.spv", &fragment_shader_size);
	shCreateShaderModule(core.device, vertex_shader_size, vertex_code, &pipeline.shader_modules[0]);
	shCreateShaderModule(core.device, fragment_shader_size, fragment_code, &pipeline.shader_modules[1]);
	shCreateShaderStage(core.device, pipeline.shader_modules[0], VK_SHADER_STAGE_VERTEX_BIT, &pipeline.shader_stages[0]);
	shCreateShaderStage(core.device, pipeline.shader_modules[1], VK_SHADER_STAGE_FRAGMENT_BIT, &pipeline.shader_stages[1]);
	
	
	ShVkFixedStates fixed_states = { 0 };
	shSetVertexInputAttribute(0, SH_VEC3_SIGNED_FLOAT, 0, 12, &fixed_states);
	shSetVertexInputAttribute(1, SH_VEC2_SIGNED_FLOAT, 12, 8, &fixed_states);
	
	shSetFixedStates(&core, SH_FIXED_STATES_POLYGON_MODE_FACE | SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, &fixed_states);
	shSetupGraphicsPipeline(&core, fixed_states, &pipeline);
	
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
	void* push_constants_data[PUSH_CONSTANTS_SIZE/8];
	memcpy(push_constants_data, projection, 64);
	memcpy(&push_constants_data[64/8], view, 64);
	
	//uniform buffer data
	float uniform_buffer_data[8] = {
		0.0f,  2.0f, 0.0f, 1.0f, //light position
		0.0f, 0.45f, 0.9f, 1.0f // light color
	};
	
	//dynamic uniform buffer data
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

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		shFrameReset(&core);
		uint32_t frame_index = 0;
		shFrameBegin(&core, &frame_index);
		
		shBindPipeline(&core, &pipeline);

		shPushConstants(&core, push_constants_data, &pipeline);

		vkUpdateDescriptorSets(core.device,
			1, &pipeline.write_descriptor_sets[0],
			0, NULL
		);
		vkUpdateDescriptorSets(core.device,
			1, &pipeline.dynamic_write_descriptor_sets[1],
			0, NULL
		);

		shWriteUniformBufferMemory(&core, 0, uniform_buffer_data, &pipeline);
		shBindUniformBuffer(&core, 0, &pipeline);
		
		shWriteDynamicUniformBufferMemory(&core, 1, model0, &pipeline);
		shBindDynamicUniformBuffer(&core, 1, &pipeline);

		shBindVertexBuffer(&core, &quad_vertex_buffer);
		shBindIndexBuffer(&core, &quad_index_buffer);
		shDrawIndexed(&core, QUAD_INDEX_COUNT);

		shWriteDynamicUniformBufferMemory(&core, 1, model1, &pipeline);
		shBindDynamicUniformBuffer(&core, 1, &pipeline);

		shBindVertexBuffer(&core, &triangle_vertex_buffer);
		shDraw(&core, TRIANGLE_VERTEX_COUNT / (fixed_states.vertex_binding_description.stride / 4));

		shEndPipeline(&pipeline);
		shFrameEnd(&core, frame_index);
	}
	
	shClearUniformBufferMemory(&core, 0, &pipeline);
	shClearUniformBufferMemory(&core, 1, &pipeline);
	shClearVertexBufferMemory(&core, triangle_vertex_buffer, triangle_vertex_buffer_memory);
	shClearVertexBufferMemory(&core, quad_vertex_buffer, quad_vertex_buffer_memory);
	shClearIndexBufferMemory(&core, quad_index_buffer, quad_index_buffer_memory);
	shDestroyPipeline(&core, &pipeline);
	shVulkanRelease(&core);

	return 0;
}

GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title) {
	if (!glfwInit()) {
		perror("Error initializing glfw");
		return NULL;
	}
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
		return NULL;
	}
	fread(code, code_size, 1, stream);
	*p_size = code_size;
	return code;
}