// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_VK_RENDERER_H
#define BAAL_VK_RENDERER_H

#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

using namespace std;

class GLFWwindow;

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
			explicit Renderer(const std::string& appName, GLFWwindow* window);
			Renderer(const Renderer&) = delete;
			Renderer(Renderer&&) = delete;

			~Renderer();

			Renderer& operator=(const Renderer&) = delete;
			Renderer& operator = (Renderer&&) = delete;

		private:
			std::vector<const char*> GetRequiredGLFWExtenstions() const;

			std::unique_ptr<Instance> instance;
			std::unique_ptr<LogicalDevice> device;

			VkSurfaceKHR surface;

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