// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_BUFFER_H
#define BAAL_VK_BUFFER_H

#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;
		class Allocator;

		class Buffer
		{
		public:
			explicit Buffer(Allocator& _allocator, VkBufferUsageFlags usage, VkDeviceSize size, void* data, std::vector<uint32_t> queueFamilyIndicies = {});
			Buffer(const Buffer&) = delete;
			Buffer(Buffer&& other) noexcept;

			~Buffer();

			Buffer& operator=(const Buffer&) = delete;
			Buffer& operator = (Buffer&&) = delete;

			VkBuffer& GetVkBuffer() { return vkBuffer; }

		private:
			VkBuffer vkBuffer{ VK_NULL_HANDLE };
			VmaAllocation vmaAllocation{ VK_NULL_HANDLE };
			Allocator& allocator;
		};
	}
}

#endif // !BAAL_VK_BUFFER_H