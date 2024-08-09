// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_MATERIAL_H
#define BAAL_VK_MATERIAL_H

#include <Mjolnir.h>

namespace Baal
{
	namespace VK
	{
		struct Material
		{
			Vector3f ambient;
			Vector3f diffuse;
			Vector3f specular;
		};
	}
}

#endif //!BAAL_VK_MATERIAL_H