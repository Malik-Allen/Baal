// MIT License, Copyright (c) 2019 Malik Allen

#include "Instance.h"

#include "../debugging/DebugUtils.h"

#include <stdexcept>

namespace Baal
{
	namespace VK
	{
		Instance::Instance(
			const string& appName, 
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

			VkResult result;
			result = vkCreateInstance(&instanceInfo, nullptr, &vkInstance);

			if(result != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create vulkan instance!");
			}

			if(bIsDebugUtilsReady)
			{
				result = CreateDebugUtilsMessengerEXT(vkInstance, &debugUtilsInfo, nullptr, &debugUtilsMessenger);

				if(result != VK_SUCCESS)
				{
					throw std::runtime_error("Failed to create vulkan debug utils messenger!");
				}
			}
		}

		Instance::~Instance()
		{
			DestroyDebugUtilsMessengerEXT(vkInstance, debugUtilsMessenger, nullptr);
			vkDestroyInstance(vkInstance, nullptr);
		}

		const VkInstance& Instance::GetVkInstance() const
		{
			return vkInstance;
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

			DEBUG_LOG(LOG::INFO, "Available Extension Count: {}", extCount);

			for(const VkExtensionProperties& ext : outExtensions)
			{
				DEBUG_LOG(LOG::INFO, "Ext: {}", ext.extensionName);
			}
		}

		bool Instance::IsLayerAvailable(const char* layerName, const std::vector<VkLayerProperties>& layers) const
		{
			for(auto& availableLayer : layers)
			{
				if (strcmp(availableLayer.layerName, layerName) == 0)
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
				if (strcmp(availableExtension.extensionName, extensionName) == 0)
				{
					return true;
				}
			}
			return false;
		}
	}
}