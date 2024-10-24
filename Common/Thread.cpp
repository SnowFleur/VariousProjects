#include"Thread.h"

Thread::~Thread()noexcept 
{
    if (threadHandle_ != INVALID_HANDLE_VALUE)
    {
        //PDH- 커널 객체 반환은 선택이 아닌 필수다.
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

/*여러 가지 기능들 */
void Thread::SetThreadPriority(const int32_t priority) 
{
    if (::SetThreadPriority(threadHandle_, priority) == 0)
    {
        std::cout << "Can't Thread Priority: " << WSAGetLastError() << "\n";
    }
}

int32_t Thread::GetThreadPriority() const 
{
    if (threadHandle_ == INVALID_HANDLE_VALUE)return -1;
    return ::GetThreadPriority(threadHandle_);
}

void Thread::ContextSwitch() {}

void Thread::WaitForThread()
{
    WaitForSingleObject(threadHandle_, INFINITE);
}

bool Thread::StartThread() 
{
    if (isStartThread_ == false)
    {
		ResumeThread(threadHandle_);
		isStartThread_ = true;
		isRunning_ = true;
		return true;
	}

    return false;
}

