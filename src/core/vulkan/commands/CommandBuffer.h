// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_VK_COMMANDBUFFER_H
#define BAAL_VK_COMMANDBUFFER_H

#include <vulkan/vulkan_core.h>

namespace Baal
{
	namespace VK
	{
		class CommandPool;

		// Used to record commands to be executed by the logical device
		class CommandBuffer
		{
		public:
			explicit CommandBuffer(CommandPool& _commandPool, VkCommandBufferLevel level);

			CommandBuffer(const CommandBuffer&) = delete;
			CommandBuffer(CommandBuffer&&) = delete;

			~CommandBuffer();

			CommandBuffer& operator=(const CommandBuffer&) = delete;
			CommandBuffer& operator=(CommandBuffer&&) = delete;

		private:
			VkCommandBuffer vkCommandBuffer{VK_NULL_HANDLE};
			CommandPool& commandPool;
		};
	}
}

#endif // !BAAL_VK_COMMANDBUFFER_H
