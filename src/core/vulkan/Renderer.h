// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_RENDERER_H
#define BAAL_VK_RENDERER_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

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
		class Allocator;
		class DescriptorPool;
		class DescriptorSetLayout;
		class DescriptorSet;
		class MeshManager;
		class Mesh;
		class MeshInstance;

		class Camera;
		class Buffer;

		class Renderer
		{
		public:
			explicit Renderer(const std::string& appName, GLFWwindow* _window);
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

			void CreateDrawCommandBuffers();
			void DestroyDrawCommandBuffers();

			void RecordDrawCommandBuffer(CommandBuffer& commandBuffer);

			void CreateSyncObjects();
			void DestroySyncObjects();

			void RecreateSwapChain();
			void CreateSwapChain();
			void DestroySwapChain();

			void CreateDescriptorPool();
			void DestroyDescriptorPool();

			void CreateDescriptorSetLayout();
			void DestroyDescriptorSetLayout();

			void CreateDescriptorSet();

			void CreateDefaultCamera();

			std::unique_ptr<Instance> instance;
			std::unique_ptr<LogicalDevice> device;

			GLFWwindow* window;
			std::unique_ptr<Surface> surface;
			std::unique_ptr<SwapChain> swapChain;
			std::vector<VkImageView> swapChainImageViews;

			std::unique_ptr<CommandPool> commandPool;
			std::vector<CommandBuffer> drawCommands;

			std::unique_ptr<RenderPass> renderPass;
			std::unique_ptr<GraphicsPipeline> forwardPipeline;
			std::vector<Framebuffer> framebuffers;

			VkSemaphore acquiredImageReady;
			VkSemaphore renderComplete;
			VkFence waitFence;

			uint32_t currentBuffer = 0;

			std::unique_ptr<Allocator> allocator;
			std::unique_ptr<DescriptorPool> descriptorPool;
			std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
			std::unique_ptr<DescriptorSet> descriptorSet;

			std::unique_ptr<MeshManager> meshManager;

			std::unique_ptr<Camera> camera;
			std::unique_ptr<Buffer> cameraUniformBuffer;

		public:
			void Init();
			void RenderFrame();
			void Shutdown();

			std::shared_ptr<Mesh> LoadMeshResource(const char* parentDirectory, const char* meshFileName);
			std::shared_ptr<MeshInstance> AddMeshInstanceToScene(Mesh& resource);
		};
	}
}

#endif // BAAL_VK_RENDERER_H