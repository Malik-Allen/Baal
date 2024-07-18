// MIT License, Copyright (c) 2024 Malik Allen

#include "ShaderModule.h"

#include "../devices/LogicalDevice.h"
#include "../utility/GLSLCompiler.h"
#include "../debugging/Error.h"

#include <fstream>
#include <cassert>

namespace Baal
{
	namespace VK
	{
		ShaderModule::ShaderModule(LogicalDevice& _device, VkShaderStageFlagBits _stage, const char* parentDirectory, const char* shaderFileName):
			device(_device),
			stage(_stage)
		{
			std::vector<char> sourceCode = ReadShaderFromFile(parentDirectory, shaderFileName);

			std::string fileName = std::string(shaderFileName);
			DEBUG_LOG(LOG::INFO, "Compiling shader {} ....", fileName);

			GLSLCompiler glslCompiler;
			std::string log;
			const bool bCompileSuccess = glslCompiler.CompileToSPIRV(stage, sourceCode, "main", spirv, log);
			if (!bCompileSuccess)
			{
				DEBUG_LOG(LOG::ERRORLOG, "Failed to compile shader: {}", log);
				assert(false);
			}
			else
			{
				DEBUG_LOG(LOG::INFO, "Successfully compiled shader {}! {}", fileName, log);
			}

			VkShaderModuleCreateInfo shaderModuleInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };

			shaderModuleInfo.codeSize = spirv.size() * sizeof(uint32_t);
			shaderModuleInfo.pCode = spirv.data();

			VK_CHECK(vkCreateShaderModule(device.GetVkDevice(), &shaderModuleInfo, nullptr, &vkShaderModule), "creating shader module");
		}

		ShaderModule::~ShaderModule()
		{
			vkDestroyShaderModule(device.GetVkDevice(), vkShaderModule, nullptr);
		}

		std::vector<char> ShaderModule::ReadShaderFromFile(const char* parentDirectory, const char* shaderFileName)
		{
			std::string shaderFilePath = std::string(parentDirectory) + std::string(shaderFileName);
			std::ifstream file(shaderFilePath, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				DEBUG_LOG(LOG::ERRORLOG, "Failed to open shader file: {}", shaderFilePath);
				assert(false);
			}

			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();
			return buffer;
		}
	}
}