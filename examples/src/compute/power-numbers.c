#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVulkan.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



void writeMemory(
	VkDevice         device, 
	VkPhysicalDevice physical_device,
	VkFence          fence,
	VkQueue          compute_queue,
	VkCommandBuffer  cmd_buffer,
	VkBuffer*        p_staging_buffer, 
	VkDeviceMemory*  p_staging_memory, 
	VkBuffer*        p_device_local_buffer, 
	VkDeviceMemory*  p_device_local_memory
) ;

void setupPipeline(
	VkDevice          device,
	VkBuffer          device_local_buffer,
	ShVkPipelinePool* p_pipeline_pool
);

char* readBinary(const char* path, uint32_t* p_size);



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

	VkInstance       instance                   = NULL;
	VkDevice         device                     = NULL;
	VkPhysicalDevice physical_device            = NULL;
	
	uint32_t         compute_queue_family_index = 0;
	VkQueue          compute_queue              = NULL;

	VkCommandPool    cmd_pool                   = NULL;
	VkCommandBuffer  cmd_buffer                 = NULL;
	VkFence          fence                      = NULL;

	shCreateInstance(
		"vulkan app",//application_name, 
		"vulkan engine",//engine_name, 
		1,//enable_validation_layers,
		0,//extension_count, 
		NULL,//pp_extension_names,
		&instance//p_instance	
	);

	shSelectPhysicalDevice(
		instance, NULL, VK_QUEUE_COMPUTE_BIT, &physical_device,
		NULL, NULL, NULL
	);

	uint32_t compute_queue_family_indices[SH_MAX_STACK_QUEUE_COUNT] = { 0 };
	shGetPhysicalDeviceQueueFamilies(
		physical_device, NULL, NULL, NULL, NULL, 
		NULL, NULL, NULL, NULL, compute_queue_family_indices, NULL, NULL
	);

	VkDeviceQueueCreateInfo compute_queue_info = { 0 };

	compute_queue_family_index = compute_queue_family_indices[0];
	float compute_queue_priority = 1.0f;
	shQueryForDeviceQueueInfo(
		compute_queue_family_index, 1, &compute_queue_priority, 0, &compute_queue_info
	);

	shSetLogicalDevice(
		physical_device, &device,
		0, NULL, 1, &compute_queue_info
	);

	shGetDeviceQueues(device, 1, compute_queue_family_indices, &compute_queue);

	shCreateCommandPool(device, compute_queue_family_index, &cmd_pool);

	shAllocateCommandBuffers(device, cmd_pool, 1, &cmd_buffer);

	shCreateFences(device, 1, 0, &fence);

	ShVkPipelinePool* p_pipeline_pool = shAllocatePipelinePool();

	shVkError(
		p_pipeline_pool == NULL,
		"invalid pipeline pool memory",
		return -1
	);

	ShVkPipeline* p_pipeline = &p_pipeline_pool->pipelines[0];

	//
	//INPUTS FOR SHADER
	//
	printf("Compute shader inputs:\n");
	for (uint32_t i = 0; i < INPUT_COUNT; i++) {
		inputs[i] = (float)(i);
		printf("%f\n", inputs[i]);
	}
	
	
	VkBuffer       staging_buffer      = NULL;
	VkBuffer       device_local_buffer = NULL;

	VkDeviceMemory staging_memory      = NULL;
	VkDeviceMemory device_local_memory = NULL;

	//
	//WRITE INPUT MEMORY
	//
	writeMemory(
		device, physical_device, fence, 
		compute_queue, cmd_buffer,  &staging_buffer, 
		&staging_memory, &device_local_buffer, &device_local_memory
	);

	//
	//SETUP COMPUTE PIPELINE
	//
	setupPipeline(device, device_local_buffer, p_pipeline_pool);

	//
	//OPERATION CHAIN
	//
	shResetFences(device, 1, &fence);
	shBeginCommandBuffer(cmd_buffer);

	//
	//BIND COMPUTE PIPELINE BEFORE DOING ANYTHING DIFFERENT FROM SETUP
	//
	shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, p_pipeline);
	

	//
	//BIND DESCRIPTOR SET
	//
	shPipelineBindDescriptorSets(
		cmd_buffer, 0, 1, VK_PIPELINE_BIND_POINT_COMPUTE, 
		0, NULL, p_pipeline_pool, p_pipeline
	);

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
	//SUBMIT OPERATIONS FROM COMMAND BUFFER TO COMPUTE compute_queue
	//
	shQueueSubmit(
		1, &cmd_buffer, compute_queue, fence, 0, NULL, 
		VK_PIPELINE_STAGE_TRANSFER_BIT, 0, NULL
	);

	//
	//WAIT FOR COMMAND BUFFER TO END
	//
	shWaitForFences(device, 1, &fence, 1, UINT64_MAX);

	
	//
	//COPY DEVICE LOCAL BUFFER (NOW STORING OUTPUTS) TO STAGING BUFFER (HOST VISIBLE)
	//
	shResetFences(device, 1, &fence);
	shBeginCommandBuffer(cmd_buffer);
	shCopyBuffer(
		cmd_buffer, device_local_buffer, 0, 0, 
		sizeof(inputs), staging_buffer
	);
	shEndCommandBuffer(cmd_buffer);
	shQueueSubmit(
		1, &cmd_buffer, compute_queue, fence, 0, NULL, 
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, NULL
	);
	shWaitForFences(device, 1, &fence, 1, UINT64_MAX);

	//
	//READ OUTPUT VALUES FROM STAGING MEMORY
	//
	float outputs[OUTPUT_COUNT];
	shReadMemory(device, staging_memory, 0, sizeof(outputs), outputs);
	
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

	shPipelinePoolDestroyDescriptorPools(device, 0, 1, p_pipeline_pool);
	shPipelinePoolDestroyDescriptorSetLayouts(device, 0, 1, p_pipeline_pool);


	//
	//DESTROY PIPELINE
	//
	shPipelineDestroyShaderModules    (device, 0, 1, p_pipeline);
	shPipelineDestroyLayout           (device, p_pipeline);
	shDestroyPipeline                 (device, p_pipeline->pipeline);

	shFreePipelinePool(p_pipeline_pool);

	//
	//END VULKAN
	//
	shDestroyFences(device, 1, &fence);
	shDestroyCommandBuffers(device, cmd_pool, 1, &cmd_buffer);
	shDestroyCommandPool(device, cmd_pool);

	shDestroyDevice(device);
	shDestroyInstance(instance);

	return 0;
}

void writeMemory(
	VkDevice         device, 
	VkPhysicalDevice physical_device,
	VkFence          fence,
	VkQueue          compute_queue,
	VkCommandBuffer  cmd_buffer,
	VkBuffer*        p_staging_buffer, 
	VkDeviceMemory*  p_staging_memory, 
	VkBuffer*        p_device_local_buffer, 
	VkDeviceMemory*  p_device_local_memory
) {
	//
	//CREATE INPUTS STAGING BUFFER
	//
	shCreateBuffer(
		device, sizeof(inputs), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
		VK_SHARING_MODE_EXCLUSIVE, p_staging_buffer
	);
	shAllocateBufferMemory(
		device, physical_device, *p_staging_buffer, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		p_staging_memory
	);
	shBindBufferMemory(
		device, *p_staging_buffer, 0, *p_staging_memory
	);

	//
	//WRITE INPUTS TO STAGING BUFFER
	//
	shWriteMemory(
		device, *p_staging_memory, 0, sizeof(inputs), inputs
	);

	//
	//CREATE INPUT/OUTPUT DEVICE LOCAL BUFFER 
	//
	shCreateBuffer(
		device, sizeof(inputs), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
		VK_SHARING_MODE_EXCLUSIVE, p_device_local_buffer
	);
	shAllocateBufferMemory(
		device, physical_device, *p_device_local_buffer, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, p_device_local_memory
	);
	shBindBufferMemory(
		device, *p_device_local_buffer, 0, *p_device_local_memory
	);

	//
	//COPY STAGING BUFFER TO DEVICE LOCAL MEMORY
	//
	shResetFences(device, 1, &fence);
	shBeginCommandBuffer(cmd_buffer);
	shCopyBuffer(
		cmd_buffer, *p_staging_buffer, 0, 0, 
		sizeof(inputs), *p_device_local_buffer
	);
	shEndCommandBuffer(cmd_buffer);
	shQueueSubmit(
		1, &cmd_buffer, compute_queue, fence, 0, NULL, 
		VK_PIPELINE_STAGE_TRANSFER_BIT, 0, NULL
	);
	shWaitForFences(device, 1, &fence, 1, UINT64_MAX);
}

void setupPipeline(
	VkDevice          device, 
	VkBuffer          device_local_buffer, 
	ShVkPipelinePool* p_pipeline_pool
) {
	shPipelinePoolSetDescriptorSetBufferInfos(
		0, 1, device_local_buffer, 0, sizeof(inputs), p_pipeline_pool
	);//each input value to compute shader is 4 bytes

	shPipelinePoolCreateDescriptorSetLayoutBinding(
		0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		1, VK_SHADER_STAGE_COMPUTE_BIT, p_pipeline_pool
	);

	shPipelinePoolCreateDescriptorSetLayout(
		device, 0, 1, 0, p_pipeline_pool
	);

	shPipelinePoolCreateDescriptorPool(
		device, 0,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		1, p_pipeline_pool
	);
	shPipelinePoolAllocateDescriptorSets(
		device, 0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0, 1, p_pipeline_pool
	);

	shPipelinePoolUpdateDescriptorSets(device, 0, 1, p_pipeline_pool);

	ShVkPipeline* p_pipeline = &p_pipeline_pool->pipelines[0];

	uint32_t shader_size = 0;
	char* shader_code = readBinary("../examples/shaders/bin/power.comp.spv", &shader_size);
	shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline);
	shPipelineCreateShaderStage(VK_SHADER_STAGE_COMPUTE_BIT, p_pipeline);
	
	shPipelineCreateLayout(device, 0, 1, p_pipeline_pool, p_pipeline);

	shSetupComputePipeline(device, p_pipeline);
}

#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER
#include <stdlib.h>
char* readBinary(const char* path, uint32_t* p_size) {
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