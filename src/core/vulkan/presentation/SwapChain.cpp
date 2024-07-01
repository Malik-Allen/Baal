// MIT License, Copyright (c) 2024 Malik Allen

#include "SwapChain.h"

#include "../src/core/vulkan/devices/PhysicalDevice.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/presentation/Surface.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		SwapChain::SwapChain(PhysicalDevice& _gpu, LogicalDevice& _device, Surface& _surface):
			gpu(_gpu),
			device(_device),
			surface(_surface)
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			QuerySurfaceCapabilities(surfaceCapabilities);

			std::vector<VkSurfaceFormatKHR> formats;
			QueurySurfaceFormats(formats);

			std::vector<VkPresentModeKHR> presentModes;
			QuerySurfacePresentModes(presentModes);

			VkSwapchainCreateInfoKHR swapChainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
			swapChainInfo.surface = surface.GetVkSurface();
			
			VK_CHECK(vkCreateSwapchainKHR(device.GetVkDevice(), &swapChainInfo, nullptr, &vkSwapChain), "creating swap chain");
		}

		SwapChain::~SwapChain()
		{
			vkDestroySwapchainKHR(device.GetVkDevice(), vkSwapChain, nullptr);
		}

		void SwapChain::QuerySurfaceCapabilities(VkSurfaceCapabilitiesKHR& outCapabilities)
		{
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu.GetVkPhysicalDevice(), surface.GetVkSurface(), &outCapabilities);
		}

		void SwapChain::QueurySurfaceFormats(std::vector<VkSurfaceFormatKHR>& outFormats)
		{
			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(gpu.GetVkPhysicalDevice(), surface.GetVkSurface(), &formatCount, nullptr);
			outFormats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(gpu.GetVkPhysicalDevice(), surface.GetVkSurface(), &formatCount, outFormats.data());

			DEBUG_LOG(LOG::INFO, "There are {} Surface Formats available!", formatCount);

			std::string formatString;
			std::string colorSpaceString;
			for (auto& format : outFormats) 
			{
				formatString = std::string(string_VkFormat(format.format));
				colorSpaceString = std::string(string_VkColorSpaceKHR(format.colorSpace));
				DEBUG_LOG(LOG::INFO, "Format: {}, ColorSpace: {}", formatString, colorSpaceString);
			}
		}

		void SwapChain::QuerySurfacePresentModes(std::vector<VkPresentModeKHR>& outPresentModes)
		{
			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(gpu.GetVkPhysicalDevice(), surface.GetVkSurface(), &presentModeCount, nullptr);
			outPresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(gpu.GetVkPhysicalDevice(), surface.GetVkSurface(), &presentModeCount, outPresentModes.data());

			DEBUG_LOG(LOG::INFO, "There are {} Present Modes available!", presentModeCount);

			std::string presentModeString;
			for (auto& presentMode : outPresentModes)
			{
				presentModeString = std::string(string_VkPresentModeKHR(presentMode));
				DEBUG_LOG(LOG::INFO, "PresentMode: {} ", presentModeString);
			}
		}
	}
}