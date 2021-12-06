#pragma once

/*
- Developer: PDH
- Descriptor: 스레드기반 테스트클래스 SnowThrad 클래스가 종속됨
-
*/

#include"SnowThread.h"
#include<memory>
#include<vector>
class CTestProcessor
{
private:
    std::vector<std::unique_ptr<CSnowThread >> vecSnowThread_;
    uint32_t RunThread();
public:
    CTestProcessor(const int32_t runThreadCount);
    virtual ~CTestProcessor();

public:
    void StartTest();
    virtual void TestCode() = 0;
};