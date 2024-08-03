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
		class LogicalDevice;

		class Image
		{
		public:
			explicit Image(
				LogicalDevice& _device, 
				const uint32_t width, 
				const uint32_t height, 
				VkImageType type, 
				VkFormat format, 
				VkImageTiling tiling, 
				VkImageUsageFlags usage, 
				VkSampleCountFlagBits samples,
				VkImageViewType viewType,
				VkDeviceSize _size, 
				void* data
			);
			Image(const Image&) = delete;
			Image(Image&& other) noexcept;

			~Image();

			Image& operator=(const Image&) = delete;
			Image& operator = (Image&&) = delete;

			VkImage& GetVkImage() { return vkImage; }
			VkImageView& GetVkImageView() { return vkImageView; }

			static void TransitionToLayout(Image& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		private:
			VkImage vkImage{ VK_NULL_HANDLE };
			VkImageView vkImageView{ VK_NULL_HANDLE };
			VmaAllocation vmaAllocation{ VK_NULL_HANDLE };
			LogicalDevice& device;

			
		};
	}
}

#endif // !BAAL_VK_IMAGE_H