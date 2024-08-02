// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_IMAGE_H
#define BAAL_VK_IMAGE_H

#include <vulkan/vulkan_core.h>
#include <vector>
#include <vk_mem_alloc.h>

namespace Baal
{
	namespace VK
	{
		class Allocator;

		class Image
		{
		public:
			explicit Image(Allocator& _allocator, const uint32_t width, const uint32_t height, VkImageType type, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkSampleCountFlagBits samples, VkDeviceSize _size, void* data);
			Image(const Image&) = delete;
			Image(Image&& other) noexcept;

			~Image();

			Image& operator=(const Image&) = delete;
			Image& operator = (Image&&) = delete;

			void Update(void* data, const size_t _size);

		private:
			VkImage vkImage{ VK_NULL_HANDLE };
			VmaAllocation vmaAllocation{ VK_NULL_HANDLE };
			Allocator& allocator;
			uint8_t* mappedData{ nullptr };
			uint64_t size;
			bool bIsMapped = false;

			void Map();
		};
	}
}

#endif // !BAAL_VK_IMAGE_H