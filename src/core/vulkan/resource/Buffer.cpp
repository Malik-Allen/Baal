// MIT License, Copyright (c) 2024 Malik Allen

#include "Buffer.h"

#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		Buffer::Buffer(LogicalDevice& _device, VkBufferUsageFlags usage, VkDeviceSize size, void* data, std::vector<uint32_t> queueFamilyIndicies /*= {}*/):
			device(_device)
		{
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

			VK_CHECK(vkCreateBuffer(device.GetVkDevice(), &bufferInfo, nullptr, &vkBuffer), "creating buffer");

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device.GetVkDevice(), vkBuffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = device.FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			VK_CHECK(vkAllocateMemory(device.GetVkDevice(), &allocInfo, nullptr, &vkBufferMemory), "allocating buffer memory");

			VK_CHECK(vkBindBufferMemory(device.GetVkDevice(), vkBuffer, vkBufferMemory, 0), "binding buffer memory");

			void* _data;
			vkMapMemory(device.GetVkDevice(), vkBufferMemory, 0, bufferInfo.size, 0, &_data);
			memcpy(_data, data, (size_t)bufferInfo.size);
			vkUnmapMemory(device.GetVkDevice(), vkBufferMemory);
		}

		Buffer::Buffer(Buffer&& other) noexcept :
			device(other.device)
		{
			vkBuffer = other.vkBuffer;
			vkBufferMemory = other.vkBufferMemory;

			other.vkBuffer = VK_NULL_HANDLE;
			other.vkBufferMemory = VK_NULL_HANDLE;
		}

		Buffer::~Buffer()
		{
			if (vkBuffer != VK_NULL_HANDLE)
			{
				vkDestroyBuffer(device.GetVkDevice(), vkBuffer, nullptr);
			}

			if (vkBufferMemory != VK_NULL_HANDLE)
			{
				vkFreeMemory(device.GetVkDevice(), vkBufferMemory, nullptr);
			}
		}
	}
}