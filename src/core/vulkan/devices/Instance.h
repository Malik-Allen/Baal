// MIT License, Copyright (c) 2019 Malik Allen

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
				const string& appName
			);

			Instance(const Instance&) = delete;
			Instance(Instance&&) = delete;

			~Instance();

			Instance& operator=(const Instance&) = delete;
			Instance& operator = (Instance&&) = delete;

			const VkInstance& GetVkInstance() const;

		private:
			VkInstance vkInstance;
			vector<std::unique_ptr<PhysicalDevice>> gpus;
			vector<const char*> enabledExtensions;
		};
	}
}

#endif // BAAL_VK_INSTANCE_H