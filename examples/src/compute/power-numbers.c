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
float inputs[INPUT_COUNT] = { 0 };
#define OUTPUT_COUNT INPUT_COUNT

float factor = 1.0f;

//
//NUMBER OF INVOCATIONS OR THREADS must be equal to the invocations defined in the compute shader as layout(local_size_x...y...z)in
//
#define INVOCATION_X_COUNT 64
#define INVOCATION_Y_COUNT 1
#define INVOCATION_Z_COUNT 1


//
//THE SIZE OF X Y Z WORKGROUPS (each workgroup contains a number of invocations defined in the compute shader as layout(local_size_x...y...z)in
//
#define X_WORKGROUPS_COUNT 1 // = 1 x workgroup * 64 x invocations = 64 invocations ---| 
#define Y_WORKGROUPS_COUNT 1 // = 1 y workgroup * 1  y invocation  = 1  invocation     |--> 64 * 1 * 1 total invocations
#define Z_WORKGROUPS_COUNT 1 // = 1 z workgroup * 1  z invocation  = 1  invocation  ---|



//
//THE TOTAL NUMBER OF COMPUTATIONS IS 
//	INVOCATION_X_COUNT * INVOCATION_Y_COUNT * INVOCATION_Z_COUNT = 
//	WORKGROUP_X_SIZE   * WORKGROUP_Y_SIZE   * WORKGROUP_Z_SIZE   = 64
//	



int main(void) {

	VkInstance       instance                   = VK_NULL_HANDLE;
	VkDevice         device                     = VK_NULL_HANDLE;
	VkPhysicalDevice physical_device            = VK_NULL_HANDLE;
	
	uint32_t         compute_queue_family_index = 0;
	VkQueue          compute_queue              = VK_NULL_HANDLE;

	VkCommandPool    cmd_pool                   = VK_NULL_HANDLE;
	VkCommandBuffer  cmd_buffer                 = VK_NULL_HANDLE;
	VkFence          fence                      = VK_NULL_HANDLE;

	shCreateInstance(
		"vulkan app",//application_name, 
		"vulkan engine",//engine_name, 
		1,//enable_validation_layers,
		0,//extension_count, 
		NULL,//pp_extension_names,
		VK_MAKE_API_VERSION(1, 3, 0, 0),//api_version,
		&instance//p_instance	
	);

	shSelectPhysicalDevice(
		instance,//instance
		VK_NULL_HANDLE,//surface
		VK_QUEUE_COMPUTE_BIT,//requirements
		&physical_device,//p_physical_device
		NULL,//p_physical_device_properties
		NULL,//p_physical_device_features
		NULL//p_physical_device_memory_properties
	);

	uint32_t compute_queue_family_indices[SH_MAX_STACK_QUEUE_COUNT] = { 0 };

	shGetPhysicalDeviceQueueFamilies(
		physical_device,//physical_device
		VK_NULL_HANDLE,//surface
		NULL,//p_queue_family_count
		NULL,//p_graphics_queue_family_count
		NULL,//p_surface_queue_family_count
		NULL,//p_compute_queue_family_count
		NULL,//p_transfer_queue_family_count
		NULL,//p_graphics_queue_family_indices
		NULL,//p_surface_queue_family_indices
		compute_queue_family_indices,//p_compute_queue_family_indices
		NULL,//p_transfer_queue_family_indices
		NULL//p_queue_families_properties
	);

	VkDeviceQueueCreateInfo compute_queue_info = { 0 };

	compute_queue_family_index = compute_queue_family_indices[0];

	float compute_queue_priority = 1.0f;

	shQueryForDeviceQueueInfo(
		compute_queue_family_index,//queue_family_index
		1,//queue_count
		&compute_queue_priority,//p_queue_priorities
		0,//protected
		&compute_queue_info//p_device_queue_info
	);

	shSetLogicalDevice(
		physical_device,//physical_device
		&device,//p_device
		0,//extension_count
		NULL,//pp_extension_names
		1,//device_queue_count
		&compute_queue_info//p_device_queue_infos
	);

	shGetDeviceQueues(
		device,//device
		1,//queue_count
		compute_queue_family_indices,//p_queue_family_indices
		&compute_queue//p_queues
	);

	shCreateCommandPool(
		device,//device
		compute_queue_family_index,//queue_family_index
		&cmd_pool//p_cmd_pool
	);

	shAllocateCommandBuffers(
		device,//device
		cmd_pool,//cmd_pool
		1,//cmd_buffer_count
		&cmd_buffer//p_cmd_buffer
	);

	shCreateFences(
		device,//device
		1,//fence_count
		0,//signaled
		&fence//p_fences
	);

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
	
	
	VkBuffer       staging_buffer      = VK_NULL_HANDLE;
	VkBuffer       device_local_buffer = VK_NULL_HANDLE;

	VkDeviceMemory staging_memory      = VK_NULL_HANDLE;
	VkDeviceMemory device_local_memory = VK_NULL_HANDLE;

	//
	//WRITE INPUT MEMORY
	//
	writeMemory(
		device,//device
		physical_device,//physical_device
		fence,//fence
		compute_queue,//compute_queue
		cmd_buffer,//cmd_buffer
		&staging_buffer,//p_staging_buffer
		&staging_memory,//p_staging_memory
		&device_local_buffer,//p_device_local_buffer
		&device_local_memory//p_device_local_memory
	);

	//
	//SETUP COMPUTE PIPELINE
	//
	setupPipeline(
		device,//device
		device_local_buffer,//device_local_buffer
		p_pipeline_pool//p_pipeline_pool
	);


	//
	//RECORD COMMAND BUFFER FOR BINDING PIPELINE
	//
	shResetFences(device, 1, &fence);
	shBeginCommandBuffer(cmd_buffer);

	//
	//SET UP MEMORY BARRIER BEFORE SHADER READS THE BUFFER (to ensure data has been copied)
	//
	shSetBufferMemoryBarrier(
		device,//device
		cmd_buffer,//cmd_buffer
		device_local_buffer,//buffer
		VK_ACCESS_HOST_WRITE_BIT,//access_before_barrier
		VK_ACCESS_SHADER_READ_BIT,//access_after_barrier
		compute_queue_family_index,//performing_queue_family_index_before_barrier
		compute_queue_family_index,//performing_queue_family_index_after_barrier
		VK_PIPELINE_STAGE_HOST_BIT,//pipeline_stage_before_barrier
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT//pipeline_stage_after_barrier
	);

	//
	//BIND COMPUTE PIPELINE BEFORE DOING ANYTHING DIFFERENT FROM SETUP
	//
	shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, p_pipeline);
	

	//
	//BIND ALL DESCRIPTOR SETS
	//
	shPipelineBindDescriptorSetUnits(
		cmd_buffer,//cmd_buffer
		0,//first_descriptor_set
		0,//first_descriptor_set_unit_idx
		2,//descriptor_set_unit_count
		VK_PIPELINE_BIND_POINT_COMPUTE,//bind_point
		0,//dynamic_descriptors_count
		NULL,//p_dynamic_offsets
		p_pipeline_pool,//p_pipeline_pool
		p_pipeline//p_pipeline
	);

	printf("\nSquaring the numbers...\n\n");

	//
	//DISPATCH DATA TO WORK GROUPS
	//
	shCmdDispatch(
		cmd_buffer, 
		1, 
		1, 
		1
	);

	//
	//CREATE BARRIER TO ENSURE THAT SHADER WRITE OPERATIONS ARE FINISHED BEFORE BUFFER IS READ BACK FOR COPY
	//
	shSetBufferMemoryBarrier(
		device,
		cmd_buffer,
		device_local_buffer,
		VK_ACCESS_SHADER_WRITE_BIT,
		VK_ACCESS_TRANSFER_READ_BIT,
		compute_queue_family_index,
		compute_queue_family_index,
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT
	);

	//
	//STOP RECORDING COMMAND BUFFER
	//
	shEndCommandBuffer(cmd_buffer);

	//
	//SUBMIT OPERATIONS FROM COMMAND BUFFER TO COMPUTE compute_queue
	//
	shQueueSubmit(
		1,//cmd_buffer_count
		&cmd_buffer,//p_cmd_buffers
		compute_queue,//queue
		fence,//fence
		0,//semaphores_to_wait_for_count
		NULL,//p_semaphores_to_wait_for
		VK_PIPELINE_STAGE_TRANSFER_BIT,//wait_stage
		0,//signal_semaphore_count
		NULL//p_signal_semaphores
	);

	//
	//WAIT FOR COMMAND BUFFER TO END
	//
	shWaitForFences(
		device,//device
		1,//fence_count
		&fence,//p_fences
		1,//wait_for_all
		UINT64_MAX//timeout_ns
	);

	//
	//COPY DEVICE LOCAL BUFFER (NOW STORING OUTPUTS) TO STAGING BUFFER (HOST VISIBLE)
	//
	shResetFences(device, 1, &fence);

	shBeginCommandBuffer(cmd_buffer);

	shCopyBuffer(
		cmd_buffer,//transfer_cmd_buffer
		device_local_buffer,//src_buffer
		0,//src_offset
		0,//dst_offset
		sizeof(inputs),//size
		staging_buffer//dst_buffer
	);

	//
	// CREATE BARRIER TO ENSURE THAT BUFFER COPY IS FINISHED BEFORE HOST READING FROM IT
	//
	shSetBufferMemoryBarrier(
		device,
		cmd_buffer,
		staging_buffer,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		VK_ACCESS_HOST_READ_BIT,
		compute_queue_family_index,
		compute_queue_family_index,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_HOST_BIT
	);

	shEndCommandBuffer(cmd_buffer);

	shQueueSubmit(
		1,//cmd_buffer_count
		&cmd_buffer,//p_cmd_buffers
		compute_queue,//queue
		fence,//fence
		0,//semaphores_to_wait_for_count
		NULL,//p_semaphores_to_wait_for
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,//wait_stage
		0,//signal_semaphore_count
		NULL//p_signal_semaphores
	);

	shWaitForFences(
		device,//device
		1,//fence_count
		&fence,//p_fences
		1,//wait_for_all
		UINT64_MAX//timeout_ns
	);

	//
	//READ OUTPUT VALUES FROM STAGING MEMORY
	//
	float outputs[OUTPUT_COUNT];

	shReadMemory(
		device,//device
		staging_memory,//memory
		0,//offset
		sizeof(outputs),//data_size
		NULL,//pp_map_data
		outputs//p_dst_data
	);
	
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
	shPipelineDestroyShaderModules(device, 0, 1, p_pipeline);
	shPipelineDestroyLayout       (device, p_pipeline);
	shDestroyPipeline             (device, p_pipeline->pipeline);

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
		device,//device
		sizeof(inputs) + sizeof(factor),//size
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,//usage
		VK_SHARING_MODE_EXCLUSIVE,//sharing_mode
		p_staging_buffer//p_buffer
	);
	
	shAllocateBufferMemory(
		device,//device
		physical_device,//physical_device
		*p_staging_buffer,//buffer
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,//property_flags
		p_staging_memory//p_memory
	);

	shBindBufferMemory(
		device,//device
		*p_staging_buffer,//buffer
		0,//offset
		*p_staging_memory//buffer_memory
	);

	//
	//WRITE INPUTS TO STAGING BUFFER
	//
	shWriteMemory(
		device,//device
		*p_staging_memory,//memory
		0,//offset
		sizeof(inputs),//data_size
		inputs//p_data
	);
	shWriteMemory(
		device,//device
		*p_staging_memory,//memory
		sizeof(inputs),//offset
		sizeof(factor),//data_size
		&factor//p_data
	);

	//
	//CREATE INPUT/OUTPUT DEVICE LOCAL BUFFER 
	//

	shCreateBuffer(
		device,//device
		sizeof(inputs) + sizeof(factor),//size
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,//usage
		VK_SHARING_MODE_EXCLUSIVE,//sharing_mode
		p_device_local_buffer//p_buffer
	);

	shAllocateBufferMemory(
		device,//device
		physical_device,//physical_device
		*p_device_local_buffer,//buffer
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,//property_flags
		p_device_local_memory//p_memory
	);

	shBindBufferMemory(
		device,//device
		*p_device_local_buffer,//buffer
		0,//offset
		*p_device_local_memory//buffer_memory
	);

	//
	//COPY STAGING BUFFER TO DEVICE LOCAL MEMORY
	//
	shResetFences(device, 1, &fence);

	shBeginCommandBuffer(cmd_buffer);
	
	shCopyBuffer(
		cmd_buffer,//transfer_cmd_buffer
		*p_staging_buffer,//src_buffer
		0,//src_offset
		0,//dst_offset
		sizeof(inputs),//size
		*p_device_local_buffer//dst_buffer
	);
	
	shCopyBuffer(
		cmd_buffer,//transfer_cmd_buffer
		*p_staging_buffer,//src_buffer
		sizeof(factor),//src_offset
		sizeof(factor),//dst_offset
		sizeof(inputs),//size
		*p_device_local_buffer//dst_buffer
	);
	
	shEndCommandBuffer(cmd_buffer);
	
	shQueueSubmit(
		1,//cmd_buffer_count
		&cmd_buffer,//p_cmd_buffers
		compute_queue,//queue
		fence,//fence
		0,//semaphores_to_wait_for_count
		NULL,//p_semaphores_to_wait_for
		VK_PIPELINE_STAGE_TRANSFER_BIT,//wait_stage
		0,//signal_semaphore_count
		NULL//p_signal_semaphores
	);
	
	shWaitForFences(
		device,//device
		1,//fence_count
		&fence,//p_fences
		1,//wait_for_all
		UINT64_MAX//timeout_ns
	);
}

void setupPipeline(
	VkDevice          device, 
	VkBuffer          device_local_buffer, 
	ShVkPipelinePool* p_pipeline_pool
) {
	shPipelinePoolSetDescriptorBufferInfos(
		0,//first_descriptor
		1,//descriptor_count
		device_local_buffer,//buffer
		0,//buffer_offset
		sizeof(inputs),//buffer_size
		p_pipeline_pool//p_pipeline_pool
	);//each input value to compute shader is 4 bytes

	shPipelinePoolSetDescriptorBufferInfos(
		1,//first_descriptor
		1,//descriptor_count
		device_local_buffer,//buffer
		sizeof(inputs),//buffer_offset
		sizeof(factor),//buffer_size
		p_pipeline_pool//p_pipeline_pool
	); 



	shPipelinePoolCreateDescriptorSetLayoutBinding(
		0,//binding
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,//descriptor_type
		1,//descriptor_set_count
		VK_SHADER_STAGE_COMPUTE_BIT,//shader_stage
		p_pipeline_pool//p_pipeline_pool
	);

	shPipelinePoolCreateDescriptorSetLayout(
		device,//device
		0,//first_binding_idx
		1,//binding_count
		0,//set_layout_idx
		p_pipeline_pool//p_pipeline_pool
	);

	shPipelinePoolCopyDescriptorSetLayout(
		0,//src_set_layout_idx
		0,//first_dst_set_layout_idx
		2,//dst_set_layout_count
		p_pipeline_pool//p_pipeline_pool
	);//same descriptor set layout for both descriptor sets



	shPipelinePoolCreateDescriptorPool(
		device,//device
		0,//pool_idx
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,//descriptor_type
		2,//descriptor_count
		p_pipeline_pool//p_pipeline_pool
	);

	shPipelinePoolAllocateDescriptorSetUnits(
		device,//device
		0,//binding
		0,//pool_idx
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,//descriptor_type
		0,//first_descriptor_set_unit
		2,//descriptor_set_unit_count
		p_pipeline_pool//p_pipeline_pool
	);

	shPipelinePoolUpdateDescriptorSetUnits(
		device,//device
		0,//first_descriptor_set_unit
		2,//descriptor_set_unit_count
		p_pipeline_pool//p_pipeline_pool
	);

	ShVkPipeline* p_pipeline = &p_pipeline_pool->pipelines[0];

	uint32_t shader_size = 0;
	char* shader_code = readBinary(
		"../../examples/shaders/bin/power.comp.spv",
		&shader_size
	);

	shPipelineCreateShaderModule(
		device,//device
		shader_size,//size
		shader_code,//code
		p_pipeline//p_pipeline
	);
	
	shPipelineCreateShaderStage(
		VK_SHADER_STAGE_COMPUTE_BIT,//shader_stage
		p_pipeline//p_pipeline
	);
	
	shPipelineCreateLayout(
		device,//device
		0,//first_descriptor_set_layout
		2,//descriptor_set_layout_count
		p_pipeline_pool,//p_pipeline_pool
		p_pipeline//p_pipeline
	);

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