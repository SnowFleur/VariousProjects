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
    uint32_t RunThread();
public:
    CTestProcessor(const int32_t runThreadCount);
    virtual ~CTestProcessor();

public:
    void StartTest();
    virtual void TestCode() = 0;
};