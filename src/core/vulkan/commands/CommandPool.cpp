// MIT License, Copyright (c) 2019 Malik Allen

#include "CommandPool.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/utility/DebugLog.h"

#include <stdexcept>

namespace Baal 
{
	namespace VK 
	{
		CommandPool::CommandPool(LogicalDevice& _device, uint32_t queueFamilyIndex):
			vkCommandPool(VK_NULL_HANDLE),
			device(_device)
		{
			VkCommandPoolCreateInfo createInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
			createInfo.queueFamilyIndex = queueFamilyIndex;
			createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			VkResult result;
			result = vkCreateCommandPool(device.GetVkDevice(), &createInfo, nullptr, &vkCommandPool);

			if (result != VK_SUCCESS) 
			{
				throw std::runtime_error("Failed to create command pool!");
			}

			DEBUG_LOG(LOG::INFO, "Created command pool... QueueFamilyIndex: {}", queueFamilyIndex);
		}

		CommandPool::~CommandPool() 
		{
			vkDestroyCommandPool(device.GetVkDevice(), vkCommandPool, nullptr);
		}
	}
}