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


GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title);

void initializeVulkan(GLFWwindow** p_window, ShVkCore* p_core);

void setupPipeline(VkDevice device, VkPhysicalDevice physical_device, uint32_t width, uint32_t height, VkBuffer descriptors_buffer, VkRenderPass render_pass, ShVkFixedStates* p_fixed_states, ShVkPipeline* p_pipeline);

void writeMemory(ShVkCore* p_core, VkBuffer* p_staging_buffer, VkDeviceMemory* p_staging_memory, VkBuffer* p_vertex_buffer, VkDeviceMemory* p_vertex_memory, VkBuffer* p_instance_buffer, VkDeviceMemory* p_instance_memory, VkBuffer* p_index_buffer, VkDeviceMemory* p_index_memory, VkBuffer* p_descriptors_buffer, VkDeviceMemory* p_descriptors_memory);

void checkWindowSize(GLFWwindow* window, ShVkCore* p_core, ShVkFixedStates* p_fixed_states, VkBuffer descriptors_buffer, ShVkPipeline* p_pipeline);

const char* readBinary(const char* path, uint32_t* p_size);



#ifndef alignas
#include "../../external/stdalign.in.h"
#endif//alignas



#define QUAD_VERTEX_COUNT 32
#define QUAD_INDEX_COUNT 6
#define TRIANGLE_VERTEX_COUNT 24

#define PER_VERTEX_BINDING 0
#define PER_INSTANCE_BINDING 1

//
//DEFAULT GLOBAL VARIABLES
//
float quad[QUAD_VERTEX_COUNT] = {
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
float triangle[TRIANGLE_VERTEX_COUNT] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
uint32_t indices[QUAD_INDEX_COUNT] = {
	0, 1, 2,
	2, 3, 0
};
float light[8] = {
	0.0f,  2.0f, 0.0f, 1.0f, //position
	0.0f, 0.45f, 0.9f, 1.0f	 //color
};
float models[48] = {
	0.2f, 0.0f, 0.0f, 0.0f,//model 0
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.2f, 0.0f,
   -0.6f,-0.2f, 0.0f, 1.0f,

	0.2f, 0.0f, 0.0f, 0.0f,//model 1
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.2f, 0.0f,
    0.6f,-0.2f, 0.0f, 1.0f,

	0.6f, 0.0f, 0.0f, 0.0f,//model 2
	0.0f, 0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.4f, 0.0f, 1.0f
};
float projection_view[32] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};



int main(void) {

	//
	//INITIALIZE CORE STRUCTURE
	//
	ShVkCore core = { 0 };


	//
	//BASIC SETUP
	//
	GLFWwindow* window = NULL;
	initializeVulkan(&window, &core);


	//
	//GET AVAILABLE MEMORY, if extension is supported
	//
	if (core.device_extension_count == 2) {//first extension is VK_KHR_swapchain
		ShMemoryBudgetProperties memory_budget = { 0 };
		shGetMemoryBudgetProperties(
			core.physical_device,
			&memory_budget
		);
		uint32_t memory_type_index = 0;
		shGetMemoryType(core.device, core.physical_device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memory_type_index);
		printf("Available device local vram: %u bytes\n", (uint32_t)memory_budget.heapBudget[memory_type_index]);

	}
	

	//
	//ALLOCATE AND WRITE MEMORY
	//
	VkBuffer       staging_buffer     = NULL;
	VkDeviceMemory staging_memory     = NULL;

	VkBuffer       vertex_buffer      = NULL;
	VkBuffer       instance_buffer    = NULL;
	VkBuffer       index_buffer       = NULL;
	VkBuffer       descriptor_buffer  = NULL;

	VkDeviceMemory vertex_memory      = NULL;
	VkDeviceMemory instance_memory    = NULL;
	VkDeviceMemory index_memory       = NULL;
	VkDeviceMemory descriptor_memory  = NULL;

	writeMemory(&core, &staging_buffer, &staging_memory, &vertex_buffer, &vertex_memory, &instance_buffer, &instance_memory, &index_buffer, &index_memory, &descriptor_buffer, &descriptor_memory);


	//
	//SETUP PIPELINE
	//
	ShVkPipeline pipeline			= { 0 };
	ShVkFixedStates	fixed_states	= { 0 };
	setupPipeline(
		core.device, 
		core.physical_device, 
		core.surface.width,
		core.surface.height,
		descriptor_buffer,
		core.render_pass,
		&fixed_states,
		&pipeline
	);


	//
	//USEFUL VARIABLES
	//
	assert(core.p_graphics_commands != NULL);

	VkDevice			device          = core.device;
	VkPhysicalDevice	physical_device = core.physical_device;
	VkCommandBuffer		cmd_buffer      = core.p_graphics_commands[0].cmd_buffer;
	VkFence				fence           = core.p_graphics_commands[0].fence;
	VkSemaphore         semaphore       = core.p_render_semaphores[0];

	uint32_t    swapchain_image_idx     = 0;
	


	//
	//MAIN LOOP
	//
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//
		//MAKE WINDOW RESIZABLE
		//
		checkWindowSize(window, &core, &fixed_states, descriptor_buffer, &pipeline);

		//
		//RESET FENCE STATE AND RECORD COMMAND BUFFER
		//
		shResetFence(device, fence);
		shBeginCommandBuffer(cmd_buffer);

		//
		//MODIFY TRIANGLE MESH, COMMAND BUFFER MUST BE RECORDING OUTSIDE OF A RENDER PASS
		//
		triangle[9] = (float)sin(glfwGetTime());
		shWriteMemory(device, staging_memory, sizeof(quad), sizeof(triangle), triangle);
		shCopyBuffer(cmd_buffer, staging_buffer, sizeof(quad), sizeof(quad), sizeof(triangle), vertex_buffer);

		//
		//ACQUIRE IMAGE AND BEGIN RENDER PASS, COMMAND BUFFER MUST BE RECORDING
		//
		shAcquireNextImage(&core, semaphore, &swapchain_image_idx);
		shBeginRenderPass(
			&core, 
			cmd_buffer, 
			(VkClearColorValue){ { 0.1f, 0.1f , 0.1f, 1.0f } }, 
			swapchain_image_idx
		);


		//
		//BIND BUFFERS
		//
		VkDeviceSize vertex_offset = 0;
		shBindVertexBuffers(cmd_buffer, PER_VERTEX_BINDING, 1, &vertex_buffer, &vertex_offset);
		shBindVertexBuffers(cmd_buffer, PER_INSTANCE_BINDING, 1, &instance_buffer, &vertex_offset);
		shBindIndexBuffer(cmd_buffer, 0, &index_buffer);

		//
		//BIND PIPELINE BEFORE DOING ANYTHING DIFFERENT FROM SETUP
		//
		shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, &pipeline);

		//
		//EASILY PUSH CONSTANTS
		//
		shPipelinePushConstants(cmd_buffer, projection_view, &pipeline);

		//
		//UPDATE DESCRIPTORS BEFORE BINDING SETS TO PIPELINE
		//
		shPipelineUpdateDescriptorSets(device, &pipeline);

		//
		//FIRST BINDING OF MODEL MATRIX WITH OFFSET OF 0 BYTES
		//
		shPipelineBindDescriptorSets(cmd_buffer, 0, 1, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, NULL, &pipeline);

		//
		//DRAW QUAD, STARTING FROM OFFSET 0 OF VERTEX AND INDEX BUFFER
		//
		shDrawIndexed(cmd_buffer, QUAD_INDEX_COUNT, 0, 0, 2, 0);
		
		//
		//DIFFERENT GEOMETRY, NEW DRAW CALL
		//
		vertex_offset = 128;//skip to third model matrix
		shBindVertexBuffers(cmd_buffer, PER_INSTANCE_BINDING, 1, &instance_buffer, &vertex_offset);
		shDraw(cmd_buffer, 3, 4, 1, 0);

		//
		//END RENDERPASS AND COMMAND BUFFER
		//
		shEndRenderPass(cmd_buffer);
		shEndCommandBuffer(cmd_buffer);

		//
		//SUBMIT TO GRAPHICS QUEUE AND PRESENT IMAGE, COMMAND BUFFER MUST BE INACTIVE
		//
		shGraphicsQueueSubmit(&core, cmd_buffer, semaphore, fence);
		shPresentImage(&core, semaphore, swapchain_image_idx);

		//
		//WAIT FOR EVERYTHING TO END
		//
		shWaitForFence(device, fence);

	}

	glfwTerminate();

	shPipelineRelease(device, &pipeline);

	//
	//CLEAR BUFFERS
	//
	shClearBufferMemory(device, vertex_buffer, vertex_memory);
	shClearBufferMemory(device, instance_buffer, instance_memory);
	shClearBufferMemory(device, index_buffer, index_memory);
	shClearBufferMemory(device, descriptor_buffer, descriptor_memory);
	shClearBufferMemory(device, staging_buffer, staging_memory);

	//
	//END VULKAN
	//
	shVulkanRelease(&core);

	return 0;
}

void initializeVulkan(GLFWwindow** p_window, ShVkCore* p_core) {
	assert(p_window != NULL && p_core != NULL);

	const char* application_name = "shvulkan example";

	uint32_t width = 720;
	uint32_t height = 480;

	(*p_window)= createWindow(width, height, application_name);

	GLFWwindow* window = (*p_window);

	uint32_t instance_extension_count = 0;
	const char** pp_instance_extensions = glfwGetRequiredInstanceExtensions(&instance_extension_count);
	shCreateInstance(p_core, application_name, "shvulkan engine", VALIDATION_LAYERS_ENABLED, instance_extension_count, pp_instance_extensions);
	glfwCreateWindowSurface(p_core->instance, window, NULL, &p_core->surface.surface);
	p_core->surface.width = width;
	p_core->surface.height = height;

	shSelectPhysicalDevice(p_core, VK_QUEUE_GRAPHICS_BIT);
	shSetLogicalDevice(p_core);
	shInitSwapchainData(p_core);
	shInitDepthData(p_core);
	shCreateRenderPass(p_core);
	shSetFramebuffers(p_core);
	shCreateGraphicsCommandBuffers(p_core, 1);
	shSetSyncObjects(p_core);
	shGetGraphicsQueue(p_core);
}

void setupPipeline(VkDevice device, VkPhysicalDevice physical_device, uint32_t width, uint32_t height, VkBuffer descriptors_buffer, VkRenderPass render_pass, ShVkFixedStates* p_fixed_states, ShVkPipeline* p_pipeline) {
	assert(p_pipeline != NULL && p_fixed_states != NULL);

	shSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, 128, &p_pipeline->push_constant_range);
	
	shPipelineSetDescriptorCount(1, p_pipeline);

	//
	//WHERE THE DESCRIPTOR DATA IS LOCATED INSIDE THE DESCRIPTOR BUFFER
	//
	shPipelineSetDescriptorBufferInfo(0, descriptors_buffer, 0, sizeof(light), p_pipeline);

	shPipelineDescriptorSetLayout(device, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, p_pipeline);

	shPipelineCreateDescriptorPool(device, 0, p_pipeline);
	
	shPipelineAllocateDescriptorSet(device, 0, p_pipeline);
	
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
	
	shSetVertexInputAttribute(0, PER_VERTEX_BINDING, SH_VEC3_SIGNED_FLOAT, 0, 12, p_fixed_states);
	shSetVertexInputAttribute(1, PER_VERTEX_BINDING, SH_VEC3_SIGNED_FLOAT, 12, 12, p_fixed_states);
	shSetVertexInputAttribute(2, PER_VERTEX_BINDING, SH_VEC2_SIGNED_FLOAT, 24, 8, p_fixed_states);
	
	//since it's a matrix, although it's not specified in the shader, it will consume multpiple input locations
	shSetVertexInputAttribute(3, PER_INSTANCE_BINDING, SH_VEC4_SIGNED_FLOAT, 0, 16, p_fixed_states);
	shSetVertexInputAttribute(4, PER_INSTANCE_BINDING, SH_VEC4_SIGNED_FLOAT, 16, 16, p_fixed_states);
	shSetVertexInputAttribute(5, PER_INSTANCE_BINDING, SH_VEC4_SIGNED_FLOAT, 32, 16, p_fixed_states);
	shSetVertexInputAttribute(6, PER_INSTANCE_BINDING, SH_VEC4_SIGNED_FLOAT, 48, 16, p_fixed_states);

	shFixedStatesSetBindingCount(2, p_fixed_states);
	shFixedStatesSetVertexInputRate(VK_VERTEX_INPUT_RATE_VERTEX, PER_VERTEX_BINDING, p_fixed_states);
	shFixedStatesSetVertexInputRate(VK_VERTEX_INPUT_RATE_INSTANCE, PER_INSTANCE_BINDING, p_fixed_states);
	shFixedStatesSetVertexInputState(p_fixed_states);
	
	shSetFixedStates(device, width, height, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, p_fixed_states);
	shSetupGraphicsPipeline(device, render_pass, *p_fixed_states, p_pipeline);
}

void writeMemory(
	ShVkCore*       p_core,
	VkBuffer*       p_staging_buffer,
	VkDeviceMemory* p_staging_memory,
	VkBuffer*       p_vertex_buffer, 
	VkDeviceMemory* p_vertex_memory, 
	VkBuffer*       p_instance_buffer,
	VkDeviceMemory* p_instance_memory,
	VkBuffer*       p_index_buffer, 
	VkDeviceMemory* p_index_memory, 
	VkBuffer*       p_descriptor_buffer,
	VkDeviceMemory* p_descriptors_memory
) {

	//
	//USEFUL VARIABLES
	//
	VkDevice device                  = p_core->device;
	VkPhysicalDevice physical_device = p_core->physical_device;
	VkCommandBuffer  cmd_buffer      = p_core->p_graphics_commands[0].cmd_buffer;
	VkFence  fence                   = p_core->p_graphics_commands[0].fence;
	
	uint32_t quad_vertices_offset     = 0;
	uint32_t triangle_vertices_offset = quad_vertices_offset     + sizeof(quad);
	uint32_t instance_models_offset   = triangle_vertices_offset + sizeof(triangle);
	uint32_t quad_indices_offset      = instance_models_offset   + sizeof(models);
	uint32_t light_offset             = quad_indices_offset      + sizeof(indices);
	
	uint32_t staging_size             = quad_vertices_offset     + 
	                                    triangle_vertices_offset + 
	                                    instance_models_offset   + 
	                                    quad_indices_offset      + 
	                                    light_offset             + 
	                                    sizeof(light)            ;

	//
	//WRITE ALL DATA TO STAGING BUFFER
	//
	shCreateBuffer(
		device, 
		staging_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		p_staging_buffer
	);
	shAllocateMemory(
		device, 
		physical_device, 
		*p_staging_buffer,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		p_staging_memory
	);
	shWriteMemory(device, *p_staging_memory, quad_vertices_offset,     sizeof(quad),     quad    );
	shWriteMemory(device, *p_staging_memory, triangle_vertices_offset, sizeof(triangle), triangle);
	shWriteMemory(device, *p_staging_memory, quad_indices_offset,      sizeof(indices),  indices );
	shWriteMemory(device, *p_staging_memory, instance_models_offset,   sizeof(models),   models  );
	shWriteMemory(device, *p_staging_memory, instance_models_offset,   sizeof(models),   models  );
	shWriteMemory(device, *p_staging_memory, light_offset,             sizeof(light),    light   );

	shBindMemory(device, *p_staging_buffer, 0, *p_staging_memory);


	//
	//SETUP DEVICE LOCAL DESTINATION BUFFERS
	//
	shCreateBuffer(device, sizeof(quad) + sizeof(triangle), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, p_vertex_buffer);
	shAllocateMemory(device, physical_device, *p_vertex_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_vertex_memory);
	shBindMemory(device, *p_vertex_buffer, 0, *p_vertex_memory);

	shCreateBuffer(device, sizeof(models), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, p_instance_buffer);
	shAllocateMemory(device, physical_device, *p_instance_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_instance_memory);
	shBindMemory(device, *p_instance_buffer, 0, *p_instance_memory);

	shCreateBuffer(device, sizeof(indices), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, p_index_buffer);
	shAllocateMemory(device, physical_device, *p_index_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_index_memory);
	shBindMemory(device, *p_index_buffer, 0, *p_index_memory);

	shCreateBuffer(device, sizeof(light), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, p_descriptor_buffer);
	shAllocateMemory(device, physical_device, *p_descriptor_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_descriptors_memory);
	shBindMemory(device, *p_descriptor_buffer, 0, *p_descriptors_memory);

	//
	//COPY STAGING BUFFER TO DEVICE LOCAL MEMORY
	//
	shResetFence(device, fence);
	shBeginCommandBuffer(cmd_buffer);
	shCopyBuffer(cmd_buffer, *p_staging_buffer, quad_vertices_offset,   0, sizeof(quad) + sizeof(triangle), *p_vertex_buffer);
	shCopyBuffer(cmd_buffer, *p_staging_buffer, instance_models_offset, 0, sizeof(models),                  *p_instance_buffer);
	shCopyBuffer(cmd_buffer, *p_staging_buffer, quad_indices_offset,    0, sizeof(indices),                 *p_index_buffer);
	shCopyBuffer(cmd_buffer, *p_staging_buffer, light_offset,           0, sizeof(light),                   *p_descriptor_buffer);
	shEndCommandBuffer(cmd_buffer);

	shQueueSubmit(1, &cmd_buffer, p_core->graphics_queue.queue, fence);
	shWaitForFence(device, fence);
}


GLFWwindow* createWindow(const uint32_t width, const uint32_t height, const char* title) {
	shVkError(!glfwInit(), "error initializing glfw", return NULL);
	shVkError(glfwVulkanSupported() == GLFW_FALSE, "vulkan not supported by glfw", return NULL);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	return glfwCreateWindow(width, height, title, NULL, NULL);
}

void checkWindowSize(GLFWwindow* window, ShVkCore* p_core, ShVkFixedStates* p_fixed_states, VkBuffer descriptors_buffer, ShVkPipeline* p_pipeline) {
	uint32_t width  = 0;
	uint32_t height = 0;

	VkDevice device                  = p_core->device;
	VkPhysicalDevice physical_device = p_core->physical_device;

	glfwGetWindowSize(window, &width, &height);
	if (width != p_core->surface.width || height != p_core->surface.height) {
		shWaitDeviceIdle(device);

		shSwapchainRelease(p_core);
		shSurfaceRelease(p_core);
		shDepthBufferRelease(p_core);

		glfwCreateWindowSurface(p_core->instance, window, NULL, &p_core->surface.surface);
		p_core->surface.width = width;
		p_core->surface.height = height;
		shInitSwapchainData(p_core);
		shInitDepthData(p_core);
		shSetFramebuffers(p_core);

		//just for legacy implementations
		shGetPhysicalDeviceSurfaceSupport(p_core, p_core->graphics_queue);


		shPipelineRelease(device, p_pipeline);
		shFixedStatesRelease(p_fixed_states);

		setupPipeline(
			device, 
			physical_device, 
			width, 
			height, 
			descriptors_buffer,
			p_core->render_pass, 
			p_fixed_states, 
			p_pipeline
		);
	}
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