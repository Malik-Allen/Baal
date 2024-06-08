// MIT License, Copyright (c) 2019 Malik Allen

#include "LogicalDevice.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"

#include <stdexcept>

namespace Baal
{
	namespace VK
	{
		LogicalDevice::LogicalDevice(const PhysicalDevice& _physicalDevice):
			physicalDevice(_physicalDevice)
		{
			std::vector<VkDeviceQueueCreateInfo> deviceQueueInfos;

			const uint32_t queueFamilyCount = physicalDevice.GetQueueFamilyProperties().size();
			std::vector<std::vector<float>> queueFamilyPriorities(queueFamilyCount);

			VkDeviceQueueCreateInfo queueInfo = {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};

			for(uint32_t i = 0; i < physicalDevice.GetQueueFamilyProperties().size(); ++i)
			{
				const VkQueueFamilyProperties& queueFamilyProperty = physicalDevice.GetQueueFamilyProperties()[i];

				// Question: Why is the graphics queue family given the highest priority?
				// Answer: Is up to you to decide if the GPU priortizes graphics queue, we could assign all queues to just have the priority as well

				queueFamilyPriorities[i].resize(queueFamilyProperty.queueCount, 0.5f);	// Setting all queue families with the same priority
				

				queueInfo.queueFamilyIndex = i;
				queueInfo.queueCount = queueFamilyProperty.queueCount;		
				queueInfo.pQueuePriorities = queueFamilyPriorities[i].data();
				deviceQueueInfos.push_back(queueInfo);
			}


			VkDeviceCreateInfo deviceInfo = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
			deviceInfo.queueCreateInfoCount = deviceQueueInfos.size();
			deviceInfo.pQueueCreateInfos = deviceQueueInfos.data();

			vkCreateDevice(physicalDevice.GetPhysicalDevice(), &deviceInfo, nullptr, &device);
		}

		LogicalDevice::~LogicalDevice()
		{
			vkDestroyDevice(device, nullptr);
		}
	}
}