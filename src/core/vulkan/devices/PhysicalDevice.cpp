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
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(vkPhysicalDevice, &properties);
			DEBUG_LOG(LOG::INFO, "Found GPU: \"{}\"", properties.deviceName);
		}

		PhysicalDevice::~PhysicalDevice()
		{

		}
	}
}