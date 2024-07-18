// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_SHADER_MODULE
#define BAAL_SHADER_MODULE

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;

		class ShaderModule
		{
		public:
			explicit ShaderModule(LogicalDevice& _device, VkShaderStageFlagBits stage, const char* parentDirectory, const char* shaderFileName);
			ShaderModule(const ShaderModule&) = delete;
			ShaderModule(ShaderModule&&) = delete;

			~ShaderModule();

			ShaderModule& operator=(const ShaderModule&) = delete;
			ShaderModule& operator = (ShaderModule&&) = delete;

		private:
			VkShaderModule vkShaderModule;
			LogicalDevice& device;
			uint32_t id;

			// Compile Source Code
			std::vector<uint32_t> spirv;

			std::vector<char> ReadShaderFromFile(const char* parentDirectory, const char* shaderFileName);
		};
	}
}

#endif // BAAL_SHADER_MODULE