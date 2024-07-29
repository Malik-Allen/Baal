// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_BUFFER_H
#define BAAL_VK_BUFFER_H

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;

		class Buffer
		{
		public:
			explicit Buffer(LogicalDevice& _device, VkBufferUsageFlags usage, VkDeviceSize size, void* data, std::vector<uint32_t> queueFamilyIndicies = {});
			Buffer(const Buffer&) = delete;
			Buffer(Buffer&& other) noexcept;

			~Buffer();

			Buffer& operator=(const Buffer&) = delete;
			Buffer& operator = (Buffer&&) = delete;

			VkBuffer& GetVkBuffer() { return vkBuffer; }
		private:
			VkBuffer vkBuffer{ VK_NULL_HANDLE };
			VkDeviceMemory vkBufferMemory{ VK_NULL_HANDLE };
			LogicalDevice& device;
		};
	}
}

#endif // !BAAL_VK_BUFFER_H