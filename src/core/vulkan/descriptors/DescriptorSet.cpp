// MIT License, Copyright (c) 2024 Malik Allen

#include "DescriptorSet.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/descriptors/DescriptorPool.h"
#include "../src/core/vulkan/descriptors/DescriptorSetLayout.h"
#include "../src/core/vulkan/resource/Buffer.h"

namespace Baal
{
	namespace VK
	{
		DescriptorSet::DescriptorSet(LogicalDevice& _device, DescriptorPool& descriptorPool, DescriptorSetLayout& descriptorSetLayout):
			device(_device)
		{
			VkDescriptorSetAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
			allocInfo.descriptorPool = descriptorPool.GetVkDescriptorPool();
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &descriptorSetLayout.GetVkDescriptorSetLayout();

			VK_CHECK(vkAllocateDescriptorSets(device.GetVkDevice(), &allocInfo, &vkDescriptorSet), "allocating descriptor sets");
		}

		DescriptorSet::~DescriptorSet()
		{
			// Descriptor pool manages memory lifetime of descriptor sets
		}
	}
}