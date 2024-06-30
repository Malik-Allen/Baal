// MIT License, Copyright (c) 2024 Malik Allen

#include "SwapChain.h"

#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/presentation/Surface.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		SwapChain::SwapChain(LogicalDevice& _device, Surface& surface):
			device(_device)
		{
			VkSwapchainCreateInfoKHR swapChainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
			swapChainInfo.surface = surface.GetVkSurface();
			
			VK_CHECK(vkCreateSwapchainKHR(device.GetVkDevice(), &swapChainInfo, nullptr, &vkSwapChain), "creating swap chain");
		}

		SwapChain::~SwapChain()
		{
			vkDestroySwapchainKHR(device.GetVkDevice(), vkSwapChain, nullptr);
		}
	}
}