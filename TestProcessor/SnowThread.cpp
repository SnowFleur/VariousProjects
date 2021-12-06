#include"SnowThread.h"

CSnowThread::~CSnowThread()noexcept 
{
    if (threadHandle_ != INVALID_HANDLE_VALUE)
    {
        //PDH- Ŀ�� ��ü ��ȯ�� ������ �ƴ� �ʼ���.
        if (CloseHandle(threadHandle_) == TRUE)
        {
            threadHandle_ = NULL;
#ifdef  PRINT_THREAD_CLOSE_LOG
            PRINT_INFO_LOG("Sucess Close Thread Handle",
                "ThreadID: ", threadID_, "\n");
#endif 
        }
        else
        {
            PRINT_ERROR_LOG("Thread Close Handle", WSAGetLastError(),"\n");
        }
    }
}

/*���� ���� ��ɵ� */
void CSnowThread::SetThreadPriority(const int32_t priority) 
{
    if (::SetThreadPriority(threadHandle_, priority) == 0)
    {
        std::cout << "Can't Thread Priority: " << WSAGetLastError() << "\n";
    }
}

int32_t CSnowThread::GetThreadPriority() const 
{
    if (threadHandle_ == INVALID_HANDLE_VALUE)return -1;
    return ::GetThreadPriority(threadHandle_);
}

void CSnowThread::ContextSwitch() {}

void CSnowThread::WaitForThread()
{
    WaitForSingleObject(threadHandle_, INFINITE);
}

bool CSnowThread::StartThread() 
{
    if (isStartThread_ == false)
    {
        ResumeThread(threadHandle_);
        isStartThread_ = true;
        return true;
    }

    return false;
}

