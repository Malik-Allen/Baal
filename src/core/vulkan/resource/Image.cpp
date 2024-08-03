// MIT License, Copyright (c) 2024 Malik Allen

#include "Image.h"

#include "../src/core/vulkan/resource/Allocator.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		Image::Image(Allocator& _allocator, const uint32_t width, const uint32_t height, VkImageType type, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkSampleCountFlagBits samples, VkDeviceSize _size, void* data):
			allocator(_allocator)
		{
			VkImageCreateInfo imageInfo = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.extent.depth = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.mipLevels = 1;

			imageInfo.imageType = type;
			imageInfo.format = format;
			imageInfo.tiling = tiling;
			imageInfo.usage = usage;

			imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			imageInfo.samples = samples;

			VmaAllocationCreateInfo allocInfo = {};
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

			// If the image is an attachment, prefer dedicated memory
			constexpr VkImageUsageFlags attachmentOnlyFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
			if (imageInfo.usage & attachmentOnlyFlags)
			{
				allocInfo.flags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
			}

			if (imageInfo.usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)
			{
				allocInfo.preferredFlags |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
			}

			VK_CHECK(vmaCreateImage(allocator.GetVmaAllocator(), &imageInfo, &allocInfo, &vkImage, &vmaAllocation, nullptr), "vma allocating image memory");
		}

		Image::Image(Image&& other) noexcept:
			allocator(other.allocator)
		{
			vkImage = other.vkImage;
			vmaAllocation = other.vmaAllocation;

			other.vkImage = VK_NULL_HANDLE;
			other.vmaAllocation = VK_NULL_HANDLE;
		}

		Image::~Image()
		{
			if (vkImage != VK_NULL_HANDLE && vmaAllocation != VK_NULL_HANDLE)
			{
				vmaDestroyImage(allocator.GetVmaAllocator(), vkImage, vmaAllocation);
			}
		}
	}
}