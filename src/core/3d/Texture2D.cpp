// MIT License, Copyright (c) 2024 Malik Allen

#include "Texture2D.h"

#include "../src/core/vulkan/debugging/Error.h"
#include "../src/core/vulkan/resource/Image.h"

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "../src/utility/stb/stb_image.h"

namespace Baal
{
	namespace VK
	{
		Texture2D::Texture2D(const char* parentDirectory, const char* textureFileName)
		{
			std::string filePath = std::string(parentDirectory) + std::string(textureFileName);

			DEBUG_LOG(LOG::INFO, "Loading 2D Texture file: {}", textureFileName);

			int width = 0;
			int height = 0;
			int channels = 0;

			stbi_uc* pixels = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
			VkDeviceSize imageSize = width * height * 4;	// RGBA for every pixel of the image (A byte for each one)

			if (pixels == nullptr) 
			{
				DEBUG_LOG(LOG::ERRORLOG, "Failed to load 2D Texture file: {}", textureFileName);
				return;
			}

			DEBUG_LOG(LOG::INFO, "Successfully loaded 2D Texture file: {}", textureFileName);
			stbi_image_free(pixels);

			// image = std::make_unique<Image>(GetAllocator(), 400, 600, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT, VK_SAMPLE_COUNT_1_BIT, 400 * 600 * 4, nullptr);
		}

		Texture2D::~Texture2D()
		{
			image.reset();
		}
	}
}