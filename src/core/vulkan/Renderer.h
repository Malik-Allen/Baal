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
		class Framebuffer;
		class RenderPass;
		class Allocator;
		class Buffer;
		class MeshHandler;
		class Mesh;
		class MeshInstance;
		class Camera;
		class RenderCameraResources;

		class Renderer
		{
		public:
			explicit Renderer(const std::string& appName, GLFWwindow* _window);
			Renderer(const Renderer&) = delete;
			Renderer(Renderer&&) = delete;

			virtual ~Renderer();

			Renderer& operator=(const Renderer&) = delete;
			Renderer& operator = (Renderer&&) = delete;

		private:
			std::vector<const char*> GetRequiredInstanceExtenstions() const;
			std::vector<const char*> GetRequiredGLFWExtenstions() const;

			std::vector<const char*> GetRequiredDeviceExtenstions() const;

			void CreateSwapChainImageViews();
			void DestroySwapChainImageViews();

			void CreateRenderPass();

			void CreateFramebuffers();
			void DestroyFramebuffers();

			void CreateDrawCommandBuffers();
			void DestroyDrawCommandBuffers();

			void CreateSyncObjects();
			void DestroySyncObjects();

			void RecreateSwapChain();
			void CreateSwapChain();
			void DestroySwapChain();

			void CreateDefaultCamera();
			void DestroyDefaultCamera();
			void UpdateDefaultCamera();

			void UpdateMeshHandler();
			void CleanUpMeshHandler();

			std::unique_ptr<Instance> instance;
			std::unique_ptr<LogicalDevice> device;

			GLFWwindow* window;
			std::unique_ptr<Surface> surface;
			std::unique_ptr<SwapChain> swapChain;
			std::vector<VkImageView> swapChainImageViews;

			std::vector<CommandBuffer> drawCommands;
			
			std::unique_ptr<RenderPass> renderPass;
			std::vector<Framebuffer> framebuffers;

			VkSemaphore acquiredImageReady;
			VkSemaphore renderComplete;
			VkFence waitFence;
			uint32_t currentBuffer = 0;
			
			std::unique_ptr<MeshHandler> meshHandler;
			std::unique_ptr<RenderCameraResources> cameraResources;

		protected:
			virtual void Initialize() = 0;
			virtual void Destroy() = 0;
			virtual void RecordDrawCommandBuffer(CommandBuffer& drawCommand, Framebuffer& frameBuffer) = 0;
			virtual void PreRender() = 0;
			virtual void PostRender() = 0;

			Instance& GetInstance();
			LogicalDevice& GetDevice();
			Surface& GetSurface();
			SwapChain& GetSwapChain();
			CommandPool& GetCommandPool();
			RenderPass& GetRenderPass();
			Allocator& GetAllocator();

			MeshHandler& GetMeshHandler();

			Camera& GetCamera();
			Buffer& GetCameraUniformBuffer();

		public:
			void Startup();
			void Render();
			void Shutdown();

			std::weak_ptr<Mesh> LoadMeshResource(const char* parentDirectory, const char* meshFileName);
			std::weak_ptr<MeshInstance> AddMeshInstanceToScene(std::weak_ptr<Mesh> resource);
		};
	}
}

#endif // BAAL_VK_RENDERER_H