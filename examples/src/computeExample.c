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

	{//GRAPHICS EXAMPLE
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

		ShVkCore core = shVkCoreInitPrerequisites();
		//INITIALIZE VULKAN INSTANCE
		shCreateInstance(&core, application_name, "shVulkan Engine", extensions_count, extensions_names);
		//CREATE WINDOW SURFACE
		VkResult r = glfwCreateWindowSurface(core.instance, window, NULL, &core.surface.surface);
		core.surface.width = width;
		core.surface.height = height;
		//CHOOSE GPU
		shSetPhysicalDevice(&core);
		//THE LOGICAL DEVICE COMMUNICATES WITH THE GRAPHICS DRIVER
		shSetLogicalDevice(&core);
		//GET THE QUEUE SPECIALIZED IN GRAPHICS
		shGetGraphicsQueue(&core);
		//CREATE SWAPCHAIN, HANDLES IMAGES
		shInitSwapchainData(&core);
		//CREATE DEPTH BUFFER
		shInitDepthData(&core);
		//CREATE RENDERPASS
		shCreateRenderPass(&core);
		//CREATE FRAMEBUFFERS, EACH FRAMEBUFFER HANDLES AN IMAGE
		shSetFramebuffers(&core);
		//
		shSetSyncObjects(&core);
		shInitCommands(&core);

		//MESH
		#define VERTEX_COUNT 20
		#define VERTEX_INPUT_ATTRIBUTE_COUNT 2 //for position and UV
		float quad[VERTEX_COUNT] = {
			-0.5f,-0.5, 0.0f, 0.0f, 0.0f,
			 0.5f,-0.5, 0.0f, 0.0f, 0.0f,
			 0.5f, 0.5, 0.0f, 0.0f, 0.0f,
			-0.5f, 0.5, 0.0f, 0.0f, 0.0f,
		};
		#define INDEX_COUNT 6
		uint32_t indices[INDEX_COUNT] = {
			0, 1, 2,
			2, 3, 0
		};
		VkBuffer vertex_buffer, index_buffer;
		VkDeviceMemory vertex_buffer_memory, index_buffer_memory;
		shCreateVertexBuffer(core, VERTEX_COUNT * 4, &vertex_buffer);
		shCreateIndexBuffer(core, INDEX_COUNT * 4, &index_buffer);
		shAllocateVertexBuffer(core, vertex_buffer, &vertex_buffer_memory);
		shAllocateIndexBuffer(core, index_buffer, &index_buffer_memory);
		shMapVertexBufferMemory(core, vertex_buffer_memory, VERTEX_COUNT * 4, quad);
		shMapIndexBufferMemory(core, index_buffer_memory, INDEX_COUNT * 4, indices);


		ShVkPipelineData pipeline = { 0 };

		{//PUSH CONSTANTS SETUP
			#define PUSH_CONSTANTS_SIZE 128
		}
		shSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, PUSH_CONSTANTS_SIZE, &pipeline);

		{//UNIFORM BUFFER SETUP
			#define UNIFORM_BUFFER_COUNT 1
			#define UNIFORM_BUFFER_SIZE 64
			#define DESCRIPTOR_SET_IDX 0
		}
		ShUniformBuffer uniform_buffer = { 0 };
		pipeline.uniform_buffer_count = UNIFORM_BUFFER_COUNT;
		pipeline.p_uniform_buffers = &uniform_buffer;
		shAllocateUniformBufferData(core, UNIFORM_BUFFER_SIZE, pipeline.p_uniform_buffers);
		shCreateDescriptorPool(core, pipeline.p_uniform_buffers);
		shDescriptorSetLayout(core, 0, VK_SHADER_STAGE_FRAGMENT_BIT, pipeline.p_uniform_buffers);
		VkDescriptorSet			descriptor_set			= { 0 };
		VkWriteDescriptorSet	write_descriptor_set	= { 0 };
		pipeline.p_descriptor_sets			= &descriptor_set;
		pipeline.p_write_descriptor_sets	= &write_descriptor_set;
		shAllocateDescriptorSets(core, DESCRIPTOR_SET_IDX, &pipeline);

		#define SHADERS_COUNT 2
		pipeline.shader_stage_count  = SHADERS_COUNT;
		pipeline.shader_module_count = SHADERS_COUNT;
		uint32_t vertex_shader_size = 0;
		uint32_t fragment_shader_size = 0;
		const char* vertex_code = readBinary("../ShVulkanExample/shaders/bin/mesh.vert.spv", &vertex_shader_size);
		const char* fragment_code = readBinary("../ShVulkanExample/shaders/bin/mesh.frag.spv", &fragment_shader_size);
		shCreateShaderModule(core.device, vertex_shader_size, vertex_code, &pipeline.shader_modules[0]);
		shCreateShaderModule(core.device, fragment_shader_size, fragment_code, &pipeline.shader_modules[1]);
		shCreateShaderStage(core.device, pipeline.shader_modules[0], VK_SHADER_STAGE_VERTEX_BIT, &pipeline.shader_stages[0]);
		shCreateShaderStage(core.device, pipeline.shader_modules[1], VK_SHADER_STAGE_FRAGMENT_BIT, &pipeline.shader_stages[1]);

		
		ShVkFixedStates fixed_states = { 0 };
		VkVertexInputAttributeDescription vertex_position_input_descriptions[VERTEX_INPUT_ATTRIBUTE_COUNT];
		fixed_states.vertex_input_attribute_description_count = VERTEX_INPUT_ATTRIBUTE_COUNT;
		fixed_states.p_vertex_input_attributes = vertex_position_input_descriptions;
		shSetVertexInputAttribute(0, SH_VEC3_SIGNED_FLOAT, 0, 12, &fixed_states);
		shSetVertexInputAttribute(1, SH_VEC2_SIGNED_FLOAT, 12, 8, &fixed_states);
		
		ShFixedStateFlags fixed_states_flags = SH_FIXED_STATES_POLYGON_MODE_FACE | SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		shSetFixedStates(core, fixed_states_flags, &fixed_states);
		shSetupGraphicsPipeline(core, fixed_states, &pipeline);

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
		memcpy(&((char*)push_constants_data)[64], view, 64);

		float light_position[4] = { 0.0f, 2.0f, 0.0f, 1.0f };
		float light_color[4] = { 0.8f, 0.3f, 0.6f, 1.0f };
		void* uniform_buffer_data[UNIFORM_BUFFER_SIZE/8];
		memcpy(uniform_buffer_data, light_position, 16);
		memcpy(&((char*)uniform_buffer_data)[16], light_color, 16);

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			shFrameReset(core);
			uint32_t frame_index = 0;
			shFrameBegin(core, &frame_index);
			
			shBindVertexBuffer(core, &vertex_buffer);
			shBindIndexBuffer(core, &index_buffer);

			shBindPipeline(core.graphics_cmd_buffer, pipeline);

			shPushConstants(core.graphics_cmd_buffer, pipeline, push_constants_data);

			shMapMemory(core.device,
				pipeline.p_uniform_buffers[DESCRIPTOR_SET_IDX].uniform_buffer_memory,
				pipeline.p_uniform_buffers[DESCRIPTOR_SET_IDX].uniform_buffer_size,
				uniform_buffer_data);
				shBindDescriptorSets(core, pipeline);

			shDraw(core.graphics_cmd_buffer, INDEX_COUNT, 1);

			shFrameEnd(core, frame_index);
		}

		shClearVertexBufferMemory(core.device, vertex_buffer, vertex_buffer_memory);
		shClearIndexBufferMemory(core.device, index_buffer, index_buffer_memory);
		shDestroyPipeline(core, &pipeline);
		shVulkanRelease(&core);
	}

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