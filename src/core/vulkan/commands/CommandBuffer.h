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
			CommandBuffer() = delete;
			CommandBuffer(CommandPool& _commandPool, VkCommandBufferLevel level);
			CommandBuffer(CommandPool& _commandPool, VkCommandBuffer& _commandBuffer);

			CommandBuffer(const CommandBuffer&) = delete;
			CommandBuffer(CommandBuffer&& other) noexcept;

			~CommandBuffer();

			CommandBuffer& operator=(const CommandBuffer&) = delete;
			CommandBuffer& operator=(CommandBuffer&&) = delete;

			void BeginRecording(VkCommandBufferUsageFlags flags);
			void EndRecording();
			void Reset();

			VkCommandBuffer& GetVkCommandBuffer() { return vkCommandBuffer; }

		private:
			VkCommandBuffer vkCommandBuffer{VK_NULL_HANDLE};
			CommandPool& commandPool;
		};
	}
}

#endif // !BAAL_VK_COMMANDBUFFER_H
