// MIT License, Copyright (c) 2024 Malik Allen

#include "GraphicsPipeline.h"

#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/pipeline/ShaderModule.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		GraphicsPipeline::GraphicsPipeline(LogicalDevice& _device, std::vector<ShaderModule>& _shaderStages):
			device(_device),
			shaderStages(_shaderStages)
		{
			std::vector<VkPipelineShaderStageCreateInfo> pipelineStages(shaderStages.size());

			VkPipelineShaderStageCreateInfo shaderStageInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
			for (size_t i = 0; i < shaderStages.size(); ++i)
			{
				shaderStageInfo.stage = shaderStages[i].GetStage();
				shaderStageInfo.module = shaderStages[i].GetVkShaderModule();
				shaderStageInfo.pName = "main";

				pipelineStages[i] = shaderStageInfo;
			}

			VkGraphicsPipelineCreateInfo graphicsPipelineInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };

			graphicsPipelineInfo.stageCount = pipelineStages.size();
			graphicsPipelineInfo.pStages = pipelineStages.data();
			
			VK_CHECK(vkCreateGraphicsPipelines(device.GetVkDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &pipeline), "creating graphics pipeline");
		}

		GraphicsPipeline::~GraphicsPipeline()
		{
			shaderStages.clear();

			vkDestroyPipeline(device.GetVkDevice(), pipeline, nullptr);
		}
	}
}