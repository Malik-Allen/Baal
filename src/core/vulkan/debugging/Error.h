// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_VK_ERROR_H
#define BAAL_VK_ERROR_H

#include "../src/utility/DebugLog.h"

#include <string>
#include <cassert>
#include <vulkan/vk_enum_string_helper.h>

#define VK_CHECK(funcResult, context)	\
		{	\
			VkResult result = funcResult;	\
			if (result != VK_SUCCESS)	\
			{	\
				std::string output = std::string(string_VkResult(result));	\
				DEBUG_LOG(LOG::ERRORLOG, "Failed {}, Error: {}", context, output);	\
				assert(false);	\
			}	\
		}	\

#endif