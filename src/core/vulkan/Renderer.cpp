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
#include "../src/core/vulkan/pipeline/ShaderModule.h"
#include "../src/core/vulkan/pipeline/GraphicsPipeline.h"
#include "../src/core/vulkan/pipeline/RenderPass.h"
#include "../src/core/vulkan/pipeline/Framebuffer.h"

#include <vulkan/vulkan_core.h>
#include <stdexcept>
#include <GLFW/glfw3.h>

namespace Baal
{
	namespace VK
	{
		Renderer::Renderer(const std::string& appName, GLFWwindow* window)
		{
			const std::vector<const char*> instanceExtensions = GetRequiredInstanceExtenstions();
			const std::vector<const char*> deviceExtensions = GetRequiredDeviceExtenstions();

			instance = std::make_unique<Instance>(appName, true, instanceExtensions);

			surface = std::make_unique<Surface>(*instance.get(), window);

			device = std::make_unique<LogicalDevice>(instance->GetGPU(), *surface.get(), deviceExtensions);

			int width = 0;
			int height = 0;
			glfwGetFramebufferSize(window, &width, &height);

			swapChain = std::make_unique<SwapChain>(instance->GetGPU(), *device.get(), *surface.get(), static_cast<uint32_t>(width), static_cast<uint32_t>(height));

			commandPool = std::make_unique<CommandPool>(*device.get(), instance->GetGPU().GetQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT));
		}

		Renderer::~Renderer() 
		{
			DestroyDrawCommandBuffers();

			commandPool.reset();

			DestroyFramebuffers();

			DestroyPipelines();

			DestroySwapChainImageViews();

			swapChain.reset();

			device.reset();

			surface.reset();

			instance.reset();
		}

		void Renderer::Init()
		{
			CreateSwapChainImageViews();

			CreatePipelines();

			CreateFramebuffers();

			CreateDrawCommandBuffers();
		}

		void Renderer::RenderFrame()
		{
			BuildCommandBuffers();

			VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &drawCommands[currentFrame].GetVkCommandBuffer();

			VK_CHECK(vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, nullptr), "submitting graphics queue");
			VK_CHECK(vkQueueWaitIdle(device->GetGraphicsQueue()), "waiting idle graphics queue");

			VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &swapChain->GetVkSwapChain();
			presentInfo.pImageIndices = &currentFrame;

			vkQueuePresentKHR(device->GetPresentQueue(), &presentInfo);
		}

		void Renderer::Shutdown()
		{
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

		void Renderer::CreatePipelines()
		{
			CreateForwardPipeline();
		}

		void Renderer::DestroyPipelines()
		{
			forwardPipeline.reset();

			renderPass.reset();
		}

		void Renderer::CreateForwardPipeline()
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

			std::vector<ShaderInfo> shaderInfo;
			shaderInfo.push_back(ShaderInfo(VK_SHADER_STAGE_VERTEX_BIT, BAAL_SHADERS_DIR, "Triangle.vert"));
			shaderInfo.push_back(ShaderInfo(VK_SHADER_STAGE_FRAGMENT_BIT, BAAL_SHADERS_DIR, "Triangle.frag"));
			
			forwardPipeline = std::make_unique<GraphicsPipeline>(*device.get(), shaderInfo, *renderPass.get(), swapChain->GetExtent().width, swapChain->GetExtent().height);
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
			VK_CHECK(commandPool->CreateCommandBuffers(framebufferCount, VK_COMMAND_BUFFER_LEVEL_PRIMARY, drawCommands), "creating draw commands");

			currentFrame = 0;
		}

		void Renderer::DestroyDrawCommandBuffers()
		{
			drawCommands.clear();
		}

		void Renderer::BuildCommandBuffers()
		{
			VkRenderPassBeginInfo renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
			renderPassInfo.renderPass = renderPass->GetVkRenderPass();
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChain->GetExtent();

			for (size_t i = 0; i < drawCommands.size(); ++i)
			{
				renderPassInfo.framebuffer = framebuffers[i].GetVkFramebuffer();

				drawCommands[i].Reset();

				drawCommands[i].BeginRecording(0);

				VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
				renderPassInfo.clearValueCount = 1;
				renderPassInfo.pClearValues = &clearColor;

				vkCmdBeginRenderPass(drawCommands[i].GetVkCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(drawCommands[i].GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, forwardPipeline->GetVkGraphicsPipeline());

				VkViewport viewport{};
				viewport.x = 0.0f;
				viewport.y = 0.0f;
				viewport.width = static_cast<float>(swapChain->GetExtent().width);
				viewport.height = static_cast<float>(swapChain->GetExtent().height);
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;
				vkCmdSetViewport(drawCommands[i].GetVkCommandBuffer(), 0, 1, &viewport);

				VkRect2D scissor{};
				scissor.offset = { 0, 0 };
				scissor.extent = swapChain->GetExtent();
				vkCmdSetScissor(drawCommands[i].GetVkCommandBuffer(), 0, 1, &scissor);

				vkCmdDraw(drawCommands[i].GetVkCommandBuffer(), 3, 1, 0, 0);

				vkCmdEndRenderPass(drawCommands[i].GetVkCommandBuffer());

				drawCommands[i].EndRecording();
			}
		}
	}
}