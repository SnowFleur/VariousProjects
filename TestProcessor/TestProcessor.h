#pragma once

/*
- Developer: PDH
- Descriptor: 스레드기반 테스트클래스 SnowThrad 클래스가 종속됨
-
*/

#include"../Common/SnowThread.h"
#include"../Common/GlobalUtilFunctions.h"
#include<memory>
#include<vector>

class TestProcessor
{
private:
	std::vector<std::unique_ptr<SnowThread >> threads_;
    int32_t sleepMs_;
    uint32_t RunThread();
public:
    explicit TestProcessor(const int32_t runThreadCount);
    virtual ~TestProcessor();

    DELETE_COPY(TestProcessor);
    DELETE_MOVE(TestProcessor);

public:
    void StartTest();
    void SetThreadSleepMs(const uint32_t ms) { sleepMs_ = ms; }
    virtual void TestCode() = 0;
};