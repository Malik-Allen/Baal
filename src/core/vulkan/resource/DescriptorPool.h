// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_DESCRIPTORPOOL_H
#define BAAL_VK_DESCRIPTORPOOL_H

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal 
{
	namespace VK 
	{
		class LogicalDevice;

		struct DescriptorPoolSize
		{
			VkDescriptorType type;
			uint32_t count;
		};

		// Block of memory where Command Buffers are allocated from
		class DescriptorPool
		{
		public:
			explicit DescriptorPool(LogicalDevice& _device, const std::vector<DescriptorPoolSize>& poolSizes);

			DescriptorPool(const DescriptorPool&) = delete;
			DescriptorPool(DescriptorPool&&) = delete;

			~DescriptorPool();

			DescriptorPool& operator=(const DescriptorPool&) = delete;
			DescriptorPool& operator=(DescriptorPool&&) = delete;

			VkDescriptorPool& GetVkDescriptorPool() { return vkDescriptorPool; }

		private:
			VkDescriptorPool vkDescriptorPool{ VK_NULL_HANDLE };
			LogicalDevice& device;
		};
	}
}

#endif // BAAL_VK_DESCRIPTORPOOL_H