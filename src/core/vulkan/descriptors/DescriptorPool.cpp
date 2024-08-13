// MIT License, Copyright (c) 2024 Malik Allen

#include "DescriptorPool.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		DescriptorPool::DescriptorPool(LogicalDevice& _device, const std::vector<DescriptorPoolSize>& poolSizes):
			device(_device)
		{
			VkDescriptorPoolCreateInfo poolInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};

			std::vector<VkDescriptorPoolSize> descriptorPoolSizes;
			for(const auto& poolSize : poolSizes)
			{
				VkDescriptorPoolSize descPoolSize = {};
				descPoolSize.type = poolSize.type;
				descPoolSize.descriptorCount = poolSize.count;
				
				descriptorPoolSizes.push_back(descPoolSize);
			}

			poolInfo.poolSizeCount = descriptorPoolSizes.size();
			poolInfo.pPoolSizes = descriptorPoolSizes.data();
			poolInfo.maxSets = 10;
			
			VK_CHECK(vkCreateDescriptorPool(device.GetVkDevice(), &poolInfo, nullptr, &vkDescriptorPool), "creating descriptor pool");
		}

		DescriptorPool::~DescriptorPool()
		{
			vkDestroyDescriptorPool(device.GetVkDevice(), vkDescriptorPool, nullptr);
		}
	}
}