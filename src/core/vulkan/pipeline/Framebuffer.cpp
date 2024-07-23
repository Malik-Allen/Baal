// MIT License, Copyright (c) 2024 Malik Allen

#include "Framebuffer.h"

#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/pipeline/RenderPass.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		Framebuffer::Framebuffer(LogicalDevice& _device, RenderPass& renderPass, std::vector<VkImageView> attachments, const uint32_t width, const uint32_t height):
			device(_device)
		{
			VkFramebufferCreateInfo framebufferInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
			framebufferInfo.renderPass = renderPass.GetVkRenderPass();
			framebufferInfo.attachmentCount = attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = width;
			framebufferInfo.height = height;
			framebufferInfo.layers = 1;

			VK_CHECK(vkCreateFramebuffer(device.GetVkDevice(), &framebufferInfo, nullptr, &vkFramebuffer), "creating framebuffer");
		}

		Framebuffer::Framebuffer(Framebuffer&& other) noexcept:
			device(other.device),
			vkFramebuffer(other.vkFramebuffer)
		{
			other.vkFramebuffer = VK_NULL_HANDLE;
		}

		Framebuffer::~Framebuffer()
		{
			if (vkFramebuffer != VK_NULL_HANDLE) 
			{
				vkDestroyFramebuffer(device.GetVkDevice(), vkFramebuffer, nullptr);
			}
		}
	}
}