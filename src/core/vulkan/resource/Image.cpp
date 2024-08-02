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
			size = _size;

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
			constexpr VkImageUsageFlags attachment_only_flags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
			if (imageInfo.usage & attachment_only_flags)
			{
				allocInfo.flags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
			}

			if (imageInfo.usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)
			{
				allocInfo.preferredFlags |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
			}

			VK_CHECK(vmaCreateImage(allocator.GetVmaAllocator(), &imageInfo, &allocInfo, &vkImage, &vmaAllocation, nullptr), "allocationg image memory");

			if (data != nullptr) 
			{
				VK_CHECK(vmaMapMemory(allocator.GetVmaAllocator(), vmaAllocation, reinterpret_cast<void**>(&mappedData)), "vma mapping image memory");
				memcpy(mappedData, data, (size_t)size);
				bIsMapped = true;
			}
		}

		Image::Image(Image&& other) noexcept:
			allocator(other.allocator),
			size(other.size)
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
				vmaUnmapMemory(allocator.GetVmaAllocator(), vmaAllocation);
				vmaDestroyImage(allocator.GetVmaAllocator(), vkImage, vmaAllocation);
				bIsMapped = false;
			}
		}

		void Image::Update(void* data, const size_t _size)
		{
			Map();
			size = _size;
			memcpy(mappedData, data, size);
		}

		void Image::Map()
		{
			if (!bIsMapped)
			{
				VK_CHECK(vmaMapMemory(allocator.GetVmaAllocator(), vmaAllocation, reinterpret_cast<void**>(&mappedData)), "vma mapping image memory");
				bIsMapped = true;
			}
		}
	}
}