// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_LOGICAL_DEVICE_H
#define BAAL_LOGICAL_DEVICE_H

#include <vulkan/vulkan_core.h>
#include <memory>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class Instance;
		class PhysicalDevice;
		class Surface;
		class CommandPool;
		class CommandBuffer;
		class Allocator;
		class Buffer;
		class Image;

		// The interface that is used to interact with the vkPhysicalDevice
		
		class LogicalDevice
		{
		public:
			explicit LogicalDevice(Instance& instance, Surface& surface, const std::vector<const char*>& requiredExtensions = {});
			LogicalDevice(const LogicalDevice&) = delete;
			LogicalDevice(LogicalDevice&&) = delete;

			~LogicalDevice();

			LogicalDevice& operator=(const LogicalDevice&) = delete;
			LogicalDevice& operator = (LogicalDevice&&) = delete;

			VkDevice& GetVkDevice() { return device; }
			VkQueue& GetGraphicsQueue() { return graphicsQueue; };
			VkQueue& GetPresentQueue() { return presentQueue; };
			CommandPool& GetCommandPool() { return *commandPool.get(); }
			Allocator& GetAllocator() { return *allocator.get(); }

			uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

			CommandBuffer CreateCommandBuffer(bool bBeginCommand = true);
			void FlushCommandBuffer(CommandBuffer& commandBuffer, VkQueue queue);

			void CopyBuffer(Buffer& source, Buffer& destination, VkDeviceSize size);
			void CopyBufferToImage(Buffer& source, Image& destination, const uint32_t width, const uint32_t height);

		private:
			const PhysicalDevice& physicalDevice;
			VkDevice device{ VK_NULL_HANDLE };
			VkQueue graphicsQueue{ VK_NULL_HANDLE };
			VkQueue presentQueue{ VK_NULL_HANDLE };
			std::vector<const char*> enabledExtensions;
			std::unique_ptr<CommandPool> commandPool;
			std::unique_ptr<Allocator> allocator;

			void QueryAvailableExtensions(std::vector<VkExtensionProperties>& outExtensions) const;
			bool IsExtensionAvailable(const char* extensionName, const std::vector<VkExtensionProperties>& extensions) const;
		};
	}
}

#endif // BAAL_LOGICAL_DEVICE_H