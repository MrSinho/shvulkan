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


const char* readBinary(const char* path, uint32_t* p_size);


typedef struct ShaderInput {
	float value;
} ShaderInput;
SH_VULKAN_GENERATE_DESCRIPTOR_STRUCTURE_MAP(ShaderInput)



int main(void) {

	ShVkCore core = { 0 };
	{
		shCreateInstance(&core, "shvulkan compute example", "shvulkan engine", VALIDATION_LAYERS_ENABLED, 0, NULL);
		shSelectPhysicalDevice(&core, VK_QUEUE_COMPUTE_BIT);
		shSetLogicalDevice(&core);
		shGetComputeQueue(&core);
		shCreateComputeCommandBuffers(&core, 1);
		shSetSyncObjects(&core);
	}
	

	ShVkPipeline* p_pipeline = shAllocateShVkPipeline();

	//Generated with macro definition
	ShaderInputDescriptorStructureMap inputs = shVkCreateShaderInputDescriptorStructures(
		core.physical_device_properties, 
		16
	);
	{
		for (uint32_t i = 0; i < inputs.structure_count; i++) {
			ShaderInput* p_input = shVkGetShaderInputDescriptorStructure(inputs, i, 0);
			p_input->value = (float)(i);
		}
		shVkMapShaderInputDecriptorStructures(&inputs);

		shPipelineCreateDescriptorBuffer(core.device, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 0, inputs.structure_size * inputs.structure_count, p_pipeline);
		shPipelineAllocateDescriptorBufferMemory(core.device, core.physical_device, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, p_pipeline);
		shPipelineBindDescriptorBufferMemory(core.device, 0, 0, p_pipeline);

		shPipelineCreateDescriptorPool(core.device, 0, p_pipeline);
		shPipelineDescriptorSetLayout(core.device, 0, 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, p_pipeline);
		shPipelineAllocateDescriptorSet(core.device, 0, p_pipeline);

		uint32_t shader_size = 0;
		const char* shader_code = readBinary("../examples/shaders/bin/power.comp.spv", &shader_size);
		shPipelineCreateShaderModule(core.device, shader_size, shader_code, p_pipeline);
		shPipelineCreateShaderStage(core.device, VK_SHADER_STAGE_COMPUTE_BIT, p_pipeline);

		shSetupComputePipeline(core.device, p_pipeline);
	}


	{/*OPERATIONS HERE*/
		shResetCommandBuffer(core.p_compute_commands[0].cmd_buffer);
		shResetFence(core.device, &core.p_compute_commands[0].fence);

		shBeginCommandBuffer(core.p_compute_commands[0].cmd_buffer);

		shBindPipeline(core.p_compute_commands[0].cmd_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, p_pipeline);

		shPipelineUpdateDescriptorSets(core.device, p_pipeline);

		shPipelineWriteDescriptorBufferMemory(core.device, 0, inputs.p_ShaderInput_map, p_pipeline);
		shPipelineBindDescriptorSet(core.p_compute_commands[0].cmd_buffer, 0, VK_PIPELINE_BIND_POINT_COMPUTE, p_pipeline);

		shCmdDispatch(core.p_compute_commands[0].cmd_buffer, 64, 1, 1);

		shEndCommandBuffer(core.p_compute_commands[0].cmd_buffer);

		shQueueSubmit(1, &core.p_compute_commands[0].cmd_buffer, core.compute_queue.queue, core.p_compute_commands[0].fence);

		shWaitForFence(core.device, &core.p_compute_commands[0].fence);

		shReadMemory(core.device, p_pipeline->descriptor_buffers_memory[0], 0, 64 * sizeof(float), inputs.p_ShaderInput_map);
		
		printf("Compute shader output values:\n");
		for (uint32_t i = 0; i < inputs.structure_count; i++) {
			ShaderInput* p_input = shVkGetShaderInputDescriptorStructure(inputs, i, 1);
			printf("%f\n", p_input->value);
		}
	}

	shVkReleaseShaderInputDescriptorStructureMap(&inputs);

	shPipelineClearDescriptorBufferMemory(core.device, 0, p_pipeline);

	shPipelineRelease(core.device, p_pipeline);

	shVulkanRelease(&core);

	return 0;
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