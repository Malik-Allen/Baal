// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_VK_COMMANDPOOL_H
#define BAAL_VK_COMMANDPOOL_H

#include <vulkan/vulkan_core.h>

namespace Baal 
{
	namespace VK 
	{
		class LogicalDevice;

		// Block of memory where Command Buffers are allocated from
		class CommandPool
		{
		public:
			explicit CommandPool(LogicalDevice& _device, uint32_t queueFamilyIndex);

			CommandPool(const CommandPool&) = delete;
			CommandPool(CommandPool&&) = delete;

			~CommandPool();

			CommandPool& operator=(const CommandPool&) = delete;
			CommandPool& operator=(CommandPool&&) = delete;

			VkCommandPool& GetVkCommandPool() { return vkCommandPool; }
			LogicalDevice& GetDevice() { return device; };

		private:
			VkCommandPool vkCommandPool{ VK_NULL_HANDLE };
			LogicalDevice& device;
		};
	}
}

#endif // BAAL_VK_COMMANDPOOL_H