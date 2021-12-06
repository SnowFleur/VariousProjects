#include"TestProcessor.h"

CTestProcessor::CTestProcessor(const int32_t runThreadCount)
{
    vecSnowThread_.reserve(runThreadCount);

    for (int32_t i = 0; i < runThreadCount; ++i)
    {
        ThreadID threadId = (i + 1) * 1000;
        std::unique_ptr<CSnowThread> pSnowThread = std::make_unique<CSnowThread>(false, &CTestProcessor::RunThread, this);
        pSnowThread->SetThreadID(threadId);
        vecSnowThread_.emplace_back(std::move(pSnowThread));
    }
}

CTestProcessor::~CTestProcessor()
{
}

uint32_t CTestProcessor::RunThread()
{
    while (true)
    {
        Sleep(100);
        TestCode();
    }
    return 0;
}


void CTestProcessor::StartTest()
{
    for (auto& thread : vecSnowThread_)
    {
        thread->StartThread();
    }
}

