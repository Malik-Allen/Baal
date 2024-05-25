// MIT License, Copyright (c) 2019 Malik Allen

#ifndef BAAL_VK_PHYSICALDEVICE_H
#define BAAL_VK_PHYSICALDEVICE_H

namespace Baal
{
	namespace VK
	{
		class PhysicalDevice
		{
		public:
			PhysicalDevice() = default;
			PhysicalDevice(const PhysicalDevice&) = delete;
			PhysicalDevice(PhysicalDevice&&) = delete;

			~PhysicalDevice() = default;

			PhysicalDevice& operator=(const PhysicalDevice&) = delete;
			PhysicalDevice& operator = (PhysicalDevice&&) = delete;
		};
	}
}

#endif //BAAL_VK_PHYSICALDEVICE_H