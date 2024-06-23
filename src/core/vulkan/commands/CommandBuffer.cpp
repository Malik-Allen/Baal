// MIT License, Copyright (c) 2019 Malik Allen

#include "CommandBuffer.h"
#include "../src/core/vulkan/commands/CommandPool.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"

#include <stdexcept>

namespace Baal
{
	namespace VK
	{
		CommandBuffer::CommandBuffer(CommandPool& _commandPool, VkCommandBufferLevel level):
			commandPool(_commandPool)
		{
			VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
			allocateInfo.commandPool = commandPool.GetVkCommandPool();
			allocateInfo.level = level;
			allocateInfo.commandBufferCount = 1;

			VkResult result;
			result = vkAllocateCommandBuffers(commandPool.GetDevice().GetVkDevice(), &allocateInfo, &vkCommandBuffer);

			if (result != VK_SUCCESS) 
			{
				throw std::runtime_error("Failed to allocate command buffer memory!");
			}
		}

		CommandBuffer::~CommandBuffer()
		{
			vkFreeCommandBuffers(commandPool.GetDevice().GetVkDevice(), commandPool.GetVkCommandPool(), 1, &vkCommandBuffer);
		}
	}
}