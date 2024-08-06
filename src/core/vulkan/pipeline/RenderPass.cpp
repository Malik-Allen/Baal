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
			std::vector<VkAttachmentDescription> vkAttachments;
			std::vector<VkAttachmentReference> colorAttachmentRefs;
			std::vector<VkAttachmentReference> depthAttachmentRefs;

			VkAttachmentReference attachmentRef;
			for (size_t i = 0; i < attachments.size(); ++i)
			{
				vkAttachments.push_back(attachments[i].description);

				if (attachments[i].usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)	// For now we are only support the color attachment, in the future we will support depth/stencil buffer
				{
					attachmentRef.attachment = i;
					attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					colorAttachmentRefs.push_back(attachmentRef);
				}

				if (attachments[i].usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
				{
					attachmentRef.attachment = i;
					attachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					depthAttachmentRefs.push_back(attachmentRef);
				}
			}

			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.flags = 0;
			subpass.inputAttachmentCount = 0;
			subpass.preserveAttachmentCount = 0;
			subpass.colorAttachmentCount = colorAttachmentRefs.size();
			subpass.pColorAttachments = colorAttachmentRefs.data();
			subpass.pDepthStencilAttachment = depthAttachmentRefs.data();
			

			VkSubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
			renderPassInfo.flags = 0;
			renderPassInfo.attachmentCount = vkAttachments.size();
			renderPassInfo.pAttachments = vkAttachments.data();
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;
			
			VK_CHECK(vkCreateRenderPass(device.GetVkDevice(), &renderPassInfo, nullptr, &vkRenderPass), "creating renderpass");
		}

		RenderPass::~RenderPass()
		{
			vkDestroyRenderPass(device.GetVkDevice(), vkRenderPass, nullptr);
		}
	}
}