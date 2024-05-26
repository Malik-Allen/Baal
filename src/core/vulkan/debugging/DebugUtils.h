// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_VK_DEBUG_UTILS_H
#define BAAL_VK_DEBUG_UTILS_H

#include "../src/utility/DebugLog.h"

#include <vulkan/vulkan_core.h>

// Derived-from: https://docs.vulkan.org/samples/latest/samples/extensions/debug_utils/README.html

namespace Baal
{
	namespace VK
	{
		VkResult CreateDebugUtilsMessengerEXT( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger )
		{
		    auto func = ( PFN_vkCreateDebugUtilsMessengerEXT )vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" );
		    if( func != nullptr )
		    {
		        return func( instance, pCreateInfo, pAllocator, pDebugMessenger );
		    }
		    else
		    {
		        return VK_ERROR_EXTENSION_NOT_PRESENT;
		    }
		}
		
		void DestroyDebugUtilsMessengerEXT( VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator )
		{
		    auto func = ( PFN_vkDestroyDebugUtilsMessengerEXT )vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" );
		    if( func != nullptr )
		    {
		        func( instance, debugMessenger, pAllocator );
		    }
		}

		VKAPI_ATTR VkBool32 VKAPI_CALL OnDebugMessage(
			VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT type,
			const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
			void *userData)
		{
			if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			{
				DEBUG_LOG(LOG::WARNING, "{} - {}: {}", callbackData->messageIdNumber, callbackData->pMessageIdName, callbackData->pMessage);
			}
			else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			{
				DEBUG_LOG(LOG::ERRORLOG, "{} - {}: {}", callbackData->messageIdNumber, callbackData->pMessageIdName, callbackData->pMessage);
			}
			return VK_FALSE;
		}
	}
}

#endif // BAAL_VK_DEBUG_UTILS_H