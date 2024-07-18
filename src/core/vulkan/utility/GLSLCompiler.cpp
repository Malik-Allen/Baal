// MIT License, Copyright (c) 2024 Malik Allen

#include "GLSLCompiler.h"

#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <SPIRV/GlslangToSpv.h>

namespace Baal
{
	namespace VK
	{
		inline EShLanguage FindShaderLanguage(VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
			case VK_SHADER_STAGE_VERTEX_BIT:
				return EShLangVertex;

			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
				return EShLangTessControl;

			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
				return EShLangTessEvaluation;

			case VK_SHADER_STAGE_GEOMETRY_BIT:
				return EShLangGeometry;

			case VK_SHADER_STAGE_FRAGMENT_BIT:
				return EShLangFragment;

			case VK_SHADER_STAGE_COMPUTE_BIT:
				return EShLangCompute;

			case VK_SHADER_STAGE_RAYGEN_BIT_KHR:
				return EShLangRayGen;

			case VK_SHADER_STAGE_ANY_HIT_BIT_KHR:
				return EShLangAnyHit;

			case VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR:
				return EShLangClosestHit;

			case VK_SHADER_STAGE_MISS_BIT_KHR:
				return EShLangMiss;

			case VK_SHADER_STAGE_INTERSECTION_BIT_KHR:
				return EShLangIntersect;

			case VK_SHADER_STAGE_CALLABLE_BIT_KHR:
				return EShLangCallable;

			case VK_SHADER_STAGE_MESH_BIT_EXT:
				return EShLangMesh;

			case VK_SHADER_STAGE_TASK_BIT_EXT:
				return EShLangTask;

			default:
				return EShLangVertex;
			}
		}

		bool GLSLCompiler::CompileToSPIRV(VkShaderStageFlagBits _stage, const std::vector<char>& sourceCode, const std::string& entryPoint, std::vector<std::uint32_t>& spirv, std::string& outLog)
		{
			glslang::InitializeProcess();

			const EShLanguage stage = FindShaderLanguage(_stage);

			std::string source = std::string(sourceCode.begin(), sourceCode.end());

			glslang::TShader shader(stage);
			const char* shaderStrings[1];
			shaderStrings[0] = source.c_str();
			shader.setStrings(shaderStrings, 1);

			shader.setEntryPoint(entryPoint.c_str());
			shader.setSourceEntryPoint(entryPoint.c_str());
			
			EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
			if (!shader.parse(GetDefaultResources(), 100, false, messages)) {
				outLog = std::string(shader.getInfoLog()) + "\n" + std::string(shader.getInfoDebugLog());
				return false;
			}

			glslang::TProgram program;
			program.addShader(&shader);

			if (!program.link(messages)) {
				outLog = std::string(program.getInfoLog()) + "\n" + std::string(program.getInfoDebugLog());
				return false;
			}

			if (shader.getInfoLog())
			{
				outLog += std::string(shader.getInfoLog()) + "\t" + std::string(shader.getInfoDebugLog()) + "\t";
			}

			if (program.getInfoLog())
			{
				outLog += std::string(program.getInfoLog()) + "\t" + std::string(program.getInfoDebugLog());
			}

			glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);

			glslang::FinalizeProcess();
			return true;
		}
}
}