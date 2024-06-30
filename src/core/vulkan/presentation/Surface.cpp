// MIT License, Copyright (c) 2024 Malik Allen

#include "Surface.h"

#include "../src/core/vulkan/initialization/Instance.h"
#include "../src/core/vulkan/debugging/Error.h"

#include <GLFW/glfw3.h>

namespace Baal
{
	namespace VK
	{
		Surface::Surface(Instance& _instance, GLFWwindow* glfwWindow):
			instance(_instance)
		{
			VK_CHECK(glfwCreateWindowSurface(instance.GetVkInstance(), glfwWindow, nullptr, &vkSurface), "creating window surface");
		}

		Surface::~Surface()
		{
			vkDestroySurfaceKHR(instance.GetVkInstance(), vkSurface, nullptr);
		}
	}
}