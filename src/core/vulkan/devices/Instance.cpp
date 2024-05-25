// MIT License, Copyright (c) 2019 Malik Allen

#include "Instance.h"

#include "PhysicalDevice.h"

#include <stdexcept>

namespace Baal
{
	namespace VK
	{
		Instance::Instance(
			const string& appName)
		{
			uint32_t apiVersion; 
			vkEnumerateInstanceVersion(&apiVersion);

			VkApplicationInfo appInfo;
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pNext = NULL;
			appInfo.pApplicationName = appName.c_str();
			appInfo.applicationVersion = 1;
			appInfo.pEngineName = "Baal Renderer";
			appInfo.engineVersion = 1;
			appInfo.apiVersion = apiVersion;

			VkInstanceCreateInfo instanceInfo;
			instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceInfo.pNext = NULL;
			instanceInfo.pApplicationInfo = &appInfo;
			instanceInfo.enabledExtensionCount = 0;
			instanceInfo.ppEnabledExtensionNames = {};
			instanceInfo.enabledLayerCount = 0;
			instanceInfo.ppEnabledLayerNames = {};

			VkResult result;
			result = vkCreateInstance(&instanceInfo, NULL, &vkInstance);

			if(result != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create vulkan instance!");
			}
		}

		Instance::~Instance()
		{
			vkDestroyInstance(vkInstance, NULL);
		}

		const VkInstance& Instance::GetVkInstance() const
		{
			return vkInstance;
		}
	}
}