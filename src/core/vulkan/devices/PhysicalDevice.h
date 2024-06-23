// MIT License, Copyright (c) 2023 Malik Allen

#ifndef BAAL_VK_PHYSICALDEVICE_H
#define BAAL_VK_PHYSICALDEVICE_H

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal
{
	namespace VK
	{
		// Represents a single implementation of Vulkan/GPUs 
		// Where Logical Device represents an instance of this implementation/GPU, with its own state, and resources

		class PhysicalDevice
		{
		public:
			explicit PhysicalDevice(VkPhysicalDevice& _vkPhysicalDevice);
			PhysicalDevice(const PhysicalDevice&) = delete;
			PhysicalDevice(PhysicalDevice&&) = delete;

			~PhysicalDevice();

			PhysicalDevice& operator=(const PhysicalDevice&) = delete;
			PhysicalDevice& operator=(PhysicalDevice&&) = delete;

			VkPhysicalDevice GetVkPhysicalDevice() const;

			const VkPhysicalDeviceProperties& GetProperties() const;
			const std::vector<VkQueueFamilyProperties>& GetQueueFamilyProperties() const;
			uint32_t GetQueueFamilyIndex(VkQueueFlags flags) const;

		private:
			VkPhysicalDevice vkPhysicalDevice{VK_NULL_HANDLE};
			VkPhysicalDeviceProperties properties;
			std::vector<VkQueueFamilyProperties> queueFamilyProperties;
		};
	}
}

#endif //BAAL_VK_PHYSICALDEVICE_H