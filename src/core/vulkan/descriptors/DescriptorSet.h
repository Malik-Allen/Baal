// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_DESCRIPTORSET_H
#define BAAL_VK_DESCRIPTORSET_H

#include <vulkan/vulkan_core.h>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;
		class DescriptorPool;
		class DescriptorSetLayout;

		class DescriptorSet
		{
		public:
			explicit DescriptorSet(LogicalDevice& _device, DescriptorPool& descriptorPool, DescriptorSetLayout& descriptorSetLayout);
			DescriptorSet(const DescriptorSet&) = delete;
			DescriptorSet(DescriptorSet&&) = delete;

			~DescriptorSet();

			DescriptorSet& operator=(const DescriptorSet&) = delete;
			DescriptorSet& operator=(DescriptorSet&&) = delete;

			VkDescriptorSet& GetVkDescriptorSet() { return vkDescriptorSet; }

		private:
			VkDescriptorSet vkDescriptorSet{ VK_NULL_HANDLE };
			LogicalDevice& device;
		};

	}
}

#endif // !BAAL_VK_DESCRIPTORSET_H