// MIT License, Copyright (c) 2024 Malik Allen

#include "Renderer.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/presentation/Surface.h"
#include "../src/core/vulkan/presentation/SwapChain.h"
#include "../src/core/vulkan/commands/CommandPool.h"
#include "../src/core/vulkan/commands/CommandBuffer.h"
#include "../src/core/vulkan/pipeline/RenderPass.h"
#include "../src/core/vulkan/pipeline/Framebuffer.h"
#include "../src/core/vulkan/resource/Buffer.h"
#include "../src/core/vulkan/resource/Allocator.h"
#include "../src/core/3d/MeshHandler.h"
#include "../src/core/3d/Mesh.h"

#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include <GLFW/glfw3.h>

namespace Baal
{
	namespace VK
	{
		Renderer::Renderer(const std::string& appName, GLFWwindow* _window)
		{
			window = _window;

			const std::vector<const char*> instanceExtensions = GetRequiredInstanceExtenstions();
			const std::vector<const char*> deviceExtensions = GetRequiredDeviceExtenstions();

			instance = std::make_unique<Instance>(appName, true, instanceExtensions);

			surface = std::make_unique<Surface>(*instance.get(), window);

			device = std::make_unique<LogicalDevice>(*instance.get(), *surface.get(), deviceExtensions);

			CreateSwapChain();

			meshHandler = std::make_unique<MeshHandler>();
		}

		Renderer::~Renderer() 
		{
			
		}

		Instance& Renderer::GetInstance()
		{
			return *instance.get();
		}

		LogicalDevice& Renderer::GetDevice()
		{
			return *device.get();
		}

		Surface& Renderer::GetSurface()
		{
			return *surface.get();
		}

		SwapChain& Renderer::GetSwapChain()
		{
			return *swapChain.get();
		}

		CommandPool& Renderer::GetCommandPool()
		{
			return device->GetCommandPool();
		}

		RenderPass& Renderer::GetRenderPass()
		{
			return *renderPass.get();
		}

		Allocator& Renderer::GetAllocator()
		{
			return device->GetAllocator();
		}

		MeshHandler& Renderer::GetMeshHandler()
		{
			return *meshHandler.get();
		}

		void Renderer::Startup()
		{
			CreateSwapChainImageViews();
			CreateRenderPass();
			CreateFramebuffers();
			CreateDrawCommandBuffers();
			CreateSyncObjects();
			Initialize();
		}

		void Renderer::Render()
		{
			PreRender();

			vkWaitForFences(device->GetVkDevice(), 1, &waitFence, VK_TRUE, UINT64_MAX);

			VkResult result = vkAcquireNextImageKHR(device->GetVkDevice(), swapChain->GetVkSwapChain(), UINT64_MAX, acquiredImageReady, VK_NULL_HANDLE, &currentBuffer);
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				RecreateSwapChain();
				return;
			}

			vkResetFences(device->GetVkDevice(), 1, &waitFence);

			if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				std::string output = std::string(string_VkResult(result));
				DEBUG_LOG(LOG::ERRORLOG, "Failed to acquire next image to present! Error: {}", output);
				assert(false);
			}

			RecordDrawCommandBuffer(drawCommands[currentBuffer], framebuffers[currentBuffer]);

			VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &drawCommands[currentBuffer].GetVkCommandBuffer();

			std::vector<VkSemaphore> waitSemaphores = { acquiredImageReady };
			std::vector<VkPipelineStageFlags> waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = waitSemaphores.size();
			submitInfo.pWaitSemaphores = waitSemaphores.data();
			submitInfo.pWaitDstStageMask = waitStages.data();

			std::vector<VkSemaphore> signalSemaphores = { renderComplete };
			submitInfo.signalSemaphoreCount = signalSemaphores.size();
			submitInfo.pSignalSemaphores = signalSemaphores.data();

			VK_CHECK(vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, waitFence), "submitting graphics queue");

			VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &swapChain->GetVkSwapChain();
			presentInfo.waitSemaphoreCount = signalSemaphores.size();
			presentInfo.pWaitSemaphores = signalSemaphores.data();
			presentInfo.pImageIndices = &currentBuffer;

			result = vkQueuePresentKHR(device->GetPresentQueue(), &presentInfo);
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				RecreateSwapChain();
				return;
			}

			if (result != VK_SUCCESS)
			{
				std::string output = std::string(string_VkResult(result));
				DEBUG_LOG(LOG::ERRORLOG, "Failed to present to present queue! Error: {}", output);
				assert(false);
			}

			PostRender();
		}

		void Renderer::Shutdown()
		{
			vkDeviceWaitIdle(device->GetVkDevice());
			Destroy();
			DestroyDrawCommandBuffers();
			DestroyFramebuffers();
			renderPass.reset();
			meshHandler.reset();
			DestroySwapChainImageViews();
			DestroySwapChain();
			DestroySyncObjects();
			device.reset();
			surface.reset();
			instance.reset();
		}

		std::shared_ptr<Mesh> Renderer::LoadMeshResource(const char* parentDirectory, const char* meshFileName)
		{
			return meshHandler->LoadMeshResource(parentDirectory, meshFileName);
		}

		std::shared_ptr<MeshInstance> Renderer::AddMeshInstanceToScene(Mesh& resource)
		{
			return meshHandler->CreateMeshInstance(GetDevice(), resource);
		}

		std::vector<const char*> Renderer::GetRequiredInstanceExtenstions() const
		{
			const std::vector<const char*> glfwExtensions = GetRequiredGLFWExtenstions();
			const std::vector<const char*> miscExtensions = {};

			std::vector<const char*> requiredExtensions;
			requiredExtensions.reserve(glfwExtensions.size() + miscExtensions.size());

			requiredExtensions.insert(requiredExtensions.end(), glfwExtensions.begin(), glfwExtensions.end());
			requiredExtensions.insert(requiredExtensions.end(), miscExtensions.begin(), miscExtensions.end());
			
			return requiredExtensions;
		}

		std::vector<const char*> Renderer::GetRequiredGLFWExtenstions() const
		{
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
			return extensions;
		}

		std::vector<const char*> Renderer::GetRequiredDeviceExtenstions() const
		{
			const std::vector<const char*> miscExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
			return miscExtensions;
		}

		void Renderer::CreateSwapChainImageViews()
		{
			DestroySwapChainImageViews();

			const std::vector<VkImage>& swapChainImages = swapChain->GetImages();

			swapChainImageViews.resize(swapChainImages.size());

			VkImageViewCreateInfo imageViewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
			for (size_t i = 0; i < swapChainImages.size(); ++i)
			{
				imageViewInfo.image = swapChainImages[i];
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewInfo.format = swapChain->GetSurfaceFormat().format;

				imageViewInfo.components = {
					VK_COMPONENT_SWIZZLE_R,
					VK_COMPONENT_SWIZZLE_G,
					VK_COMPONENT_SWIZZLE_B,
					VK_COMPONENT_SWIZZLE_A 
				};

				imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageViewInfo.subresourceRange.baseMipLevel = 0;
				imageViewInfo.subresourceRange.levelCount = 1;
				imageViewInfo.subresourceRange.baseArrayLayer = 0;
				imageViewInfo.subresourceRange.layerCount = 1;

				VK_CHECK(vkCreateImageView(device->GetVkDevice(), &imageViewInfo, nullptr, &swapChainImageViews[i]), "creating swap chain image view");
			}
		}

		void Renderer::DestroySwapChainImageViews()
		{
			for (size_t i = 0; i < swapChainImageViews.size(); ++i)
			{
				vkDestroyImageView(device->GetVkDevice(), swapChainImageViews[i], nullptr);
			}

			swapChainImageViews.clear();
		}

		void Renderer::CreateRenderPass()
		{
			std::vector<Attachment> attachments;

			Attachment colorAttachment;
			colorAttachment.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			colorAttachment.description.flags = 0;
			colorAttachment.description.format = swapChain->GetSurfaceFormat().format;
			colorAttachment.description.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			attachments.push_back(colorAttachment);

			renderPass = std::make_unique<RenderPass>(*device.get(), attachments);
		}

		void Renderer::CreateFramebuffers()
		{
			std::vector<VkImageView> attachments(1);

			for (size_t i = 0; i < swapChainImageViews.size(); ++i) 
			{
				attachments[0] = swapChainImageViews[i];
				framebuffers.push_back(Framebuffer(*device.get(), *renderPass.get(), attachments, swapChain->GetExtent().width, swapChain->GetExtent().height));
			}
		}

		void Renderer::DestroyFramebuffers()
		{
			framebuffers.clear();
		}

		void Renderer::CreateDrawCommandBuffers()
		{
			const uint32_t framebufferCount = framebuffers.size();
			drawCommands.reserve(framebufferCount);
			VK_CHECK(GetCommandPool().CreateCommandBuffers(framebufferCount, VK_COMMAND_BUFFER_LEVEL_PRIMARY, drawCommands), "creating draw commands");
			currentBuffer = 0;
		}

		void Renderer::DestroyDrawCommandBuffers()
		{
			drawCommands.clear();
		}

		void Renderer::CreateSyncObjects()
		{
			VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
			semaphoreInfo.flags = 0;
			VK_CHECK(vkCreateSemaphore(device->GetVkDevice(), &semaphoreInfo, nullptr, &acquiredImageReady), "creating semaphore for acquired image ready");
			VK_CHECK(vkCreateSemaphore(device->GetVkDevice(), &semaphoreInfo, nullptr, &renderComplete), "creating semaphore for render complete");

			VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			VK_CHECK(vkCreateFence(device->GetVkDevice(), &fenceInfo, nullptr, &waitFence), "creating wait fence");
		}

		void Renderer::DestroySyncObjects()
		{
			vkDestroySemaphore(device->GetVkDevice(), acquiredImageReady, nullptr);
			vkDestroySemaphore(device->GetVkDevice(), renderComplete, nullptr);
			vkDestroyFence(device->GetVkDevice(), waitFence, nullptr);
		}

		void Renderer::RecreateSwapChain()
		{
			vkDeviceWaitIdle(device->GetVkDevice());

			DestroyFramebuffers();
			DestroySwapChainImageViews();
			DestroySwapChain();

			CreateSwapChain();
			CreateSwapChainImageViews();
			CreateFramebuffers();
		}

		void Renderer::CreateSwapChain()
		{
			if (window == nullptr)
			{
				DEBUG_LOG(LOG::ERRORLOG, "Failed to create SwapChain! GLFWwindow is nullptr!");
				throw std::runtime_error("Failed to create SwapChain! GLFWwindow is nullptr!");
			}

			int width = 0;
			int height = 0;
			glfwGetFramebufferSize(window, &width, &height);

			// When the window is minimized, wait until it is maximized
			while (width == 0 || height == 0)
			{
				glfwGetFramebufferSize(window, &width, &height);
				glfwWaitEvents();
			}

			vkDeviceWaitIdle(device->GetVkDevice());

			swapChain = std::make_unique<SwapChain>(instance->GetGPU(), *device.get(), *surface.get(), static_cast<uint32_t>(width), static_cast<uint32_t>(height));
		}

		void Renderer::DestroySwapChain()
		{
			swapChain.reset();
		}
	}
}