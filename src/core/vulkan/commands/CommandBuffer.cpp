// MIT License, Copyright (c) 2019 Malik Allen

#include "CommandBuffer.h"
#include "../src/core/vulkan/commands/CommandPool.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/debugging/Error.h"

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

			VK_CHECK(vkAllocateCommandBuffers(commandPool.GetDevice().GetVkDevice(), &allocateInfo, &vkCommandBuffer), "allocatiing command buffer");
		}

		CommandBuffer::CommandBuffer(CommandPool& _commandPool, VkCommandBuffer& _commandBuffer) :
			commandPool(_commandPool),
			vkCommandBuffer(_commandBuffer)
		{
		}

		CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept :
			commandPool(other.commandPool)
		{
			if (this != &other) 
			{
				vkCommandBuffer = other.vkCommandBuffer;
				other.vkCommandBuffer = VK_NULL_HANDLE;
			}
		}

		CommandBuffer::~CommandBuffer()
		{
			if (vkCommandBuffer != VK_NULL_HANDLE) 
			{
				vkFreeCommandBuffers(commandPool.GetDevice().GetVkDevice(), commandPool.GetVkCommandPool(), 1, &vkCommandBuffer);
			}
		}

		void CommandBuffer::BeginRecording(VkCommandBufferUsageFlags flags)
		{
			VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			beginInfo.flags = flags;

			VK_CHECK(vkBeginCommandBuffer(vkCommandBuffer, &beginInfo), "beginning command buffer recording");
		}

		void CommandBuffer::EndRecording()
		{
			VK_CHECK(vkEndCommandBuffer(vkCommandBuffer), "ending command buffer recording");
		}

		void CommandBuffer::Reset()
		{
			VK_CHECK(vkResetCommandBuffer(vkCommandBuffer, 0), "resetting command buffer");
		}
	}
}