// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_SHADER_MODULE_H
#define BAAL_SHADER_MODULE_H

#include <vulkan/vulkan_core.h>
#include <vector>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;

		struct ShaderInfo
		{
			VkShaderStageFlagBits stage;
			const char* parentDirectory; 
			const char* shaderFileName;
		};
		
		// Responsible for loading and compiling shader files
		// TODO: Add logic to save and read a spirv file if one exists for the shader, instead of recompiling each time. For now this is enough until compile times take up too much time

		class ShaderModule
		{
		public:
			explicit ShaderModule(LogicalDevice& _device, const ShaderInfo& shaderInfo);
			ShaderModule(const ShaderModule&) = delete;
			ShaderModule(ShaderModule&& other) noexcept;

			~ShaderModule();

			ShaderModule& operator=(const ShaderModule&) = delete;
			ShaderModule& operator = (ShaderModule&&) = delete;

			VkShaderModule GetVkShaderModule() const { return vkShaderModule; }
			VkShaderStageFlagBits GetStage() const { return stage; }

		private:
			VkShaderModule vkShaderModule{VK_NULL_HANDLE};
			LogicalDevice& device;
			VkShaderStageFlagBits stage;
			// Compile Source Code
			std::vector<uint32_t> spirv;

			std::vector<char> ReadShaderFromFile(const char* parentDirectory, const char* shaderFileName);
		};
	}
}

#endif // !BAAL_SHADER_MODULE_H