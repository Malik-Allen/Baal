// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_Texture_H
#define BAAL_VK_Texture_H

#include <vulkan/vulkan_core.h>
#include <memory>

namespace Baal
{
	namespace VK
	{
		class LogicalDevice;
		class Image;

		struct Texture
		{
			const char* parentDirectory;
			const char* fileName;
			VkImageType type;
			VkFormat format; 
			VkImageUsageFlags usage;
			VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
			VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
		};

		class TextureInstance
		{
			friend class TextureHandler;
			std::unique_ptr<Image> image;
			uint32_t id = 0;
		public:
			explicit TextureInstance(LogicalDevice& device, const Texture texture);
			TextureInstance(const TextureInstance&) = delete;
			TextureInstance(TextureInstance&&) noexcept = delete;

			~TextureInstance();

			TextureInstance& operator=(const TextureInstance&) = delete;
			TextureInstance& operator = (TextureInstance&&) = delete;
		};
	}
}

#endif // !BAAL_VK_Texture_H