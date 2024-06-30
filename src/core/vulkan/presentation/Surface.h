// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_SURFACE_H
#define BAAL_VK_SURFACE_H

#include <vulkan/vulkan_core.h>

class GLFWwindow;

namespace Baal
{
	namespace VK
	{
		class Instance;

		class Surface 
		{
		public:
			explicit Surface(Instance& _instance, GLFWwindow* glfwWindow);
			Surface(const Surface&) = delete;
			Surface(Surface&&) = delete;

			~Surface();

			Surface& operator=(const Surface&) = delete;
			Surface& operator = (Surface&&) = delete;

			VkSurfaceKHR& GetVkSurface() { return vkSurface; }

		private:
			VkSurfaceKHR vkSurface{VK_NULL_HANDLE};
			Instance& instance;
		};
	}
}

#endif // !BAAL_VK_SURFACE_H
