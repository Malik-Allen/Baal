// MIT License, Copyright (c) 2019 Malik Allen

#include "CommandPool.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/commands/CommandBuffer.h"
#include "../src/core/vulkan/debugging/Error.h"

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

			VK_CHECK(vkCreateCommandPool(device.GetVkDevice(), &createInfo, nullptr, &vkCommandPool), "creating command pool");

			DEBUG_LOG(LOG::INFO, "Created command pool... QueueFamilyIndex: {}", queueFamilyIndex);
		}

		CommandPool::~CommandPool() 
		{
			vkDestroyCommandPool(device.GetVkDevice(), vkCommandPool, nullptr);
		}

		VkResult CommandPool::CreateCommandBuffers(uint32_t count, VkCommandBufferLevel level, std::vector<CommandBuffer>& outCommandBuffers)
		{
			VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
			allocateInfo.commandPool = vkCommandPool;
			allocateInfo.level = level;
			allocateInfo.commandBufferCount = count;

			std::vector<VkCommandBuffer> buffers;
			buffers.resize(count);

			VkResult result;
			result = vkAllocateCommandBuffers(GetDevice().GetVkDevice(), &allocateInfo, buffers.data());

			for (size_t i = 0; i < buffers.size(); ++i) 
			{
				outCommandBuffers.push_back(CommandBuffer(*this, buffers[i]));
			}
			return result;
		}
	}
}