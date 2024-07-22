// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_RENDERPASS_H
#define BAAL_RENDERPASS_H

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;

		struct Attachment
		{
			VkImageUsageFlagBits usage;
			VkAttachmentDescription description;
		};

		// Used to define the color and depth buffers, sample counts, and data loading and saving before an after rendering operations are performed.
		// Can be made of multiple subpasses.

		class RenderPass
		{
		public:
			explicit RenderPass(LogicalDevice& _device, const std::vector<Attachment>& _attachments);
			RenderPass(const RenderPass&) = delete;
			RenderPass(RenderPass&&) = delete;

			~RenderPass();

			RenderPass& operator=(const RenderPass&) = delete;
			RenderPass& operator = (RenderPass&&) = delete;

			VkRenderPass& GetVkRenderPass() { return vkRenderPass; };

		private:
			VkRenderPass vkRenderPass{VK_NULL_HANDLE};
			LogicalDevice& device;
			std::vector<Attachment> attachments;
		};
	}
}

#endif // !BAAL_RENDERPASS_H