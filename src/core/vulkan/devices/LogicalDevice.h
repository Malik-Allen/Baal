// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_LOGICAL_DEVICE_H
#define BAAL_LOGICAL_DEVICE_H

#include <vulkan/vulkan_core.h>
#include <memory>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class PhysicalDevice;

		// The interface that is used to interact with the vkPhysicalDevice
		
		class LogicalDevice
		{
		public:
			explicit LogicalDevice(const PhysicalDevice& _physicalDevice, const std::vector<const char*>& requiredExtensions = {});
			LogicalDevice(const LogicalDevice&) = delete;
			LogicalDevice(LogicalDevice&&) = delete;

			~LogicalDevice();

			LogicalDevice& operator=(const LogicalDevice&) = delete;
			LogicalDevice& operator = (LogicalDevice&&) = delete;

			VkDevice& GetVkDevice() { return device; }
			VkQueue& GetGraphicsQueue() { return graphicsQueue; };

		private:
			const PhysicalDevice& physicalDevice;
			VkDevice device{ VK_NULL_HANDLE };
			VkQueue graphicsQueue{ VK_NULL_HANDLE };
			std::vector<const char*> enabledExtensions;

			void QueryAvailableExtensions(std::vector<VkExtensionProperties>& outExtensions) const;
			bool IsExtensionAvailable(const char* extensionName, const std::vector<VkExtensionProperties>& extensions) const;
		};
	}
}

#endif // BAAL_LOGICAL_DEVICE_H