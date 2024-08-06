// MIT License, Copyright (c) 2024 Malik Allen

#include "PhysicalDevice.h"
#include "../src/core/vulkan/debugging/Error.h"

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
			queueFamilyProperties.resize(count);
			vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &count, queueFamilyProperties.data());

			DEBUG_LOG(LOG::INFO, "{} queue families avalable", count);
		}

		PhysicalDevice::~PhysicalDevice()
		{

		}

		VkPhysicalDevice PhysicalDevice::GetVkPhysicalDevice() const
		{
			return vkPhysicalDevice;
		}

		const VkPhysicalDeviceProperties& PhysicalDevice::GetProperties() const
		{
			return properties;
		}
		
		const std::vector<VkQueueFamilyProperties>& PhysicalDevice::GetQueueFamilyProperties() const
		{
			return queueFamilyProperties;
		}

		uint32_t PhysicalDevice::GetQueueFamilyIndex(VkQueueFlags flags) const
		{
			uint32_t size = queueFamilyProperties.size();
			for (uint32_t i = 0; i < size; ++i) 
			{
				if (queueFamilyProperties[i].queueFlags & flags) 
				{
					return i;
				}
			}
			throw std::runtime_error("Could not find matching queue family index");
		}

		VkFormat PhysicalDevice::GetSuitableDepthFormat(const std::vector<VkFormat>& inDepthformats)
		{
			VkFormat depthFormat = VK_FORMAT_UNDEFINED;

			for (auto& format : inDepthformats)
			{
				VkFormatProperties properties;
				vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, format, &properties);

				// Format must support depth stencil attachment for optimal tiling
				if (properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
				{
					depthFormat = format;
					break;
				}
			}

			if (depthFormat != VK_FORMAT_UNDEFINED)
			{
				DEBUG_LOG(LOG::INFO, "Depth format selected: {}", string_VkFormat(depthFormat));
				return depthFormat;
			}

			throw std::runtime_error("No suitable depth format could be determined");
		}
	}
}