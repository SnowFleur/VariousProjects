
#include <iostream>
#include"MemoryPool.h"


int main()
{
	MultiSizeMemoryPool b;


	int a = 5;

	auto p = b.Allocate(150);
	auto p2 = b.Allocate(4);

	b.Deallocate(p);

}