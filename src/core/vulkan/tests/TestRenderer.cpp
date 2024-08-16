// MIT License, Copyright (c) 2024 Malik Allen

#include "TestRenderer.h"

#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/devices/PhysicalDevice.h"
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
#include "../src/core/3d/Light.h"
#include "../src/core/vulkan/resource/Sampler.h"

#include <array>

namespace Baal
{
	namespace VK
	{
		TestRenderer::TestRenderer()
		{}

		TestRenderer::~TestRenderer()
		{}

		void TestRenderer::DestroyTarget()
		{
			GetMeshHandler().DestroyMeshInstance(destroyTarget);
		}

		void TestRenderer::Initialize()
		{
			AddMeshInstanceToScene(LoadMeshResource(BAAL_MODELS_DIR, "spoon.obj"));
			AddMeshInstanceToScene(LoadMeshResource(BAAL_MODELS_DIR, "spoon.obj"));
			destroyTarget = AddMeshInstanceToScene(LoadMeshResource(BAAL_MODELS_DIR, "spoon.obj"));
			AddMeshInstanceToScene(LoadMeshResource(BAAL_MODELS_DIR, "Skull.obj"));
			AddMeshInstanceToScene(LoadMeshResource(BAAL_MODELS_DIR, "teacup.obj"));
			AddMeshInstanceToScene(LoadMeshResource(BAAL_MODELS_DIR, "teacup.obj"));
			AddMeshInstanceToScene(LoadMeshResource(BAAL_MODELS_DIR, "spoon.obj"));
			AddMeshInstanceToScene(LoadMeshResource(BAAL_MODELS_DIR, "teapot.obj"));
			AddMeshInstanceToScene(LoadMeshResource(BAAL_MODELS_DIR, "teacup.obj"));

			CreateTextures();

			CreateTestLights();

			CreateDescriptorPool();
			CreateDescriptorSetLayout();
			CreatePipelines();
			CreateDescriptorSet();
		}

		void TestRenderer::Destroy()
		{
			DestroyPipelines();
			descriptorSetLayout.reset();
			descriptorPool.reset();

			DestroyTextures();
			
			DestroyTestLights();
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

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = clearValues.size();
			renderPassInfo.pClearValues = clearValues.data();

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
				vkCmdBindVertexBuffers(commandBuffer.GetVkCommandBuffer(), 0, 1, &subMeshes[i]->GetVertexBuffer().GetVkBuffer(), offsets);
				vkCmdBindIndexBuffer(commandBuffer.GetVkCommandBuffer(), subMeshes[i]->GetIndexBuffer().GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);

				VertexPushConstants vertConstants(GetMeshHandler().GetMeshInstances()[subMeshes[i]->GetParentId()]->model);
				vkCmdPushConstants(commandBuffer.GetVkCommandBuffer(), forwardPipeline->GetVkGraphicsPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(VertexPushConstants), &vertConstants);

				FragmentPushConstants fragConstants(subMeshes[i]->GetMaterial());
				vkCmdPushConstants(commandBuffer.GetVkCommandBuffer(), forwardPipeline->GetVkGraphicsPipelineLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(VertexPushConstants), sizeof(FragmentPushConstants), &fragConstants);
				
				uint32_t dynamicOffset = 3 * static_cast<uint32_t>(dynamicAlignment);
				vkCmdBindDescriptorSets(commandBuffer.GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, forwardPipeline->GetVkGraphicsPipelineLayout(), 0, 1, &descriptorSet->GetVkDescriptorSet(), 1, &dynamicOffset);

				vkCmdDrawIndexed(commandBuffer.GetVkCommandBuffer(), subMeshes[i]->GetIndexCount(), 1, 0, 0, 0);
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
					modelRotation += 0.3f;

					meshInstances[i]->model = Matrix4f::Translate(Vector3f(1.0f, 1.0f, 1.0f) * static_cast<float>(i)) * Matrix4f::Rotate(modelRotation * static_cast<float>(i), Vector3f(0.0f, 1.0f, 0.0f)) * Matrix4f::Scale(Vector3f(2.0f));
				}
			}

			GetDirectionalLight().color.r += 1;
			GetDirectionalLight().color.g += 2;
			GetDirectionalLight().color.b += 3;

			lightRotation = 133.0f;

			Quatf quat;
			quat.RotateAxis(Vector3f(0.5f, 1.0f, 0.0f), lightRotation);
			Vector3f direction = quat.RotateVector(Vector3f::ForwardVector);

			GetDirectionalLight().color = Color::White;
			GetDirectionalLight().direction = direction;

			GetDirectionalLightUniformBuffer().Update(&GetDirectionalLight(), GetDirectionalLightUniformBuffer().GetSize());

			GetPointLight(3).color.r -= 1;
			GetPointLight(3).color.g -= 2;
			GetPointLight(3).color.b -= 3;

			GetPointLightsUniformBuffer().Update(&GetPointLight(3), sizeof(PointLight), 3 * sizeof(PointLight));
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
			shaderInfo.push_back(ShaderInfo(VK_SHADER_STAGE_VERTEX_BIT, BAAL_SHADERS_DIR, "Phong.vert"));
			shaderInfo.push_back(ShaderInfo(VK_SHADER_STAGE_FRAGMENT_BIT, BAAL_SHADERS_DIR, "Phong.frag"));
			
			VkPipelineRasterizationStateCreateInfo rasterizer = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;
			rasterizer.depthBiasConstantFactor = 0.0f;
			rasterizer.depthBiasClamp = 0.0f;
			rasterizer.depthBiasSlopeFactor = 0.0f;

			VkPushConstantRange vertPushConstant = {};
			vertPushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			vertPushConstant.offset = 0;
			vertPushConstant.size = sizeof(VertexPushConstants);

			VkPushConstantRange fragPushConstant = {};
			fragPushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragPushConstant.offset = sizeof(VertexPushConstants);
			fragPushConstant.size = sizeof(FragmentPushConstants);

			std::vector<VkPushConstantRange> pushConstants;
			pushConstants.push_back(vertPushConstant);
			pushConstants.push_back(fragPushConstant);

			forwardPipeline = std::make_unique<GraphicsPipeline>(GetDevice(), shaderInfo, GetRenderPass(), *descriptorSetLayout.get(), rasterizer, pushConstants, GetSwapChain().GetExtent().width, GetSwapChain().GetExtent().height);
		}

		void TestRenderer::CreateDescriptorPool()
		{
			std::vector<DescriptorPoolSize> poolSizes;
			poolSizes.push_back(DescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1));
			poolSizes.push_back(DescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1));
			poolSizes.push_back(DescriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3));

			descriptorPool = std::make_unique<DescriptorPool>(GetDevice(), poolSizes);
		}

		void TestRenderer::CreateDescriptorSetLayout()
		{
			std::vector<DescriptorSetBinding> bindings;
			bindings.push_back(DescriptorSetBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0, 1));	// Camera
			bindings.push_back(DescriptorSetBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, 1, 1));	// Test Lights
			bindings.push_back(DescriptorSetBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 2, 1));	// Texture Sampler
			bindings.push_back(DescriptorSetBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 3, 1));	// Directional Light 
			bindings.push_back(DescriptorSetBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 4, 1));	// Point Light
			
			descriptorSetLayout = std::make_unique<DescriptorSetLayout>(GetDevice(), bindings);
		}

		void TestRenderer::CreateDescriptorSet()
		{
			descriptorSet = std::make_unique<DescriptorSet>(GetDevice(), *descriptorPool.get(), *descriptorSetLayout.get());

			VkDescriptorBufferInfo camInfo{};
			camInfo.buffer = GetCameraUniformBuffer().GetVkBuffer();
			camInfo.offset = 0;
			camInfo.range = GetCameraUniformBuffer().GetSize();

			VkWriteDescriptorSet camDescWrite = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
			camDescWrite.dstSet = descriptorSet->GetVkDescriptorSet();
			camDescWrite.dstBinding = 0;
			camDescWrite.dstArrayElement = 0;
			camDescWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			camDescWrite.descriptorCount = 1;
			camDescWrite.pBufferInfo = &camInfo;
			camDescWrite.pImageInfo = nullptr; // Optional
			camDescWrite.pTexelBufferView = nullptr; // Optional

			VkDescriptorBufferInfo lightInfo{};
			lightInfo.buffer = lightsUBO->GetVkBuffer();
			lightInfo.offset = 0;
			lightInfo.range = dynamicAlignment;

			VkWriteDescriptorSet lightDescWrite = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
			lightDescWrite.dstSet = descriptorSet->GetVkDescriptorSet();
			lightDescWrite.dstBinding = 1;
			lightDescWrite.dstArrayElement = 0;
			lightDescWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			lightDescWrite.descriptorCount = 1;
			lightDescWrite.pBufferInfo = &lightInfo;
			lightDescWrite.pImageInfo = nullptr; // Optional
			lightDescWrite.pTexelBufferView = nullptr; // Optional

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = texture->GetImage().GetVkImageView();
			imageInfo.sampler = textureSampler->GetVkSampler();

			VkWriteDescriptorSet imageDescWrite = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
			imageDescWrite.dstSet = descriptorSet->GetVkDescriptorSet();
			imageDescWrite.dstBinding = 2;
			imageDescWrite.dstArrayElement = 0;
			imageDescWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			imageDescWrite.descriptorCount = 1;
			imageDescWrite.pBufferInfo = nullptr;
			imageDescWrite.pImageInfo = &imageInfo; // Optional
			imageDescWrite.pTexelBufferView = nullptr; // Optional

			VkDescriptorBufferInfo dlightInfo{};
			dlightInfo.buffer = GetDirectionalLightUniformBuffer().GetVkBuffer();
			dlightInfo.offset = 0;
			dlightInfo.range = GetDirectionalLightUniformBuffer().GetSize();

			VkWriteDescriptorSet dlightDescWrite = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
			dlightDescWrite.dstSet = descriptorSet->GetVkDescriptorSet();
			dlightDescWrite.dstBinding = 3;
			dlightDescWrite.dstArrayElement = 0;
			dlightDescWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			dlightDescWrite.descriptorCount = 1;
			dlightDescWrite.pBufferInfo = &dlightInfo;
			dlightDescWrite.pImageInfo = nullptr; // Optional
			dlightDescWrite.pTexelBufferView = nullptr; // Optional

			VkDescriptorBufferInfo plightInfo{};
			plightInfo.buffer = GetPointLightsUniformBuffer().GetVkBuffer();
			plightInfo.offset = 0;
			plightInfo.range = GetPointLightsUniformBuffer().GetSize();

			VkWriteDescriptorSet plightDescWrite = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
			plightDescWrite.dstSet = descriptorSet->GetVkDescriptorSet();
			plightDescWrite.dstBinding = 4;
			plightDescWrite.dstArrayElement = 0;
			plightDescWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			plightDescWrite.descriptorCount = 1;
			plightDescWrite.pBufferInfo = &plightInfo;
			plightDescWrite.pImageInfo = nullptr; // Optional
			plightDescWrite.pTexelBufferView = nullptr; // Optional

			std::vector<VkWriteDescriptorSet> descriptorWrites;
			descriptorWrites.push_back(camDescWrite);
			descriptorWrites.push_back(lightDescWrite);
			descriptorWrites.push_back(imageDescWrite);
			descriptorWrites.push_back(dlightDescWrite);
			descriptorWrites.push_back(plightDescWrite);

			vkUpdateDescriptorSets(GetDevice().GetVkDevice(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
		}

		void TestRenderer::CreateTextures()
		{
			texture = std::make_unique<TextureInstance>(GetDevice(), Texture(BAAL_TEXTURES_DIR, "CheckerboardPattern.png", VK_IMAGE_TYPE_2D));

			VkSamplerCreateInfo samplerInfo = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.anisotropyEnable = VK_FALSE;
			samplerInfo.maxAnisotropy = 1.0f;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;

			textureSampler = std::make_unique<Sampler>(GetDevice(), GetInstance().GetGPU(), samplerInfo);
		}

		void TestRenderer::DestroyTextures()
		{
			textureSampler.reset();
			texture.reset();
		}

		void TestRenderer::CreateTestLights()
		{
			PointLight p;
			p.color = Color::Blue;
			lights.reserve(6);
			lights.push_back(PointLight());
			lights.push_back(PointLight());
			lights.push_back(PointLight());
			lights.push_back(PointLight());
			lights.push_back(PointLight());
			lights.push_back(PointLight());

			lights[0].color = Color::White;
			lights[1].color = Color::Yellow;
			lights[2].color = Color::White;
			lights[3].color = Color::Yellow;
			lights[4].color = Color::Red;

			size_t minUniformBufferOffsetAlignment = static_cast<size_t>(GetInstance().GetGPU().GetProperties().limits.minUniformBufferOffsetAlignment);
			dynamicAlignment = sizeof(PointLight);
			if (minUniformBufferOffsetAlignment > 0)
			{
				dynamicAlignment = (dynamicAlignment + minUniformBufferOffsetAlignment - 1) & ~(minUniformBufferOffsetAlignment - 1);
			}

			const VkDeviceSize bufferSize = dynamicAlignment * lights.size();
			Buffer stagingBuffer = Buffer::CreateStagingBuffer(GetAllocator(), bufferSize, lights.data());
			

			lightsUBO = std::make_unique<Buffer>(GetAllocator(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize);
			GetDevice().CopyBuffer(stagingBuffer, *lightsUBO.get(), bufferSize);


			lights[0].color = Color::Cyan;

			for (size_t i = 0; i < lights.size(); i++) 
			{
				lightsUBO->Update(&lights[i], dynamicAlignment, (i * dynamicAlignment));
			}
		}

		void TestRenderer::DestroyTestLights()
		{
			lightsUBO.reset();
			lights.clear();
		}
	}
}