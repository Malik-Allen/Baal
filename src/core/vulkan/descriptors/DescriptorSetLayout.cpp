// MIT License, Copyright (c) 2024 Malik Allen

#include "DescriptorSetLayout.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"

namespace Baal
{
	namespace VK
	{
		DescriptorSetLayout::DescriptorSetLayout(LogicalDevice& _device, std::vector<DescriptorSetBinding> bindings): 
			device(_device)
		{
			std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

			for (size_t i = 0; i < bindings.size(); ++i)
			{
				VkDescriptorSetLayoutBinding layoutBinding;
				layoutBinding.binding = bindings[i].binding;
				layoutBinding.descriptorType = bindings[i].type;
				layoutBinding.descriptorCount = bindings[i].count;
				layoutBinding.stageFlags = bindings[i].stage;

				if (layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER)
				{
					layoutBinding.pImmutableSamplers = nullptr;
				}
				layoutBindings.push_back(layoutBinding);
			}

			VkDescriptorSetLayoutCreateInfo descritptorSetLayoutInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
			descritptorSetLayoutInfo.bindingCount = layoutBindings.size();
			descritptorSetLayoutInfo.pBindings = layoutBindings.data();

			VK_CHECK(vkCreateDescriptorSetLayout(device.GetVkDevice(), &descritptorSetLayoutInfo, nullptr, &vkDescriptorSetLayout), "creating descriptor set layout");
		}

		DescriptorSetLayout::~DescriptorSetLayout()
		{
			vkDestroyDescriptorSetLayout(device.GetVkDevice(), vkDescriptorSetLayout, nullptr);
		}
	}
}