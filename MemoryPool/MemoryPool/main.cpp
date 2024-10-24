
#include <iostream>

#include "MemoryPool.h"

int main()
{

	for (int32_t i = 0; i < 1000; ++i)
	{
		int* p = (int*)(g_memory_pool.Allocate(4));
		*p = i;
		std::cout << *p << "\n";
		g_memory_pool.Deallocate(p);
	}


}
