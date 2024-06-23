// MIT License, Copyright (c) 2023 Malik Allen

#ifndef BAAL_VK_INSTANCE_H
#define BAAL_VK_INSTANCE_H

#include <vulkan/vulkan_core.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

namespace Baal
{
	namespace VK
	{
		class PhysicalDevice;

		/*
		* Main responsibilities:
		*	Initializing the VkInstance
		*	Setting up debug callbacks
		*	Enabling extensions passed to the Instance
		*	Enabling the validation layers
		*	Finding and creating all PhysicalDevices(GPUs)
		*/
		class Instance
		{
		public:
			explicit Instance(
				const string& appName, 
				const bool bEnableDebugValidationExtension = true,
				const std::vector<const char*>& requiredExtensions = {},
				const std::vector<const char*>& requiredValidationLayers = {});

			Instance(const Instance&) = delete;
			Instance(Instance&&) = delete;

			~Instance();

			Instance& operator=(const Instance&) = delete;
			Instance& operator = (Instance&&) = delete;

			VkInstance& GetVkInstance();
			PhysicalDevice& GetGPU();

		private:
			void QueryAvailableLayers(std::vector<VkLayerProperties>& outLayers) const;
			void QueryAvailableExtensions(std::vector<VkExtensionProperties>& outExtensions) const;

			bool IsLayerAvailable(const char* layerName, const std::vector<VkLayerProperties>& layers) const;
			bool IsExtensionAvailable(const char* extensionName, const std::vector<VkExtensionProperties>& extensions) const;

			void QueryPhysicalDevices(std::vector<VkPhysicalDevice>& outDevices) const;
			void SelectPhysicalDevice(std::vector<VkPhysicalDevice>& devices);

			VkInstance vkInstance{ VK_NULL_HANDLE };
			std::vector<const char*> enabledLayers;
			std::vector<const char*> enabledExtensions;
			VkDebugUtilsMessengerEXT debugUtilsMessenger{ VK_NULL_HANDLE };
			std::unique_ptr<PhysicalDevice> physicalDevice;
		};
	}
}

#endif // BAAL_VK_INSTANCE_H