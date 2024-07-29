// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_ALLOCATOR_H
#define BAAL_VK_ALLOCATOR_H

#include <vk_mem_alloc.h>

namespace Baal
{
	namespace VK
	{
		class Instance;
		class PhysicalDevice;
		class LogicalDevice;

		class Allocator
		{
		public:
			explicit Allocator(Instance& instance, LogicalDevice& _device);
			Allocator(const Allocator&) = delete;
			Allocator(Allocator&&) = delete;

			~Allocator();

			Allocator& operator=(const Allocator&) = delete;
			Allocator& operator = (Allocator&&) = delete;

			VmaAllocator& GetVmaAllocator() { return vmaAllocator; }

		private:
			VmaAllocator vmaAllocator{ VK_NULL_HANDLE };
		};
	}
}

#endif // !BAAL_VK_ALLOCATOR_H