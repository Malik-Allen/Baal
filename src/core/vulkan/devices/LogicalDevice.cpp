// MIT License, Copyright (c) 2024 Malik Allen

#include "LogicalDevice.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"
#include "../src/core/vulkan/presentation/Surface.h"
#include "../src/core/vulkan/commands/CommandPool.h"
#include "../src/core/vulkan/commands/CommandBuffer.h"
#include "../src/core/vulkan/resource/Allocator.h"
#include "../src/core/vulkan/resource/Buffer.h"

namespace Baal
{
	namespace VK
	{
		LogicalDevice::LogicalDevice(Instance& instance, Surface& surface, const std::vector<const char*>& requiredExtensions):
			physicalDevice(instance.GetGPU())
		{
			std::vector<VkDeviceQueueCreateInfo> deviceQueueInfos;

			const uint32_t queueFamilyCount = physicalDevice.GetQueueFamilyProperties().size();
			std::vector<std::vector<float>> queueFamilyPriorities(queueFamilyCount);

			VkDeviceQueueCreateInfo queueInfo = {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};

			for(uint32_t i = 0; i < physicalDevice.GetQueueFamilyProperties().size(); ++i)
			{
				const VkQueueFamilyProperties& queueFamilyProperty = physicalDevice.GetQueueFamilyProperties()[i];

				// Question: Why is the graphics queue family given the highest priority?
				// Answer: Is up to you to decide if the GPU priortizes graphics queue, we could assign all queues to just have the priority as well

				queueFamilyPriorities[i].resize(queueFamilyProperty.queueCount, 0.5f);	// Setting all queue families with the same priority
				

				queueInfo.queueFamilyIndex = i;
				queueInfo.queueCount = queueFamilyProperty.queueCount;		
				queueInfo.pQueuePriorities = queueFamilyPriorities[i].data();
				deviceQueueInfos.push_back(queueInfo);
			}


			VkDeviceCreateInfo deviceInfo = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
			deviceInfo.queueCreateInfoCount = deviceQueueInfos.size();
			deviceInfo.pQueueCreateInfos = deviceQueueInfos.data();

			std::vector<VkExtensionProperties> availableExts;
			QueryAvailableExtensions(availableExts);

			std::vector<const char*> requestedExts(requiredExtensions);

			std::vector<const char*> enabledExts;
			for (auto requiredExt : requestedExts)
			{
				if (IsExtensionAvailable(requiredExt, availableExts))
				{
					enabledExts.push_back(requiredExt);
					DEBUG_LOG(LOG::INFO, "Enabled Ext: {}", requiredExt);
				}
				else
				{
					DEBUG_LOG(LOG::WARNING, "{} is NOT avaialble!", requiredExt);
				}
			}

			deviceInfo.enabledExtensionCount = enabledExts.size();
			deviceInfo.ppEnabledExtensionNames = enabledExts.data();

			VK_CHECK(vkCreateDevice(physicalDevice.GetVkPhysicalDevice(), &deviceInfo, nullptr, &device), "creating device");

			vkGetDeviceQueue(device, physicalDevice.GetQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT), 0, &graphicsQueue);
			
			for (uint32_t i = 0; i < queueFamilyCount; ++i) 
			{
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice.GetVkPhysicalDevice(), i, surface.GetVkSurface(), &presentSupport);

				if (presentSupport) {
					vkGetDeviceQueue(device, i, 0, &presentQueue);
					break;
				}
			}

			commandPool = std::make_unique<CommandPool>(*this, physicalDevice.GetQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT));
			allocator = std::make_unique<Allocator>(instance, *this);
		}

		LogicalDevice::~LogicalDevice()
		{
			commandPool.reset();
			allocator.reset();
			vkDestroyDevice(device, nullptr);
		}

		uint32_t LogicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
		{
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice.GetVkPhysicalDevice(), &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			DEBUG_LOG(LOG::ERRORLOG, "Failed to find memory type!");
			throw std::runtime_error("Failed to find memory type");
		}

		CommandBuffer LogicalDevice::CreateCommandBuffer()
		{
			CommandBuffer outCommandBuffer(*commandPool.get(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
			return outCommandBuffer;
		}

		void LogicalDevice::FlushCommandBuffer(CommandBuffer& commandBuffer, VkQueue queue)
		{
			commandBuffer.EndRecording();

			VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer.GetVkCommandBuffer();

			VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, nullptr), "flushing command buffer");
			VK_CHECK(vkQueueWaitIdle(queue), "waiting queue idle");
		}

		void LogicalDevice::CopyBuffer(Buffer& source, Buffer& destination, VkDeviceSize size)
		{
			CommandBuffer commandBuffer(CreateCommandBuffer());
			
			commandBuffer.BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

			VkBufferCopy copyRegion{};
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer.GetVkCommandBuffer(), source.GetVkBuffer(), destination.GetVkBuffer(), 1, &copyRegion);

			FlushCommandBuffer(commandBuffer, GetGraphicsQueue());
		}

		void LogicalDevice::QueryAvailableExtensions(std::vector<VkExtensionProperties>& outExtensions) const
		{
			uint32_t extCount;
			vkEnumerateDeviceExtensionProperties(physicalDevice.GetVkPhysicalDevice(), nullptr, &extCount, nullptr);
			outExtensions.resize(extCount);
			vkEnumerateDeviceExtensionProperties(physicalDevice.GetVkPhysicalDevice(), nullptr, &extCount, outExtensions.data());

			DEBUG_LOG(LOG::INFO, "Available Device Extension Count: {}", extCount);

			for (const VkExtensionProperties& ext : outExtensions)
			{
				DEBUG_LOG(LOG::INFO, "Ext: {}", ext.extensionName);
			}
		}

		bool LogicalDevice::IsExtensionAvailable(const char* extensionName, const std::vector<VkExtensionProperties>& extensions) const
		{
			for (auto& availableExtension : extensions)
			{
				if (std::strcmp(availableExtension.extensionName, extensionName) == 0)
				{
					return true;
				}
			}
			return false;
		}
	}
}