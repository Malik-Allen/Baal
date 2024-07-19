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
		class ShaderModule;

		class GraphicsPipeline
		{
		public:
			GraphicsPipeline(LogicalDevice& _device, std::vector<ShaderModule>& _shaderStages);
			GraphicsPipeline(const GraphicsPipeline&) = delete;
			GraphicsPipeline(GraphicsPipeline&&) = delete;

			~GraphicsPipeline();

			GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
			GraphicsPipeline& operator = (GraphicsPipeline&&) = delete;

		private:
			VkPipeline pipeline{VK_NULL_HANDLE};
			LogicalDevice& device;
			std::vector<ShaderModule>& shaderStages;
		};
	}
}

#endif // !BAAL_GRAPHICS_PIPELINE_H
