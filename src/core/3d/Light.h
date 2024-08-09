// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_LIGHT_H
#define BAAL_VK_LIGHT_H

#include <Mjolnir.h>

namespace Baal
{
	namespace VK
	{
		struct alignas(16) PointLight
		{
			Vector3f positon;
			Color color = Color(255, 255, 255);
		};
	}
}

#endif // !BAAL_VK_LIGHT_H