// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_LOGICAL_DEVICE_H
#define BAAL_LOGICAL_DEVICE_H

#include <vulkan/vulkan_core.h>

namespace Baal
{
	namespace VK
	{
		class PhysicalDevice;

		// The interface that is used to interact with the vkPhysicalDevice


		class LogicalDevice
		{
			explicit LogicalDevice(PhysicalDevice& physicalDevice);
			LogicalDevice(const LogicalDevice&) = delete;
			LogicalDevice(LogicalDevice&&) = delete;

			~LogicalDevice();

			LogicalDevice& operator=(const LogicalDevice&) = delete;
			LogicalDevice& operator = (LogicalDevice&&) = delete;
		};
	}
}

#endif // BAAL_LOGICAL_DEVICE_H