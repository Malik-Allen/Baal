// MIT License, Copyright (c) 2024 Malik Allen

#include "Buffer.h"

#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		Buffer::Buffer(LogicalDevice& _device, VkBufferUsageFlags usage, VkDeviceSize size, std::vector<uint32_t> queueFamilyIndicies /*= {}*/):
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


		}

		Buffer::Buffer(Buffer&& other) noexcept :
			device(other.device)
		{
			vkBuffer = other.vkBuffer;

			other.vkBuffer = VK_NULL_HANDLE;
		}

		Buffer::~Buffer()
		{
			if (vkBuffer != VK_NULL_HANDLE)
			{
				vkDestroyBuffer(device.GetVkDevice(), vkBuffer, nullptr);
			}
		}
	}
}