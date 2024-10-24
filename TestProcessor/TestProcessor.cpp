#include"TestProcessor.h"


TestProcessor::TestProcessor(const int32_t runThreadCount) :
	sleepMs_(10)
{
	threads_.reserve(runThreadCount);

	for (int32_t i = 0; i < runThreadCount; ++i)
	{
		ThreadID threadId = (i + 1) * 1000;
		std::unique_ptr<SnowThread> pSnowThread = std::make_unique<SnowThread>(false, &TestProcessor::RunThread, this);
		pSnowThread->SetThreadID(threadId);
		threads_.emplace_back(std::move(pSnowThread));
	}
}

TestProcessor::~TestProcessor()
{
}

uint32_t TestProcessor::RunThread()
{
	while (true)
	{
		Sleep(sleepMs_);
		TestCode();
	}
	return 0;
}


void TestProcessor::StartTest()
{
	for (auto& thread : threads_)
	{
		thread->StartThread();
	}

	for (auto& thread : threads_)
	{
		thread->WaitForThread();
	}

}

