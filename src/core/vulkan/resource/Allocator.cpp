// MIT License, Copyright (c) 2024 Malik Allen

#include "Allocator.h"

#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/debugging/Error.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace Baal
{
	namespace VK
	{
		Allocator::Allocator(Instance& instance, LogicalDevice& _device)
		{
			const auto vulkanFunctions = VmaVulkanFunctions{
			.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
			.vkGetDeviceProcAddr = vkGetDeviceProcAddr,
			};

			VmaAllocatorCreateInfo allocatorInfo = {};
			allocatorInfo.instance = instance.GetVkInstance();
			allocatorInfo.physicalDevice = instance.GetGPU().GetVkPhysicalDevice();
			allocatorInfo.device = _device.GetVkDevice();
			allocatorInfo.pVulkanFunctions = &vulkanFunctions;

			VK_CHECK(vmaCreateAllocator(&allocatorInfo, &vmaAllocator), "creating vma allocator");
		}

		Allocator::~Allocator()
		{
			vmaDestroyAllocator(vmaAllocator);
		}
	}
}