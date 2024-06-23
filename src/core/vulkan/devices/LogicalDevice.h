// MIT License, Copyright (c) 2023 Malik Allen

#ifndef BAAL_LOGICAL_DEVICE_H
#define BAAL_LOGICAL_DEVICE_H

#include <vulkan/vulkan_core.h>
#include <memory>

namespace Baal
{
	namespace VK
	{
		class PhysicalDevice;
		class CommandPool;

		// The interface that is used to interact with the vkPhysicalDevice
		
		class LogicalDevice
		{
		public:
			explicit LogicalDevice(const PhysicalDevice& _physicalDevice);
			LogicalDevice(const LogicalDevice&) = delete;
			LogicalDevice(LogicalDevice&&) = delete;

			~LogicalDevice();

			LogicalDevice& operator=(const LogicalDevice&) = delete;
			LogicalDevice& operator = (LogicalDevice&&) = delete;

			VkDevice& GetVkDevice() { return device; }

		private:
			const PhysicalDevice& physicalDevice;
			VkDevice device{ VK_NULL_HANDLE };
			std::unique_ptr<CommandPool> commandPool;
		};
	}
}

#endif // BAAL_LOGICAL_DEVICE_H