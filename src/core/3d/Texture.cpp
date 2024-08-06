// MIT License, Copyright (c) 2024 Malik Allen

#include "Texture.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/resource/Image.h"
#include "../src/core/vulkan/resource/Buffer.h"
#include "../src/core/vulkan/commands/CommandBuffer.h"

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "../src/utility/stb/stb_image.h"

namespace Baal
{
	namespace VK
	{
		TextureInstance::TextureInstance(LogicalDevice& device, const Texture texture)
		{
			std::string filePath = std::string(texture.parentDirectory) + std::string(texture.fileName);

			DEBUG_LOG(LOG::INFO, "Loading Texture: {}", texture.fileName);

			int width = 0;
			int height = 0;
			int channels = 0;

			stbi_uc* pixels = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
			VkDeviceSize imageSize = width * height * 4;	// Channels represent the RGBA 4 bytes of data per pixel

			if (pixels == nullptr) 
			{
				DEBUG_LOG(LOG::ERRORLOG, "Failed to load Texture: {}", texture.fileName);
				return;
			}

			DEBUG_LOG(LOG::INFO, "Successfully loaded Texture: {} | [{}x{}] {} channel(s)", texture.fileName, width, height, channels);

			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.baseArrayLayer = 0;
			subresourceRange.layerCount = 1;

			image = std::make_unique<Image>(
				device, 
				static_cast<uint32_t>(width), 
				static_cast<uint32_t>(height), 
				texture.type, 
				VK_FORMAT_R8G8B8A8_SRGB, 
				VK_IMAGE_TILING_OPTIMAL, 
				VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
				VK_SAMPLE_COUNT_1_BIT, 
				VK_IMAGE_VIEW_TYPE_2D, 
				subresourceRange);

			Buffer stagingBuffer = Buffer::CreateStagingBuffer(device.GetAllocator(), imageSize, pixels);
			Image::TransitionToLayout(*image.get(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, VK_ACCESS_TRANSFER_WRITE_BIT, subresourceRange);
			device.CopyBufferToImage(stagingBuffer, *image.get(), width, height);
			Image::TransitionToLayout(*image.get(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, subresourceRange);

			stbi_image_free(pixels);
		}

		TextureInstance::~TextureInstance()
		{
			image.reset();
		}
	}
}