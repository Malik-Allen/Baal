// MIT License, Copyright (c) 2024 Malik Allen

#include "Buffer.h"

#include "../src/core/vulkan/resource/Allocator.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		//Buffer::Buffer(Allocator& _allocator, VkBufferUsageFlags usage, VkDeviceSize size, void* data, std::vector<uint32_t> queueFamilyIndicies /*= {}*/):
		//	allocator(_allocator)
		//{
		//	VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		//	bufferInfo.usage = usage;
		//	bufferInfo.size = size;

		//	if (queueFamilyIndicies.size() > 1)
		//	{
		//		bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		//	}
		//	else 
		//	{
		//		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//	}

		//	bufferInfo.queueFamilyIndexCount = queueFamilyIndicies.size();
		//	bufferInfo.pQueueFamilyIndices = queueFamilyIndicies.data();

		//	VmaAllocationCreateInfo allocInfo = {};
		//	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

		//	VK_CHECK(vmaCreateBuffer(allocator.GetVmaAllocator(), &bufferInfo, &allocInfo, &vkBuffer, &vmaAllocation, nullptr), "allocationg buffer memory");
		//}

		Buffer::Buffer(Allocator& _allocator, VkBufferUsageFlags usage, VkDeviceSize size, void* data, std::vector<uint32_t> queueFamilyIndicies):
			allocator(_allocator)
		{
		}

		Buffer::Buffer(Buffer&& other) noexcept :
			allocator(other.allocator)
		{
			vkBuffer = other.vkBuffer;
			vmaAllocation = other.vmaAllocation;

			other.vkBuffer = VK_NULL_HANDLE;
			other.vmaAllocation = VK_NULL_HANDLE;
		}

		Buffer::~Buffer()
		{
			if (vkBuffer != VK_NULL_HANDLE)
			{
				// vmaDestroyBuffer(allocator.GetVmaAllocator(), vkBuffer, vmaAllocation);
			}
		}
	}
}