// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_RENDERER_H
#define BAAL_VK_RENDERER_H

#include <memory>
#include <string>
#include <vector>

class GLFWwindow;

namespace Baal
{
	namespace VK
	{
		class Instance;
		class LogicalDevice;
		class Surface;
		class SwapChain;
		class CommandPool;
		class CommandBuffer;

		class Renderer
		{
		public:
			explicit Renderer(const std::string& appName, GLFWwindow* window);
			Renderer(const Renderer&) = delete;
			Renderer(Renderer&&) = delete;

			~Renderer();

			Renderer& operator=(const Renderer&) = delete;
			Renderer& operator = (Renderer&&) = delete;

		private:
			std::vector<const char*> GetRequiredInstanceExtenstions() const;
			std::vector<const char*> GetRequiredGLFWExtenstions() const;

			std::vector<const char*> GetRequiredDeviceExtenstions() const;

			std::unique_ptr<Instance> instance;
			std::unique_ptr<LogicalDevice> device;

			std::unique_ptr<Surface> surface;
			std::unique_ptr<SwapChain> swapChain;

			std::unique_ptr<CommandPool> commandPool;
			std::vector<CommandBuffer> drawCommands;

		public:
			void Init();
			void RenderFrame();
			void Shutdown();
		};
	}
}

#endif // BAAL_VK_RENDERER_H