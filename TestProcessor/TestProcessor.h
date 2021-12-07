#pragma once

/*
- Developer: PDH
- Descriptor: �������� �׽�ƮŬ���� SnowThrad Ŭ������ ���ӵ�
-
*/

#include"SnowThread.h"
#include<memory>
#include<vector>
class CTestProcessor
{
private:
    std::vector<std::unique_ptr<CSnowThread >> vecSnowThread_;
    int32_t sleepMs;

    uint32_t RunThread();
public:
    CTestProcessor(const int32_t runThreadCount);
    virtual ~CTestProcessor();

public:
    void StartTest();
    void SetThreadSleepMs(uint32_t ms) { sleepMs = ms; }
    virtual void TestCode() = 0;
};