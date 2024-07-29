// MIT License, Copyright (c) 2024 Malik Allen

#include "Allocator.h"

#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		Allocator::Allocator(Instance& instance, LogicalDevice& _device)
		{
			VmaAllocatorCreateInfo allocatorInfo = {};
			allocatorInfo.instance = instance.GetVkInstance();
			allocatorInfo.physicalDevice = instance.GetGPU().GetVkPhysicalDevice();
			allocatorInfo.device = _device.GetVkDevice();

			VK_CHECK(vmaCreateAllocator(&allocatorInfo, &vmaAllocator), "creating vma allocator");
		}

		Allocator::~Allocator()
		{
			vmaDestroyAllocator(vmaAllocator);
		}
	}
}