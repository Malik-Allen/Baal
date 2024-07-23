// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_FRAMEBUFFER_H
#define BAAL_FRAMEBUFFER_H

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;
		class RenderPass;

		class Framebuffer
		{
		public:
			explicit Framebuffer(LogicalDevice& _device, RenderPass& renderPass, std::vector<VkImageView> attachments, const uint32_t width, const uint32_t height);
			Framebuffer(const Framebuffer&) = delete;
			Framebuffer(Framebuffer&& other) noexcept;

			~Framebuffer();

			Framebuffer& operator=(const Framebuffer&) = delete;
			Framebuffer& operator = (Framebuffer&&) = delete;

		private:
			VkFramebuffer vkFramebuffer{ VK_NULL_HANDLE };
			LogicalDevice& device;
		};
	}
}

#endif // !BAAL_FRAMEBUFFER_H