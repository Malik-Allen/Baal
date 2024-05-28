// MIT License, Copyright (c) 2019 Malik Allen

#include "PhysicalDevice.h"
#include "../src/utility/DebugLog.h"

#include <vector>

namespace Baal
{
	namespace VK
	{
		PhysicalDevice::PhysicalDevice(VkPhysicalDevice& _vkPhysicalDevice) :
			vkPhysicalDevice(_vkPhysicalDevice)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);
			DEBUG_LOG(LOG::INFO, "Found GPU: \"{}\"", properties.deviceName);

			uint32_t count;
			vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &count, nullptr);
			std::vector<VkQueueFamilyProperties> queueFamilyproperties;
			vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &count, queueFamilyproperties.data());

			DEBUG_LOG(LOG::INFO, "{} queue families avalable", count);
		}

		PhysicalDevice::~PhysicalDevice()
		{

		}

		VkPhysicalDevice& PhysicalDevice::GetPhysicalDevice()
		{
			return vkPhysicalDevice;
		}
	}
}