// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_SWAPCHAIN_H
#define BAAL_VK_SWAPCHAIN_H

#include <vulkan/vulkan_core.h>

namespace Baal
{
	namespace VK 
	{
		class LogicalDevice;
		class Surface;

		class SwapChain
		{
		public:
			explicit SwapChain(LogicalDevice& _device, Surface& surface);
			SwapChain(const SwapChain&) = delete;
			SwapChain(SwapChain&&) = delete;

			~SwapChain();

			SwapChain& operator=(const SwapChain&) = delete;
			SwapChain& operator = (SwapChain&&) = delete;

		private:
			VkSwapchainKHR vkSwapChain{VK_NULL_HANDLE};
			LogicalDevice& device;
		};
	}
}

#endif // !BAAL_VK_SWAPCHAIN_H
