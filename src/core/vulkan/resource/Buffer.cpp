// MIT License, Copyright (c) 2024 Malik Allen

#include "Buffer.h"

#include "../src/core/vulkan/resource/Allocator.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		Buffer::Buffer(Allocator& _allocator, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkDeviceSize _size, void* data, std::vector<uint32_t> queueFamilyIndicies /*= {}*/):
			allocator(_allocator),
			size(_size)
		{
			bIsMapped = false;

			VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
			bufferInfo.usage = usage;
			bufferInfo.size = size;

			if (queueFamilyIndicies.size() > 1)
			{
				bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
			}
			else 
			{
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			}

			bufferInfo.queueFamilyIndexCount = queueFamilyIndicies.size();
			bufferInfo.pQueueFamilyIndices = queueFamilyIndicies.data();

			VmaAllocationCreateInfo allocInfo = {};
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
			allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
			allocInfo.requiredFlags = memoryProperties;

			VK_CHECK(vmaCreateBuffer(allocator.GetVmaAllocator(), &bufferInfo, &allocInfo, &vkBuffer, &vmaAllocation, nullptr), "vma allocating buffer memory");

			if (data != nullptr) 
			{
				VK_CHECK(vmaMapMemory(allocator.GetVmaAllocator(), vmaAllocation, reinterpret_cast<void**>(&mappedData)), "vma mapping buffer memory");
				memcpy(mappedData, data, (size_t)bufferInfo.size);
				bIsMapped = true;
			}
		}

		Buffer::Buffer(Buffer&& other) noexcept :
			allocator(other.allocator),
			size(other.size)
		{
			vkBuffer = other.vkBuffer;
			vmaAllocation = other.vmaAllocation;

			other.vkBuffer = VK_NULL_HANDLE;
			other.vmaAllocation = VK_NULL_HANDLE;
		}

		Buffer::~Buffer()
		{
			if (vkBuffer != VK_NULL_HANDLE && vmaAllocation != VK_NULL_HANDLE)
			{
				if(bIsMapped)
				{
					vmaUnmapMemory(allocator.GetVmaAllocator(), vmaAllocation);
				}
				vmaDestroyBuffer(allocator.GetVmaAllocator(), vkBuffer, vmaAllocation);
				bIsMapped = false;
			}
		}

		void Buffer::Update(void* data, const size_t _size)
		{
			Map();
			size = _size;
			memcpy(mappedData, data, size);
		}

		Buffer Buffer::CreateStagingBuffer(Allocator& allocator, VkDeviceSize _size, void* data)
		{
			Buffer outBuffer(allocator, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _size, data);
			return outBuffer;
		}

		void Buffer::Map()
		{
			if (!bIsMapped)
			{
				VK_CHECK(vmaMapMemory(allocator.GetVmaAllocator(), vmaAllocation, reinterpret_cast<void**>(&mappedData)), "vma mapping buffer memory");
				bIsMapped = true;
			}
		}
	}
}