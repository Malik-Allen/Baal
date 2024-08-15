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
		
		struct DirectionalLight
		{
			alignas(16) Color color = Color(255, 255, 255);
			alignas(16) Vector3f direction = Vector3f::ForwardVector;
			float intensity = 1.0f;
		};

		struct PointLight
		{
			alignas(16) Color color = Color(255, 255, 255);
			alignas(16) Vector3f positon;
			float intensity = 1.0f;
			float attenuation  = 1.0f;
		};

		struct SpotLight
		{
			alignas(16) Color color = Color(255, 255, 255);
			alignas(16) Vector3f direction = Vector3f::ForwardVector;
			float intensity = 1.0f;
			alignas(16) Vector3f positon;
			float attenuation  = 1.0f;
			// Cone Angle (Inner/ Outer)
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