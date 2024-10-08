// MIT License, Copyright (c) 2024 Malik Allen

#include "GraphicsPipeline.h"

#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/pipeline/ShaderModule.h"
#include "../src/core/vulkan/pipeline/RenderPass.h"
#include "../src/core/vulkan/descriptors/DescriptorSetLayout.h"
#include "../src/core/3d/Mesh.h"
#include "../src/core/vulkan/debugging/Error.h"

namespace Baal
{
	namespace VK
	{
		GraphicsPipeline::GraphicsPipeline(
			LogicalDevice& _device,
			std::vector<ShaderInfo>& shaderInfo,
			RenderPass& renderPass,
			DescriptorSetLayout& descriptorSetLayout,
			VkPipelineRasterizationStateCreateInfo& rasterizerInfo,
			std::vector<VkPushConstantRange>& pushConstants,
			const uint32_t width,
			const uint32_t height)
			: device(_device)
		{
			for (size_t i = 0; i < shaderInfo.size(); ++i)
			{
				shaderStages.push_back(ShaderModule(device, shaderInfo[i]));
			}

			std::vector<VkPipelineShaderStageCreateInfo> pipelineStages(shaderStages.size());

			VkPipelineShaderStageCreateInfo shaderStageInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
			for (size_t i = 0; i < shaderStages.size(); ++i)
			{
				shaderStageInfo.stage = shaderStages[i].GetStage();
				shaderStageInfo.module = shaderStages[i].GetVkShaderModule();
				shaderStageInfo.pName = "main";

				pipelineStages[i] = shaderStageInfo;
			}

			// Dynamic states are used to represent states can be changed without needing to recreate the graphics pipeline, very useful.
			std::vector<VkDynamicState> dynamicStates = {
				VK_DYNAMIC_STATE_VIEWPORT,	// Will provide starting values in this constructor, will update the values through command buffers
				VK_DYNAMIC_STATE_SCISSOR // Will provide starting values in this constructor, will update the values through command buffers
			};
			
			VkPipelineDynamicStateCreateInfo dynamicState = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
			dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
			dynamicState.pDynamicStates = dynamicStates.data();
			
			VkVertexInputBindingDescription bindingDescription;
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			std::vector<VkVertexInputAttributeDescription> attributeDescriptions = GetVertexAttributes();

			VkPipelineVertexInputStateCreateInfo vertexInput = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
			vertexInput.vertexBindingDescriptionCount = 1;
			vertexInput.pVertexBindingDescriptions = &bindingDescription; 
			vertexInput.vertexAttributeDescriptionCount = attributeDescriptions.size();
			vertexInput.pVertexAttributeDescriptions = attributeDescriptions.data();

			VkPipelineInputAssemblyStateCreateInfo inputAssembly = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;
			
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(width);
			viewport.height = static_cast<float>(height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = VkExtent2D(width, height);

			VkPipelineViewportStateCreateInfo viewportState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			VkPipelineMultisampleStateCreateInfo multisampling = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampling.minSampleShading = 1.0f;
			multisampling.pSampleMask = nullptr;
			multisampling.alphaToCoverageEnable = VK_FALSE;
			multisampling.alphaToOneEnable = VK_FALSE;

			VkPipelineColorBlendAttachmentState colorBlendAttachment{};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

			VkPipelineColorBlendStateCreateInfo colorBlending = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;			

			VkPipelineDepthStencilStateCreateInfo depthStencil = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
			depthStencil.depthTestEnable = VK_TRUE;
			depthStencil.depthWriteEnable = VK_TRUE;
			depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
			depthStencil.depthBoundsTestEnable = VK_FALSE;
			depthStencil.minDepthBounds = 0.0f;
			depthStencil.maxDepthBounds = 1.0f;
			depthStencil.stencilTestEnable = VK_FALSE;
			depthStencil.front = {};
			depthStencil.back = {};

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
			pipelineLayoutInfo.setLayoutCount = 1;
			pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout.GetVkDescriptorSetLayout();
			pipelineLayoutInfo.pushConstantRangeCount = pushConstants.size();
			pipelineLayoutInfo.pPushConstantRanges = pushConstants.data();

			VK_CHECK(vkCreatePipelineLayout(device.GetVkDevice(), &pipelineLayoutInfo, nullptr, &layout), "creating graphics pipeline layout");

			VkGraphicsPipelineCreateInfo pipelineInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
			pipelineInfo.stageCount = pipelineStages.size();
			pipelineInfo.pStages = pipelineStages.data();
			pipelineInfo.pDynamicState = &dynamicState;
			pipelineInfo.pVertexInputState = &vertexInput;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizerInfo;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.pDepthStencilState = &depthStencil;
			pipelineInfo.layout = layout;
			pipelineInfo.renderPass = renderPass.GetVkRenderPass();
			pipelineInfo.subpass = 0;
			
			VK_CHECK(vkCreateGraphicsPipelines(device.GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline), "creating graphics pipeline");
		}

		GraphicsPipeline::~GraphicsPipeline()
		{
			vkDestroyPipeline(device.GetVkDevice(), pipeline, nullptr);
			vkDestroyPipelineLayout(device.GetVkDevice(), layout, nullptr);
			shaderStages.clear();
		}

		std::vector<VkVertexInputAttributeDescription> GraphicsPipeline::GetVertexAttributes() const
		{
			std::vector<VkVertexInputAttributeDescription> vertexAttributes;
			vertexAttributes.push_back(VkVertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos)));
			vertexAttributes.push_back(VkVertexInputAttributeDescription(1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm)));
			vertexAttributes.push_back(VkVertexInputAttributeDescription(2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoords)));
			vertexAttributes.push_back(VkVertexInputAttributeDescription(3, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)));
			return vertexAttributes;
		}
	}
}