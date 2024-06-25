// MIT License, Copyright (c) 2019 Malik Allen

#include "Renderer.h"

#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/commands/CommandPool.h"
#include "../src/core/vulkan/commands/CommandBuffer.h"

#include <stdexcept>

namespace Baal
{
	namespace VK
	{
		Renderer::Renderer(const string& appName) 
		{
			instance = std::make_unique<Instance>(appName);
			
			device = std::make_unique<LogicalDevice>(instance->GetGPU());

			commandPool = std::make_unique<CommandPool>(*device.get(), instance->GetGPU().GetQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT));
		}

		Renderer::~Renderer() 
		{
			drawCommands.clear();

			commandPool.reset();
			device.reset();
			instance.reset();
		}

		void Renderer::Init()
		{
			drawCommands.reserve(3);

			VkResult result;
			result = commandPool->CreateCommandBuffers(3, VK_COMMAND_BUFFER_LEVEL_PRIMARY, drawCommands);
			if (result != VK_SUCCESS) 
			{
				throw std::runtime_error("Failed to create draw commands!");
			}
		}

		void Renderer::Render()
		{}

		void Renderer::Shutdown()
		{}
	}
}