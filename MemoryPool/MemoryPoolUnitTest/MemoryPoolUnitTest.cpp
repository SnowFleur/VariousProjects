#include "pch.h"
#include<iostream>
#include "CppUnitTest.h"

#include"../MemoryPool/MemoryPool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MemoryPoolUnitTest
{
	TEST_CLASS(MemoryPoolUnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			for (int32_t i = 0; i < 1000; ++i)
			{
				int* p = (int*)(g_memory_pool.Allocate(4));
				*p = i;
				std::cout << *p << "\n";
				g_memory_pool.Deallocate(p);
			}

		}
	};
}
