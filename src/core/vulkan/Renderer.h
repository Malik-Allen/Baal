// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_RENDERER_H
#define BAAL_VK_RENDERER_H

#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>

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
		class GraphicsPipeline;
		class RenderPass;
		class Framebuffer;

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

			void CreateSwapChainImageViews();
			void DestroySwapChainImageViews();

			void CreatePipelines();
			void DestroyPipelines();

			void CreateForwardPipeline();

			void CreateFramebuffers();
			void DestroyFramebuffers();

			std::unique_ptr<Instance> instance;
			std::unique_ptr<LogicalDevice> device;

			std::unique_ptr<Surface> surface;
			std::unique_ptr<SwapChain> swapChain;
			std::vector<VkImageView> swapChainImageViews;

			std::unique_ptr<CommandPool> commandPool;
			std::vector<CommandBuffer> drawCommands;

			std::unique_ptr<RenderPass> renderPass;
			std::unique_ptr<GraphicsPipeline> forwardPipeline;
			std::vector<Framebuffer> framebuffers;

		public:
			void Init();
			void RenderFrame();
			void Shutdown();
		};
	}
}

#endif // BAAL_VK_RENDERER_H