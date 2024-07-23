// MIT License, Copyright (c) 2024 Malik Allen

#include "LogicalDevice.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"
#include "../src/core/vulkan/presentation/Surface.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		LogicalDevice::LogicalDevice(const PhysicalDevice& _physicalDevice, Surface& surface, const std::vector<const char*>& requiredExtensions):
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

			std::vector<VkExtensionProperties> availableExts;
			QueryAvailableExtensions(availableExts);

			std::vector<const char*> requestedExts(requiredExtensions);

			std::vector<const char*> enabledExts;
			for (auto requiredExt : requestedExts)
			{
				if (IsExtensionAvailable(requiredExt, availableExts))
				{
					enabledExts.push_back(requiredExt);
					DEBUG_LOG(LOG::INFO, "Enabled Ext: {}", requiredExt);
				}
				else
				{
					DEBUG_LOG(LOG::WARNING, "{} is NOT avaialble!", requiredExt);
				}
			}

			deviceInfo.enabledExtensionCount = enabledExts.size();
			deviceInfo.ppEnabledExtensionNames = enabledExts.data();

			VK_CHECK(vkCreateDevice(physicalDevice.GetVkPhysicalDevice(), &deviceInfo, nullptr, &device), "creating device");

			vkGetDeviceQueue(device, physicalDevice.GetQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT), 0, &graphicsQueue);
			
			for (uint32_t i = 0; i < queueFamilyCount; ++i) 
			{
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice.GetVkPhysicalDevice(), i, surface.GetVkSurface(), &presentSupport);

				if (presentSupport) {
					vkGetDeviceQueue(device, i, 0, &presentQueue);
					break;
				}
			}
		}

		LogicalDevice::~LogicalDevice()
		{
			vkDestroyDevice(device, nullptr);
		}

		void LogicalDevice::QueryAvailableExtensions(std::vector<VkExtensionProperties>& outExtensions) const
		{
			uint32_t extCount;
			vkEnumerateDeviceExtensionProperties(physicalDevice.GetVkPhysicalDevice(), nullptr, &extCount, nullptr);
			outExtensions.resize(extCount);
			vkEnumerateDeviceExtensionProperties(physicalDevice.GetVkPhysicalDevice(), nullptr, &extCount, outExtensions.data());

			DEBUG_LOG(LOG::INFO, "Available Device Extension Count: {}", extCount);

			for (const VkExtensionProperties& ext : outExtensions)
			{
				DEBUG_LOG(LOG::INFO, "Ext: {}", ext.extensionName);
			}
		}

		bool LogicalDevice::IsExtensionAvailable(const char* extensionName, const std::vector<VkExtensionProperties>& extensions) const
		{
			for (auto& availableExtension : extensions)
			{
				if (std::strcmp(availableExtension.extensionName, extensionName) == 0)
				{
					return true;
				}
			}
			return false;
		}
	}
}