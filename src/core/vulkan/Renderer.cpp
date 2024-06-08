// MIT License, Copyright (c) 2019 Malik Allen

#include "Renderer.h"

#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"

namespace Baal
{
	namespace VK
	{
		Renderer::Renderer(const string& appName) 
		{
			instance = make_unique<Instance>(appName);
			
			device = make_unique<LogicalDevice>(instance->GetGPU());
		}

		Renderer::~Renderer() 
		{}

		void Renderer::Init()
		{
			
		}

		void Renderer::Render()
		{}

		void Renderer::Shutdown()
		{}
	}
}