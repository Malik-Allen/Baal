// MIT License, Copyright (c) 2024 Malik Allen

#include "Sampler.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"

namespace Baal
{
	namespace VK
	{
		Sampler::Sampler(LogicalDevice& _device, PhysicalDevice& gpu, VkSamplerCreateInfo& samplerCreateInfo):
			device(_device)
		{
			VK_CHECK(vkCreateSampler(device.GetVkDevice(), &samplerCreateInfo, nullptr, &vkSampler), "creating sampler");			
		}

		Sampler::~Sampler()
		{
			vkDestroySampler(device.GetVkDevice(), vkSampler, nullptr);
		}
	}
}