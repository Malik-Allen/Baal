// MIT License, Copyright (c) 2019 Malik Allen

#include "Renderer.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/commands/CommandPool.h"
#include "../src/core/vulkan/commands/CommandBuffer.h"


#include <stdexcept>
#include <GLFW/glfw3.h>

namespace Baal
{
	namespace VK
	{
		Renderer::Renderer(const std::string& appName, GLFWwindow* window)
		{
			std::vector<const char*> extensions = GetRequiredGLFWExtenstions();

			instance = std::make_unique<Instance>(appName, true, extensions);

			VK_CHECK(glfwCreateWindowSurface(instance.get()->GetVkInstance(), window, nullptr, &surface), "creating window surface");

			device = std::make_unique<LogicalDevice>(instance->GetGPU());

			commandPool = std::make_unique<CommandPool>(*device.get(), instance->GetGPU().GetQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT));
		}

		Renderer::~Renderer() 
		{
			drawCommands.clear();

			commandPool.reset();
			device.reset();

			vkDestroySurfaceKHR(instance.get()->GetVkInstance(), surface, nullptr);

			instance.reset();
		}

		void Renderer::Init()
		{
			drawCommands.reserve(3);
			VK_CHECK(commandPool->CreateCommandBuffers(3, VK_COMMAND_BUFFER_LEVEL_PRIMARY, drawCommands), "creating draw commands");
		}

		void Renderer::RenderFrame()
		{}

		void Renderer::Shutdown()
		{}

		std::vector<const char*> Renderer::GetRequiredGLFWExtenstions() const
		{
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
			return extensions;
		}
	}
}