// MIT License, Copyright (c) 2024 Malik Allen

#include "Instance.h"

#include "../debugging/DebugUtils.h"
#include "../debugging/Error.h"
#include "../devices/PhysicalDevice.h"

#include <stdexcept>

namespace Baal
{
	namespace VK
	{
		Instance::Instance(
			const std::string& appName, 
			const bool bEnableDebugValidationExtension /*= true*/,
			const std::vector<const char*>& requiredExtensions /*= {}*/,
			const std::vector<const char*>& requiredValidationLayers /*= {}*/)
		{
			uint32_t apiVersion; 
			vkEnumerateInstanceVersion(&apiVersion);

			VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
			appInfo.pNext = NULL;
			appInfo.pApplicationName = appName.c_str();
			appInfo.applicationVersion = 1;
			appInfo.pEngineName = "Baal Renderer";
			appInfo.engineVersion = 1;
			appInfo.apiVersion = apiVersion;

			VkInstanceCreateInfo instanceInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
			instanceInfo.pNext = NULL;
			instanceInfo.pApplicationInfo = &appInfo;

			std::vector<VkLayerProperties> availableLayers;
			QueryAvailableLayers(availableLayers);

			std::vector<VkExtensionProperties> availableExts;
			QueryAvailableExtensions(availableExts);

			std::vector<const char*> requestedLayers(requiredValidationLayers);
			std::vector<const char*> requestedExts(requiredExtensions);

			bool bIsDebugUtilsReady = false;
			VkDebugUtilsMessengerCreateInfoEXT debugUtilsInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};

			if(bEnableDebugValidationExtension)
			{
				requestedLayers.push_back("VK_LAYER_KHRONOS_validation");

				const bool bIsDebugUtilsAvaialble = IsExtensionAvailable(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, availableExts);

				if(bIsDebugUtilsAvaialble)
				{
					requestedExts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

					debugUtilsInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
					debugUtilsInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
					debugUtilsInfo.pfnUserCallback = OnDebugMessage;

					instanceInfo.pNext = &debugUtilsInfo;

					bIsDebugUtilsReady = true;
				}
				else
				{
					DEBUG_LOG(LOG::WARNING, "Debug Utils Ext is NOT avaialble!");
				}
			}

			std::vector<const char*> enabledLayers;
			for(auto requiredLayer : requestedLayers)
			{
				if(IsLayerAvailable(requiredLayer, availableLayers))
				{
					enabledLayers.push_back(requiredLayer);
					DEBUG_LOG(LOG::INFO, "Enabled Layer: {}", requiredLayer);
				}
				else
				{
					DEBUG_LOG(LOG::WARNING, "{} is NOT avaialble!", requiredLayer);
				}
			}

			std::vector<const char*> enabledExts;
			for(auto requiredExt : requestedExts)
			{
				if(IsExtensionAvailable(requiredExt, availableExts))
				{
					enabledExts.push_back(requiredExt);
					DEBUG_LOG(LOG::INFO, "Enabled Ext: {}", requiredExt);
				}
				else
				{
					DEBUG_LOG(LOG::WARNING, "{} is NOT avaialble!", requiredExt);
				}
			}

			instanceInfo.enabledExtensionCount = enabledExts.size();
			instanceInfo.ppEnabledExtensionNames = enabledExts.data();
			instanceInfo.enabledLayerCount = enabledLayers.size();
			instanceInfo.ppEnabledLayerNames = enabledLayers.data();

			VK_CHECK(vkCreateInstance(&instanceInfo, nullptr, &vkInstance), "creating vulkan instance");

			if(bIsDebugUtilsReady)
			{
				VK_CHECK(CreateDebugUtilsMessengerEXT(vkInstance, &debugUtilsInfo, nullptr, &debugUtilsMessenger), "creating vulkan debug utils messenger");
			}

			std::vector<VkPhysicalDevice> availableDevices;
			QueryPhysicalDevices(availableDevices);
			SelectPhysicalDevice(availableDevices);
		}

		Instance::~Instance()
		{
			physicalDevice.reset();

			DestroyDebugUtilsMessengerEXT(vkInstance, debugUtilsMessenger, nullptr);
			vkDestroyInstance(vkInstance, nullptr);
		}

		VkInstance& Instance::GetVkInstance()
		{
			return vkInstance;
		}

		PhysicalDevice& Instance::GetGPU()
		{
			return *physicalDevice.get();
		}

		void Instance::QueryAvailableLayers(std::vector<VkLayerProperties>& outLayers) const
		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			outLayers.resize(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, outLayers.data());

			DEBUG_LOG(LOG::INFO, "Available Layer Count: {}", layerCount);

			for(const VkLayerProperties& layer : outLayers)
			{
				DEBUG_LOG(LOG::INFO, "Layer: {}", layer.layerName);
			}
		}

		void Instance::QueryAvailableExtensions(std::vector<VkExtensionProperties>& outExtensions) const
		{
			uint32_t extCount;
			vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
			outExtensions.resize(extCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extCount, outExtensions.data());

			DEBUG_LOG(LOG::INFO, "Available Instance Extension Count: {}", extCount);

			for(const VkExtensionProperties& ext : outExtensions)
			{
				DEBUG_LOG(LOG::INFO, "Ext: {}", ext.extensionName);
			}
		}

		bool Instance::IsLayerAvailable(const char* layerName, const std::vector<VkLayerProperties>& layers) const
		{
			for(auto& availableLayer : layers)
			{
				if (std::strcmp(availableLayer.layerName, layerName) == 0)
				{
					return true;
				}
			}
			return false;
		}

		bool Instance::IsExtensionAvailable(const char* extensionName, const std::vector<VkExtensionProperties>& extensions) const
		{
			for(auto& availableExtension : extensions)
			{
				if (std::strcmp(availableExtension.extensionName, extensionName) == 0)
				{
					return true;
				}
			}
			return false;
		}

		void Instance::QueryPhysicalDevices(std::vector<VkPhysicalDevice>& outDevices) const
		{
			uint32_t count;
			vkEnumeratePhysicalDevices(vkInstance, &count, nullptr);
			outDevices.resize(count);
			vkEnumeratePhysicalDevices(vkInstance, &count, outDevices.data());

			DEBUG_LOG(LOG::INFO, "Found {} physical devices.", count);

			VkPhysicalDeviceProperties properties;
			for(auto& device : outDevices)
			{
				vkGetPhysicalDeviceProperties(device, &properties);

				DEBUG_LOG(LOG::INFO, "\tDevice: \"{}\" \t Vulkan Version: {}", properties.deviceName, properties.apiVersion);
			}
		}

		void Instance::SelectPhysicalDevice(std::vector<VkPhysicalDevice>& devices)
		{
			for(auto& device : devices)
			{
				// This is a chance to look at the QueueFamily Properties for each Physical Device
				// However, for the purpose of getting started we will select the first avaialble device

				physicalDevice = std::make_unique<PhysicalDevice>(device);
				break;
			}
		}
	}
}