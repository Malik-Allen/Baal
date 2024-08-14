// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_SAMPLER_H
#define BAAL_VK_SAMPLER_H

#include <vulkan/vulkan_core.h>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;
		class PhysicalDevice;

		class Sampler
		{
		public:
			explicit Sampler(LogicalDevice& _device, PhysicalDevice& gpu, VkSamplerCreateInfo& samplerCreateInfo);
			Sampler(const Sampler&) = delete;
			Sampler(Sampler&&) noexcept = delete;

			~Sampler();

			Sampler& operator=(const Sampler&) = delete;
			Sampler& operator = (Sampler&&) = delete;

			VkSampler& GetVkSampler() { return vkSampler; }

		private:
			VkSampler vkSampler{ VK_NULL_HANDLE };
			LogicalDevice& device;
		};
	}
}

#endif // !BAAL_VK_SAMPLER_H