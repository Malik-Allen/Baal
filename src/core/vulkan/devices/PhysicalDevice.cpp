// MIT License, Copyright (c) 2019 Malik Allen

#include "PhysicalDevice.h"
#include "../src/utility/DebugLog.h"

namespace Baal
{
	namespace VK
	{
		PhysicalDevice::PhysicalDevice(VkPhysicalDevice& _vkPhysicalDevice) :
			vkPhysicalDevice(_vkPhysicalDevice)
		{
			vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);
			DEBUG_LOG(LOG::INFO, "Found GPU: \"{}\"", properties.deviceName);

			uint32_t count;
			vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &count, nullptr);
			queueFamilyproperties.resize(count);
			vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &count, queueFamilyproperties.data());

			DEBUG_LOG(LOG::INFO, "{} queue families avalable", count);
		}

		PhysicalDevice::~PhysicalDevice()
		{

		}

		VkPhysicalDevice PhysicalDevice::GetPhysicalDevice() const
		{
			return vkPhysicalDevice;
		}

		const VkPhysicalDeviceProperties& PhysicalDevice::GetProperties() const
		{
			return properties;
		}
		
		const std::vector<VkQueueFamilyProperties>& PhysicalDevice::GetQueueFamilyProperties() const
		{
			return queueFamilyproperties;
		}
	}
}