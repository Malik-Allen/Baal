// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_VK_RENDERER_H
#define BAAL_VK_RENDERER_H

#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace Baal
{
	namespace VK
	{
		class Instance;
		class LogicalDevice;
		class CommandPool;
		class CommandBuffer;

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
			void Render();
			void Shutdown();

			std::unique_ptr<Instance> instance;
			std::unique_ptr<LogicalDevice> device;
			std::unique_ptr<CommandPool> commandPool;

			std::vector<CommandBuffer> drawCommands;

		public:
			void Init();

		};
	}
}

#endif // BAAL_VK_RENDERER_H