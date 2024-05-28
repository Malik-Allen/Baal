// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_VK_PHYSICALDEVICE_H
#define BAAL_VK_PHYSICALDEVICE_H

#include <vulkan/vulkan_core.h>

namespace Baal
{
	namespace VK
	{
		// Represents a single implementation of Vulkan/GPUs 
		// Where Logical Device represents an instance of this implementation/GPU, with its own state, and resources

		class PhysicalDevice
		{
			friend class LogicalDevice;
		public:
			explicit PhysicalDevice(VkPhysicalDevice& _vkPhysicalDevice);
			PhysicalDevice(const PhysicalDevice&) = delete;
			PhysicalDevice(PhysicalDevice&&) = delete;

			~PhysicalDevice();

			PhysicalDevice& operator=(const PhysicalDevice&) = delete;
			PhysicalDevice& operator=(PhysicalDevice&&) = delete;

		private:
			VkPhysicalDevice vkPhysicalDevice{VK_NULL_HANDLE};

			VkPhysicalDevice& GetPhysicalDevice();
		};
	}
}

#endif //BAAL_VK_PHYSICALDEVICE_H