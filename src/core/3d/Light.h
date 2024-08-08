// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_LIGHT_H
#define BAAL_VK_LIGHT_H

#include <Mjolnir.h>

namespace Baal
{
	namespace VK
	{
		// PointLight
			// Vector3f origin;
		// SpotLight
			// Some cone like structure
		// DirectionalLight
			// Ray3Df ray;

		class Light
		{
		public:
			Light();
			Light(const Light&) = delete;
			Light(Light&&) = delete;

			~Light();

			Light& operator=(const Light&) = delete;
			Light& operator = (Light&&) = delete;

		private:
			Color color;
		};
	}
}

#endif // !BAAL_VK_LIGHT_H