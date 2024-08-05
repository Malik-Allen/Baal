// MIT License, Copyright (c) 2024 Malik Allen

#include "Sampler.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"

namespace Baal
{
	namespace VK
	{
		Sampler::Sampler(LogicalDevice& _device, PhysicalDevice& gpu):
			device(_device)
		{
			VkSamplerCreateInfo samplerInfo = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.anisotropyEnable = VK_FALSE;
			samplerInfo.maxAnisotropy = 1.0f;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;

			VK_CHECK(vkCreateSampler(device.GetVkDevice(), &samplerInfo, nullptr, &vkSampler), "creating sampler");			
		}

		Sampler::~Sampler()
		{
			vkDestroySampler(device.GetVkDevice(), vkSampler, nullptr);
		}
	}
}