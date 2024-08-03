// MIT License, Copyright (c) 2024 Malik Allen

#include "Texture.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/devices/LogicalDevice.h"
#include "../src/core/vulkan/resource/Image.h"

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
			VkDeviceSize imageSize = width * height * channels;	// Channels represent the RGBA

			if (pixels == nullptr) 
			{
				DEBUG_LOG(LOG::ERRORLOG, "Failed to load Texture: {}", texture.fileName);
				return;
			}

			DEBUG_LOG(LOG::INFO, "Successfully loaded Texture: {} | [{}x{}] {} channel(s)", texture.fileName, width, height, channels);
			

			image = std::make_unique<Image>(device, static_cast<uint32_t>(width), static_cast<uint32_t>(height), texture.type, texture.format, texture.tiling, texture.usage, texture.samples, imageSize, pixels);

			stbi_image_free(pixels);


		}

		TextureInstance::~TextureInstance()
		{
			image.reset();
		}
	}
}