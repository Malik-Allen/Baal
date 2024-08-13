// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_TEST_RENDERER_H
#define BAAL_VK_TEST_RENDERER_H

#include "../src/core/vulkan/Renderer.h"

namespace Baal
{
	namespace VK
	{
		class Buffer;
		class Image;
		class CommandBuffer;
		class GraphicsPipeline;
		class RenderPass;
		class Framebuffer;
		class DescriptorPool;
		class DescriptorSetLayout;
		class DescriptorSet;
		class TextureInstance;
		class Sampler;
		class MeshInstance;
		class PointLight;

		class TestRenderer : public Renderer
		{
		public:
			TestRenderer();
			TestRenderer(const TestRenderer&) = delete;
			TestRenderer(TestRenderer&&) = delete;

			virtual ~TestRenderer();

			TestRenderer& operator=(const TestRenderer&) = delete;
			TestRenderer& operator = (TestRenderer&&) = delete;

			void DestroyTarget();

		private:
			virtual void Initialize() override final;
			virtual void Destroy() override final;
			virtual void RecordDrawCommandBuffer(CommandBuffer& commandBuffer, Framebuffer& frameBuffer) override final;
			virtual void PreRender() override final;
			virtual void PostRender() override final;

			std::unique_ptr<GraphicsPipeline> forwardPipeline;

			float rotation = 0.0f;

			std::unique_ptr<DescriptorPool> descriptorPool;
			std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
			std::unique_ptr<DescriptorSet> descriptorSet;

			std::unique_ptr<TextureInstance> texture;
			std::unique_ptr<Sampler> textureSampler;

			std::weak_ptr<MeshInstance> destroyTarget;

			uint32_t dynamicAlignment;
			std::unique_ptr<Buffer> lightsUBO;
			std::vector<PointLight> lights;

			void CreatePipelines();
			void DestroyPipelines();
			void CreateForwardPipeline();

			void CreateDescriptorPool();
			void CreateDescriptorSetLayout();
			void CreateDescriptorSet();

			void CreateLights();
			void DestroyLights();
		};
	}
}

#endif // !BAAL_VK_TEST_RENDERER_H