// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_TEXTURE2D_H
#define BAAL_VK_TEXTURE2D_H

#include <memory>


namespace Baal
{
	namespace VK
	{
		class Image;

		class Texture2D
		{
		public:
			explicit Texture2D(const char* parentDirectory, const char* textureFileName);
			Texture2D(const Texture2D&) = delete;
			Texture2D(Texture2D&&) noexcept = delete;

			~Texture2D();

			Texture2D& operator=(const Texture2D&) = delete;
			Texture2D& operator = (Texture2D&&) = delete;
		private:
			std::unique_ptr<Image> image;
		};
	}
}

#endif // !BAAL_VK_TEXTURE2D_H