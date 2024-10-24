#pragma once

#include "MemoryPoolTest.h"
#include "TestProcessor.h"

class MemoryPoolTest : public TestProcessor
{
private:

public:
	explicit MemoryPoolTest(int32_t runThreadCount)
		: TestProcessor(runThreadCount)
	{
	}


	void TestCode() override;
};


