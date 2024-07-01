// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_SWAPCHAIN_H
#define BAAL_VK_SWAPCHAIN_H

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal
{
	namespace VK 
	{
		class PhysicalDevice;
		class LogicalDevice;
		class Surface;

		// Represents a queue of images waiting to presented to the screen

		class SwapChain
		{
		public:
			explicit SwapChain(PhysicalDevice& _gpu, LogicalDevice& _device, Surface& _surface);
			SwapChain(const SwapChain&) = delete;
			SwapChain(SwapChain&&) = delete;

			~SwapChain();

			SwapChain& operator=(const SwapChain&) = delete;
			SwapChain& operator = (SwapChain&&) = delete;

		private:
			VkSwapchainKHR vkSwapChain{VK_NULL_HANDLE};
			PhysicalDevice& gpu;
			LogicalDevice& device;
			Surface& surface;

			void QuerySurfaceCapabilities(VkSurfaceCapabilitiesKHR& outCapabilities);
			void QueurySurfaceFormats(std::vector<VkSurfaceFormatKHR>& outFormats);
			void QuerySurfacePresentModes(std::vector<VkPresentModeKHR>& outPresentModes);
		};
	}
}

#endif // !BAAL_VK_SWAPCHAIN_H
