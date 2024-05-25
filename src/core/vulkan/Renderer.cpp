// MIT License, Copyright (c) 2019 Malik Allen

#include "Renderer.h"

namespace Baal
{
	namespace VK
	{
		Renderer::Renderer(const string& appName) 
		{
			instance = make_unique<Instance>(appName);
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