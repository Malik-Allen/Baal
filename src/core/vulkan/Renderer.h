// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_VK_RENDERER_H
#define BAAL_VK_RENDERER_H

#include "devices/Instance.h"

using namespace std;

namespace Baal
{
	namespace VK
	{
		class Renderer
		{
		public:
			explicit Renderer(const string& appName);
			Renderer(const Renderer&) = delete;
			Renderer(Renderer&&) = delete;

			~Renderer();

			Renderer& operator=(const Renderer&) = delete;
			Renderer& operator = (Renderer&&) = delete;

		private:
			void Init();
			void Render();
			void Shutdown();

			unique_ptr<Instance> instance;
		};
	}
}

#endif // BAAL_VK_RENDERER_H