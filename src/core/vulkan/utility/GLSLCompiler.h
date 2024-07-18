// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_GLSL_COMPILER
#define BAAL_GLSL_COMPILER

#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

namespace Baal
{
	namespace VK
	{
		class GLSLCompiler
		{
		public:
			GLSLCompiler() = default;
			GLSLCompiler(const GLSLCompiler&) = delete;
			GLSLCompiler(GLSLCompiler&&) = delete;

			~GLSLCompiler() = default;

			GLSLCompiler& operator=(const GLSLCompiler&) = delete;
			GLSLCompiler& operator = (GLSLCompiler&&) = delete;

			bool CompileToSPIRV(
				VkShaderStageFlagBits _stage, 
				const std::vector<char>& sourceCode,
				const std::string& entryPoint,
				std::vector<std::uint32_t>& spirv,
				std::string& outLog);
		};

	}
}

#endif // BAAL_GLSL_COMPILER