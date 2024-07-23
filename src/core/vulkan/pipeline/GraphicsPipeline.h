// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_GRAPHICS_PIPELINE_H
#define BAAL_GRAPHICS_PIPELINE_H

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;
		struct ShaderInfo;
		class ShaderModule;
		class RenderPass;

		// Sets up Shader Stages and Fixed-Function stages of pipeline

		class GraphicsPipeline
		{
		public:
			explicit GraphicsPipeline(LogicalDevice& _device, std::vector<ShaderInfo>& shaderInfo, RenderPass& renderPass, const uint32_t width, const uint32_t height);
			GraphicsPipeline(const GraphicsPipeline&) = delete;
			GraphicsPipeline(GraphicsPipeline&&) = delete;

			~GraphicsPipeline();

			GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
			GraphicsPipeline& operator = (GraphicsPipeline&&) = delete;

			VkPipeline& GetVkGraphicsPipeline() { return pipeline; }

		private:
			VkPipeline pipeline{VK_NULL_HANDLE};
			VkPipelineLayout layout{ VK_NULL_HANDLE };
			LogicalDevice& device;
			std::vector<ShaderModule> shaderStages;
		};
	}
}

#endif // !BAAL_GRAPHICS_PIPELINE_H
