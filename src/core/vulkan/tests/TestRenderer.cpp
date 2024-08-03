// MIT License, Copyright (c) 2024 Malik Allen

#include "TestRenderer.h"

#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/resource/Buffer.h"
#include "../src/core/vulkan/resource/Image.h"
#include "../src/core/vulkan/commands/CommandBuffer.h"
#include "../src/core/vulkan/presentation/SwapChain.h"
#include "../src/core/vulkan/pipeline/RenderPass.h"
#include "../src/core/vulkan/pipeline/Framebuffer.h"
#include "../src/core/vulkan/pipeline/ShaderModule.h"
#include "../src/core/vulkan/pipeline/GraphicsPipeline.h"
#include "../src/core/vulkan/descriptors/DescriptorPool.h"
#include "../src/core/vulkan/descriptors/DescriptorSetLayout.h"
#include "../src/core/vulkan/descriptors/DescriptorSet.h"
#include "../src/core/3d/MeshHandler.h"
#include "../src/core/3d/Mesh.h"
#include "../src/core/3d/Camera.h"
#include "../src/core/3d/Texture.h"

namespace Baal
{
	namespace VK
	{
		TestRenderer::TestRenderer(const std::string& appName, GLFWwindow* _window):
			Renderer(appName, _window)
		{}

		TestRenderer::~TestRenderer()
		{
		}

		void TestRenderer::Initialize()
		{
			CreateDefaultCamera();
			CreateDescriptorPool();
			CreateDescriptorSetLayout();
			CreatePipelines();
			CreateDescriptorSet();

			// image = std::make_unique<Image>(GetDevice(), 400, 600, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT, VK_SAMPLE_COUNT_1_BIT, 400 * 600 * 4, nullptr);
			texture = std::make_unique<TextureInstance>(GetDevice(), Texture(BAAL_TEXTURES_DIR, "CheckerboardPattern.png", VK_IMAGE_TYPE_2D));
		}

		void TestRenderer::Destroy()
		{
			image.reset();
			texture.reset();

			DestroyPipelines();
			descriptorSetLayout.reset();
			descriptorPool.reset();
			DestroyDefaultCamera();
		}

		void TestRenderer::RecordDrawCommandBuffer(CommandBuffer& commandBuffer, Framebuffer& frameBuffer)
		{
			VkRenderPassBeginInfo renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
			renderPassInfo.renderPass = GetRenderPass().GetVkRenderPass();
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = GetSwapChain().GetExtent();

			renderPassInfo.framebuffer = frameBuffer.GetVkFramebuffer();

			commandBuffer.Reset();

			commandBuffer.BeginRecording(0);

			VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffer.GetVkCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffer.GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, forwardPipeline->GetVkGraphicsPipeline());

			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(GetSwapChain().GetExtent().width);
			viewport.height = static_cast<float>(GetSwapChain().GetExtent().height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(commandBuffer.GetVkCommandBuffer(), 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = GetSwapChain().GetExtent();
			vkCmdSetScissor(commandBuffer.GetVkCommandBuffer(), 0, 1, &scissor);

			VkDeviceSize offsets[] = { 0 };


			std::vector<std::shared_ptr<SubMeshInstance>>& subMeshes = GetMeshHandler().GetSubMeshInstances();
			for (size_t i = 0; i < subMeshes.size(); ++i)
			{
				vkCmdBindVertexBuffers(commandBuffer.GetVkCommandBuffer(), 0, 1, &subMeshes[i]->vertexBuffer->GetVkBuffer(), offsets);
				vkCmdBindIndexBuffer(commandBuffer.GetVkCommandBuffer(), subMeshes[i]->indexBuffer->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);

				vkCmdPushConstants(commandBuffer.GetVkCommandBuffer(), forwardPipeline->GetVkGraphicsPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshMatrices), &GetMeshHandler().GetMeshInstances()[subMeshes[i]->parentId]->matrices);

				vkCmdBindDescriptorSets(commandBuffer.GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, forwardPipeline->GetVkGraphicsPipelineLayout(), 0, 1, &descriptorSet->GetVkDescriptorSet(), 0, nullptr);

				vkCmdDrawIndexed(commandBuffer.GetVkCommandBuffer(), subMeshes[i]->indexCount, 1, 0, 0, 0);
			}

			vkCmdEndRenderPass(commandBuffer.GetVkCommandBuffer());

			commandBuffer.EndRecording();
		}

		void TestRenderer::PreRender()
		{
			std::vector<std::shared_ptr<MeshInstance>>& meshInstances = GetMeshHandler().GetMeshInstances();
			for (size_t i = 0; i < meshInstances.size(); ++i)
			{
				if (i % 2 == 0)
				{
					meshInstances[i]->matrices.model = Matrix4f::Translate(Vector3f(1.0f, 1.0f, 1.0f) * static_cast<float>(i)) * Matrix4f::Rotate(45.0f * static_cast<float>(i), Vector3f(0.0f, 1.0f, 0.0f)) * Matrix4f::Scale(Vector3f(1.0f));
				}
			}

			cameraUniformBuffer->Update(&camera->GetMatrices(), sizeof(CameraMatrices));
		}

		void TestRenderer::PostRender()
		{
		}

		void TestRenderer::CreatePipelines()
		{
			CreateForwardPipeline();
		}

		void TestRenderer::DestroyPipelines()
		{
			forwardPipeline.reset();
		}

		void TestRenderer::CreateForwardPipeline()
		{
			std::vector<ShaderInfo> shaderInfo;
			shaderInfo.push_back(ShaderInfo(VK_SHADER_STAGE_VERTEX_BIT, BAAL_SHADERS_DIR, "MVP.vert"));
			shaderInfo.push_back(ShaderInfo(VK_SHADER_STAGE_FRAGMENT_BIT, BAAL_SHADERS_DIR, "Triangle.frag"));

			forwardPipeline = std::make_unique<GraphicsPipeline>(GetDevice(), shaderInfo, GetRenderPass(), *descriptorSetLayout.get(), GetSwapChain().GetExtent().width, GetSwapChain().GetExtent().height);
		}

		void TestRenderer::CreateDescriptorPool()
		{
			std::vector<DescriptorPoolSize> poolSizes;
			poolSizes.push_back(DescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1));

			descriptorPool = std::make_unique<DescriptorPool>(GetDevice(), poolSizes);
		}

		void TestRenderer::CreateDescriptorSetLayout()
		{
			std::vector<DescriptorSetBinding> bindings;
			bindings.push_back(DescriptorSetBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0, 1));

			descriptorSetLayout = std::make_unique<DescriptorSetLayout>(GetDevice(), bindings);
		}

		void TestRenderer::CreateDescriptorSet()
		{
			descriptorSet = std::make_unique<DescriptorSet>(GetDevice(), *descriptorPool.get(), *descriptorSetLayout.get());

			VkDescriptorBufferInfo buffeInfo{};
			buffeInfo.buffer = cameraUniformBuffer->GetVkBuffer();
			buffeInfo.offset = 0;
			buffeInfo.range = cameraUniformBuffer->GetSize();

			VkWriteDescriptorSet camDescWrite = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
			camDescWrite.dstSet = descriptorSet->GetVkDescriptorSet();
			camDescWrite.dstBinding = 0;
			camDescWrite.dstArrayElement = 0;
			camDescWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			camDescWrite.descriptorCount = 1;
			camDescWrite.pBufferInfo = &buffeInfo;
			camDescWrite.pImageInfo = nullptr; // Optional
			camDescWrite.pTexelBufferView = nullptr; // Optional

			vkUpdateDescriptorSets(GetDevice().GetVkDevice(), 1, &camDescWrite, 0, nullptr);
		}

		void TestRenderer::CreateDefaultCamera()
		{
			camera = std::make_unique<Camera>();
			cameraUniformBuffer = std::make_unique<Buffer>(GetAllocator(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeof(CameraMatrices), &camera->GetMatrices());
		}

		void TestRenderer::DestroyDefaultCamera()
		{
			cameraUniformBuffer.reset();
			camera.reset();
		}
	}
}