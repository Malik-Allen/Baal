// MIT License, Copyright (c) 2024 Malik Allen

#include "Image.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/resource/Allocator.h"
#include "../src/core/vulkan/resource/Buffer.h"
#include "../src/core/vulkan/commands/CommandBuffer.h"

namespace Baal
{
	namespace VK
	{
		Image::Image(
			LogicalDevice& _device, 
			const uint32_t width, 
			const uint32_t height, 
			VkImageType type, 
			VkFormat format, 
			VkImageTiling tiling, 
			VkImageUsageFlags usage, 
			VkSampleCountFlagBits samples,
			VkImageViewType viewType,
			VkImageSubresourceRange subresourceRange):
			device(_device)
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

			VK_CHECK(vmaCreateImage(device.GetAllocator().GetVmaAllocator(), &imageInfo, &allocInfo, &vkImage, &vmaAllocation, nullptr), "vma allocating image memory");

			VkImageViewCreateInfo viewInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
			viewInfo.image = vkImage;
			viewInfo.viewType = viewType;
			viewInfo.format = format;
			viewInfo.subresourceRange = subresourceRange;

			VK_CHECK(vkCreateImageView(device.GetVkDevice(), &viewInfo, nullptr, &vkImageView), "creating image view");
		}

		Image::Image(Image&& other) noexcept:
			device(other.device)
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
				vkDestroyImageView(device.GetVkDevice(), vkImageView, nullptr);
				vmaDestroyImage(device.GetAllocator().GetVmaAllocator(), vkImage, vmaAllocation);
			}
		}

		void Image::TransitionToLayout(
			Image& image,
			VkFormat format,
			VkImageLayout oldLayout,
			VkImageLayout newLayout,
			VkPipelineStageFlags srcStage,
			VkPipelineStageFlags dstStage,
			VkAccessFlags srcAccessMask,
			VkAccessFlags dstAccessMask,
			VkImageSubresourceRange subresouceRange)
		{
			CommandBuffer commandBuffer(image.device.CreateCommandBuffer());

			VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
			barrier.oldLayout = oldLayout;
			barrier.newLayout = newLayout;

			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = image.GetVkImage();
			barrier.subresourceRange = subresouceRange;

			VkPipelineStageFlags sourceStage = srcStage;
			VkPipelineStageFlags destinationStage = dstStage;

			barrier.srcAccessMask = srcAccessMask;
			barrier.dstAccessMask = dstAccessMask;

			vkCmdPipelineBarrier(commandBuffer.GetVkCommandBuffer(), sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			image.device.FlushCommandBuffer(commandBuffer, image.device.GetGraphicsQueue());
		}
	}
}