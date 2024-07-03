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
		SwapChain::SwapChain(PhysicalDevice& _gpu, LogicalDevice& _device, Surface& _surface, uint32_t width, uint32_t height):
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

			const VkSurfaceFormatKHR selectedSurfaceFormat = SelectSurfaceFormat(formats);
			const VkPresentModeKHR selectedPresentMode = SelectPresentMode(presentModes);

			VkSwapchainCreateInfoKHR swapChainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
			swapChainInfo.surface = surface.GetVkSurface();
			swapChainInfo.imageFormat = selectedSurfaceFormat.format;
			swapChainInfo.imageColorSpace = selectedSurfaceFormat.colorSpace;

			swapChainInfo.minImageCount = surfaceCapabilities.minImageCount;
			swapChainInfo.imageExtent = VkExtent2D(width, height);

			swapChainInfo.presentMode = selectedPresentMode;
			swapChainInfo.imageArrayLayers = surfaceCapabilities.maxImageArrayLayers;
			swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			swapChainInfo.preTransform = surfaceCapabilities.currentTransform;
			swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			swapChainInfo.clipped = VK_TRUE;			
			
			VK_CHECK(vkCreateSwapchainKHR(device.GetVkDevice(), &swapChainInfo, nullptr, &vkSwapChain), "creating swap chain");

			uint32_t imageCount = 0;
			VK_CHECK(vkGetSwapchainImagesKHR(device.GetVkDevice(), vkSwapChain, &imageCount, nullptr), "getting swap chain image count");
			images.resize(imageCount);
			VK_CHECK(vkGetSwapchainImagesKHR(device.GetVkDevice(), vkSwapChain, &imageCount, images.data()), "getting swap chain image data");
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

		VkSurfaceFormatKHR SwapChain::SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
		{
			if (formats.size() == 0)
			{
				DEBUG_LOG(LOG::ERRORLOG, "Cannot select Surface Format from empty list of formats!");
				assert(false);
			}

			// For now this will work to get things going, in the future we can create a priority list to select the desired surface format and color space
			for (size_t i = 0; i < formats.size(); ++i) 
			{
				if (formats[i].format == VK_FORMAT_R8G8B8A8_SRGB && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					return formats[i];
				}
			}
			return formats[0];
		}

		VkPresentModeKHR SwapChain::SelectPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
		{
			// For now this will work to get things going, in the future we can create a priority list to select the desired present mode
			for (size_t i = 0; i < presentModes.size(); ++i)
			{
				if (presentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
				{
					return presentModes[i];
				}
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}
	}
}