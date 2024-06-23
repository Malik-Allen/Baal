// MIT License, Copyright (c) 2023 Malik Allen

#include "Renderer.h"

#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/commands/CommandPool.h"

namespace Baal
{
	namespace VK
	{
		Renderer::Renderer(const string& appName) 
		{
			instance = std::make_unique<Instance>(appName);
			
			device = std::make_unique<LogicalDevice>(instance->GetGPU());

			
		}

		Renderer::~Renderer() 
		{
			device.reset();
			instance.reset();
		}

		void Renderer::Init()
		{
			
		}

		void Renderer::Render()
		{}

		void Renderer::Shutdown()
		{}
	}
}