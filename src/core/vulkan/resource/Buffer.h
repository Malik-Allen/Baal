// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_BUFFER_H
#define BAAL_VK_BUFFER_H

#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>
#include <vector>
#include <memory>

namespace Baal
{
	namespace VK
	{
		class Allocator;

		class Buffer
		{
		public:
			explicit Buffer(Allocator& _allocator, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkDeviceSize _size, std::vector<uint32_t> queueFamilyIndicies = {});
			Buffer(const Buffer&) = delete;
			Buffer(Buffer&& other) noexcept;

			~Buffer();

			Buffer& operator=(const Buffer&) = delete;
			Buffer& operator = (Buffer&&) = delete;

			VkBuffer& GetVkBuffer() { return vkBuffer; }
			uint64_t GetSize() const { return size; }

			void Map();
			void Unmap();
			void Flush();
			size_t Update(void* data, const size_t _size, size_t offset = 0);

			static Buffer CreateStagingBuffer(Allocator& allocator, VkDeviceSize _size, void* data);
		private:
			VkBuffer vkBuffer{ VK_NULL_HANDLE };
			VmaAllocation vmaAllocation{ VK_NULL_HANDLE };
			Allocator& allocator;
			uint64_t size;
			uint8_t bIsMapped:1 = false;
			uint8_t* mappedData{ nullptr };
			uint8_t bIsCoherent:1 = false;
		};
	}
}

#endif // !BAAL_VK_BUFFER_H