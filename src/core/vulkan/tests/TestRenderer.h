// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_TEST_RENDERER_H
#define BAAL_VK_TEST_RENDERER_H

#include "../src/core/vulkan/Renderer.h"

namespace Baal
{
	namespace VK
	{
		class Buffer;
		class CommandBuffer;
		class GraphicsPipeline;
		class RenderPass;
		class Framebuffer;
		class DescriptorPool;
		class DescriptorSetLayout;
		class DescriptorSet;
		class Camera;

		class TestRenderer : public Renderer
		{
		public:
			explicit TestRenderer(const std::string& appName, GLFWwindow* _window);
			TestRenderer(const TestRenderer&) = delete;
			TestRenderer(TestRenderer&&) = delete;

			virtual ~TestRenderer();

			TestRenderer& operator=(const TestRenderer&) = delete;
			TestRenderer& operator = (TestRenderer&&) = delete;

		private:
			virtual void Initialize() override final;
			virtual void Destroy() override final;
			virtual void RecordDrawCommandBuffer(CommandBuffer& commandBuffer, Framebuffer& frameBuffer) override final;
			virtual void PreRender() override final;
			virtual void PostRender() override final;

			std::unique_ptr<GraphicsPipeline> forwardPipeline;

			std::unique_ptr<DescriptorPool> descriptorPool;
			std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
			std::unique_ptr<DescriptorSet> descriptorSet;			

			std::unique_ptr<Camera> camera;
			std::unique_ptr<Buffer> cameraUniformBuffer;

			void CreatePipelines();
			void DestroyPipelines();
			void CreateForwardPipeline();

			void CreateDescriptorPool();
			void CreateDescriptorSetLayout();
			void CreateDescriptorSet();

			void CreateDefaultCamera();
			void DestroyDefaultCamera();
		};
	}
}

#endif // !BAAL_VK_TEST_RENDERER_H