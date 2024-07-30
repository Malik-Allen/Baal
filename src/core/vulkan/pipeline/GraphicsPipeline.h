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
		class DescriptorPool;

		struct DescriptorSetBinding
		{
			VkDescriptorType type;
			VkShaderStageFlags stage;
			uint32_t binding;
			uint32_t count;
		};

		// Sets up Shader Stages and Fixed-Function stages of pipeline

		class GraphicsPipeline
		{
		public:
			explicit GraphicsPipeline(
				LogicalDevice& _device, 
				std::vector<ShaderInfo>& shaderInfo, 
				RenderPass& renderPass, 
				DescriptorPool& descriptorPool,
				const std::vector<DescriptorSetBinding>& descriptorSetBindings,
				const uint32_t width, 
				const uint32_t height);

			GraphicsPipeline(const GraphicsPipeline&) = delete;
			GraphicsPipeline(GraphicsPipeline&&) = delete;

			~GraphicsPipeline();

			GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
			GraphicsPipeline& operator = (GraphicsPipeline&&) = delete;

			VkPipeline& GetVkGraphicsPipeline() { return pipeline; }
			VkPipelineLayout& GetVkGraphicsPipelineLayout() { return layout; }
			VkDescriptorSet& GetVkDescriptorSet() { return descriptorSet; }

		private:
			VkPipeline pipeline{VK_NULL_HANDLE};
			VkPipelineLayout layout{ VK_NULL_HANDLE };
			VkDescriptorSetLayout  descriptorSetLayout{ VK_NULL_HANDLE };
			VkDescriptorSet descriptorSet{ VK_NULL_HANDLE };
			LogicalDevice& device;
			std::vector<ShaderModule> shaderStages;

			std::vector<VkVertexInputAttributeDescription> GetVertexAttributes() const;
			std::vector<VkDescriptorSetLayoutBinding> CreateDescriptorSetLayoutBindings(const std::vector<DescriptorSetBinding>& descriptorSetBinding) const;
		};
	}
}

#endif // !BAAL_GRAPHICS_PIPELINE_H
