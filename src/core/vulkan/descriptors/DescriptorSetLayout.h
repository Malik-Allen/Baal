// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_DESCRIPTORSETLAYOUT_H
#define BAAL_VK_DESCRIPTORSETLAYOUT_H

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;

		struct DescriptorSetBinding
		{
			VkDescriptorType type;
			VkShaderStageFlags stage;
			uint32_t binding;
			uint32_t count;
		};

		class DescriptorSetLayout
		{
		public:
			explicit DescriptorSetLayout(LogicalDevice& _device, std::vector<DescriptorSetBinding> bindings);

			DescriptorSetLayout(const DescriptorSetLayout&) = delete;
			DescriptorSetLayout(DescriptorSetLayout&&) = delete;

			~DescriptorSetLayout();

			DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
			DescriptorSetLayout& operator=(DescriptorSetLayout&&) = delete;

			VkDescriptorSetLayout& GetVkDescriptorSetLayout() { return vkDescriptorSetLayout; }

		private:
			VkDescriptorSetLayout vkDescriptorSetLayout{ VK_NULL_HANDLE };
			LogicalDevice& device;
		};

	}
}

#endif // !BAAL_VK_DESCRIPTORSETLAYOUT_H