#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDrawLoop.h>
#include <shvulkan/shVkDescriptorStructureMap.h>

#ifndef NDEBUG
#define VALIDATION_LAYERS_ENABLED 1
#else
#define VALIDATION_LAYERS_ENABLED 0
#endif//NDEBUG


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


const char* readBinary(const char* path, uint32_t* p_size);


typedef float shaderInput_t;


void initializeVulkan(ShVkCore* p_core);

void writeMemory(ShVkCore* p_core, VkBuffer* p_staging_buffer, VkDeviceMemory* p_staging_memory, VkBuffer* p_device_local_buffer, VkDeviceMemory* p_device_local_memory);

void setupPipeline(VkDevice device, VkBuffer device_local_buffer, ShVkPipeline* p_pipeline);



#define INPUT_COUNT 64
float inputs[INPUT_COUNT];
#define OUTPUT_COUNT INPUT_COUNT



//
//NUMBER OF PARALLEL UNITS
//you would prefer a multiple of 64 which is multiple of 32 and 64
//-> performance boos for Nvidia and AMD gpus
//
#define INVOCATION_X_COUNT 16
#define INVOCATION_Y_COUNT 4
#define INVOCATION_Z_COUNT 1


//
//THE SIZE OF X Y WORKGROUPS
//
#define WORKGROUP_X_SIZE (INPUT_COUNT / INVOCATION_X_COUNT) // = 64 / 16 = 4
#define WORKGROUP_Y_SIZE (INPUT_COUNT / INVOCATION_Y_COUNT) // = 64 / 4  = 16
#define WORKGROUP_Z_SIZE 1


//
//THE TOTAL NUMBER OF COMPUTATIONS IS 
//	INVOCATION_X_COUNT * INVOCATION_Y_COUNT * INVOCATION_Z_COUNT = 
//	WORKGROUP_X_SIZE   * WORKGROUP_Y_SIZE   * WORKGROUP_Z_SIZE   = 64
//	


int main(void) {

	ShVkCore core = { 0 };
	initializeVulkan(&core);
	

	ShVkPipeline pipeline = { 0 };

	//
	//INPUTS FOR SHADER
	//
	printf("Compute shader inputs:\n");
	for (uint32_t i = 0; i < INPUT_COUNT; i++) {
		inputs[i] = (float)(i);
		printf("%f\n", inputs[i]);
	}
	
	
	VkBuffer       staging_buffer     = NULL;
	VkBuffer       device_local_buffer = NULL;

	VkDeviceMemory staging_memory     = NULL;
	VkDeviceMemory device_local_memory = NULL;

	//
	//WRITE INPUT MEMORY
	//
	writeMemory(&core, &staging_buffer, &staging_memory, &device_local_buffer, &device_local_memory);

	//
	//USEFUL VARIABLES
	//
	assert(core.p_compute_commands != NULL);
	VkDevice device            = core.device;
	VkCommandBuffer cmd_buffer = core.p_compute_commands[0].cmd_buffer;
	VkFence fence              = core.p_compute_commands[0].fence;
	VkQueue queue              = core.compute_queue.queue;

	//
	//SETUP COMPUTE PIPELINE
	//
	setupPipeline(device, device_local_buffer, &pipeline);

	//
	//OPERATION CHAIN
	//
	shResetFence(device, fence);
	shBeginCommandBuffer(cmd_buffer);

	//
	//BIND COMPUTE PIPELINE BEFORE DOING ANYTHING DIFFERENT FROM SETUP
	//
	shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, &pipeline);

	//
	//UPDATE DESCRIPTOR BEFORE BINDING SETS
	//
	shPipelineUpdateDescriptorSets(device, &pipeline);

	//
	//BIND DESCRIPTOR SET
	//
	shPipelineBindDescriptorSet(cmd_buffer, 0, VK_PIPELINE_BIND_POINT_COMPUTE, &pipeline);

	printf("\nSquaring the numbers...\n\n");
	//
	//DISPATCH DATA TO WORK GROUPS
	//
	shCmdDispatch(
		cmd_buffer, 
		WORKGROUP_X_SIZE, 
		WORKGROUP_Y_SIZE, 
		1
	);

	//
	//STOP RECORDING COMMAND BUFFER
	//
	shEndCommandBuffer(cmd_buffer);

	//
	//SUBMIT OPERATIONS FROM COMMAND BUFFER TO COMPUTE QUEUE
	//
	shQueueSubmit(1, &cmd_buffer, queue, fence);

	//
	//WAIT FOR COMMAND BUFFER TO END
	//
	shWaitForFence(device, fence, UINT64_MAX);

	
	//
	//COPY DEVICE LOCAL BUFFER (NOW STORING OUTPUTS) TO STAGING BUFFER (HOST VISIBLE)
	//
	shResetFence(device, fence);
	shBeginCommandBuffer(cmd_buffer);
	shCopyBuffer(cmd_buffer, device_local_buffer, 0, 0, sizeof(inputs), staging_buffer);
	shEndCommandBuffer(cmd_buffer);
	shQueueSubmit(1, &cmd_buffer, queue, fence);
	shWaitForFence(device, fence, UINT64_MAX);

	//
	//READ OUTPUT VALUES FROM STAGING MEMORY
	//
	float outputs[OUTPUT_COUNT];
	shReadMemory(core.device, staging_memory, 0, sizeof(outputs), outputs);
	
	//
	//LOG OUTPUT VALUES
	//
	printf("Compute shader output values:\n");
	for (uint32_t i = 0; i < INPUT_COUNT; i++) {
		printf("%f\n", outputs[i]);
	}

	//
	//CLEAR STAGING AND DEVICE LOCAL MEMORY
	//
	shClearBufferMemory(device, staging_buffer, staging_memory);
	shClearBufferMemory(device, device_local_buffer, device_local_memory);

	//
	//DESTROY PIPELINE
	//
	shPipelineRelease(core.device, &pipeline);

	//
	//END VULKAN
	//
	shVulkanRelease(&core);

	return 0;
}

void initializeVulkan(ShVkCore* p_core) {
	shCreateInstance(p_core, "shvulkan compute example", "shvulkan engine", VALIDATION_LAYERS_ENABLED, 0, NULL);
	shSelectPhysicalDevice(p_core, VK_QUEUE_COMPUTE_BIT);
	shSetLogicalDevice(p_core);
	shGetComputeQueue(p_core);
	shCreateComputeCommandBuffers(p_core, 1);
	shSetSyncObjects(p_core);
}

void writeMemory(ShVkCore* p_core, VkBuffer* p_staging_buffer, VkDeviceMemory* p_staging_memory, VkBuffer* p_device_local_buffer, VkDeviceMemory* p_device_local_memory) {

	VkDevice device = p_core->device;
	VkCommandBuffer cmd_buffer = p_core->p_compute_commands[0].cmd_buffer;
	VkPhysicalDevice physical_device = p_core->physical_device;
	VkFence fence = p_core->p_compute_commands[0].fence;
	VkQueue queue = p_core->compute_queue.queue;

	//
	//CREATE INPUTS STAGING BUFFER
	//
	shCreateBuffer(device, sizeof(inputs), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, p_staging_buffer);
	shAllocateMemory(device, physical_device, *p_staging_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, p_staging_memory);
	shBindMemory(device, *p_staging_buffer, 0, *p_staging_memory);

	//
	//WRITE INPUTS TO STAGING BUFFER
	//
	shWriteMemory(device, *p_staging_memory, 0, sizeof(inputs), inputs);

	//
	//CREATE INPUT/OUTPUT DEVICE LOCAL BUFFER 
	//
	shCreateBuffer(device, sizeof(inputs), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, p_device_local_buffer);
	shAllocateMemory(device, physical_device, *p_device_local_buffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_device_local_memory);
	shBindMemory(device, *p_device_local_buffer, 0, *p_device_local_memory);

	//
	//COPY STAGING BUFFER TO DEVICE LOCAL MEMORY
	//
	shResetFence(device, fence);
	shBeginCommandBuffer(cmd_buffer);
	shCopyBuffer(cmd_buffer, *p_staging_buffer, 0, 0, sizeof(inputs), *p_device_local_buffer);
	shEndCommandBuffer(cmd_buffer);
	shQueueSubmit(1, &cmd_buffer, queue, fence);
	shWaitForFence(device, fence, UINT64_MAX);
}

void setupPipeline(VkDevice device, VkBuffer device_local_buffer, ShVkPipeline* p_pipeline) {

	shPipelineSetDescriptorCount(1, p_pipeline);

	shPipelineSetDescriptorBufferInfo(0, device_local_buffer, 0, sizeof(inputs), p_pipeline);//each input value to compute shader is 4 bytes

	shPipelineCreateDescriptorPool(device, 0, p_pipeline);
	shPipelineDescriptorSetLayout(device, 0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, p_pipeline);
	shPipelineAllocateDescriptorSet(device, 0, p_pipeline);

	uint32_t shader_size = 0;
	const char* shader_code = readBinary("../examples/shaders/bin/power.comp.spv", &shader_size);
	shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline);
	shPipelineCreateShaderStage(device, VK_SHADER_STAGE_COMPUTE_BIT, p_pipeline);

	shSetupComputePipeline(device, p_pipeline);
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