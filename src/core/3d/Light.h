// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_LIGHT_H
#define BAAL_VK_LIGHT_H

#include <Mjolnir.h>

#ifndef BAAL_MAX_LIGHTS
#define BAAL_MAX_LIGHTS 8
#endif // !BAAL_MAX_LIGHTS

namespace Baal
{
	namespace VK
	{
		class Buffer;
		
		struct alignas(16) DirectionalLight
		{
			Color color = Color(255, 255, 255);
			float intensity = 1.0f;
			Vector3f direction;
		};

		struct alignas(16) PointLight
		{
			Color color = Color(255, 255, 255);
			float intensity = 1.0f;
			float attenuation  = 1.0f;
			Vector3f positon;
		};

		struct alignas(16) SpotLight
		{
			Color color = Color(255, 255, 255);
			float intensity = 1.0f;
			float attenuation  = 1.0f;
			// Cone Angle (Inner/ Outer)
			Vector3f positon;
			Vector3f direction;
		};

		template<typename T>
		struct LightSource
		{
			T light;
			std::unique_ptr<Buffer> buffer = nullptr;
		};

		template<typename T>
		struct LightSourceArray
		{
			std::array<T, BAAL_MAX_LIGHTS> lights;
			std::unique_ptr<Buffer> buffer = nullptr;
		};
	}
}

typedef Baal::VK::LightSource<Baal::VK::DirectionalLight> DirectionalLightSource;
typedef Baal::VK::LightSourceArray<Baal::VK::PointLight> PointLightSourceArray;
typedef Baal::VK::LightSourceArray<Baal::VK::SpotLight> SpotLightSourceArray;


#endif // !BAAL_VK_LIGHT_H