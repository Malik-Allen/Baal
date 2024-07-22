// MIT License, Copyright (c) 2024 Malik Allen

#include "RenderPass.h"

#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		RenderPass::RenderPass(LogicalDevice& _device, const std::vector<Attachment>& _attachments):
			device(_device),
			attachments(_attachments)
		{
			std::vector<VkAttachmentDescription2> vkAttachments;
			std::vector<VkAttachmentReference2> colorAttachmentRefs;

			VkAttachmentReference2 attachmentRef;
			for (size_t i = 0; i < attachments.size(); ++i)
			{
				vkAttachments.push_back(attachments[i].description);

				if (attachments[i].usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)	// For now we are only support the color attachment, in the future we will support depth/stencil buffer
				{
					attachmentRef.attachment = i;
					attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					colorAttachmentRefs.push_back(attachmentRef);
				}
			}

			VkSubpassDescription2 subpass = {VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.flags = 0;
			subpass.inputAttachmentCount = 0;
			subpass.preserveAttachmentCount = 0;
			subpass.colorAttachmentCount = colorAttachmentRefs.size();
			subpass.pColorAttachments = colorAttachmentRefs.data();

			VkRenderPassCreateInfo2 renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2 };
			renderPassInfo.flags = 0;
			renderPassInfo.dependencyCount = 0;
			renderPassInfo.attachmentCount = vkAttachments.size();
			renderPassInfo.pAttachments = vkAttachments.data();

			// For now we will use a single subpass
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			
			VK_CHECK(vkCreateRenderPass2(device.GetVkDevice(), &renderPassInfo, nullptr, &vkRenderPass), "creating renderpass");
		}

		RenderPass::~RenderPass()
		{
			vkDestroyRenderPass(device.GetVkDevice(), vkRenderPass, nullptr);
		}
	}
}