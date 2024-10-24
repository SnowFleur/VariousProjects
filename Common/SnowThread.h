#pragma once
/*
- Descriptor: Thread�� ĸ��ȭ �Ͽ� �������� ����� ������ Ŭ����
-
*/

#include<functional>
#include"../Common/WindowsHeader.h"
#include"../Common/LogCollector.h"

#define PRINT_THREAD_CLOSE_LOG

#ifdef _WIN64
using ThreadID = uint32_t;
#else
using ThreadID = DWORD32;
#endif // _WIN64

class SnowThread
{
protected:
	using CallBackThreadFtn = std::function<uint32_t()>;
private:
	HANDLE					threadHandle_;
	bool						isStartThread_;
	bool						isRunning_;
	CallBackThreadFtn		fRunThreadFunction_;
	ThreadID					threadId_;

	static  uint32_t __stdcall OnInvoke(LPVOID arg)
	{
		if (reinterpret_cast<SnowThread*>(arg) == nullptr) return 0;
		uint32_t returnValue = reinterpret_cast<SnowThread*>(arg)->fRunThreadFunction_();
#ifdef  PRINT_THREAD_CLOSE_LOG
		PRINT_INFO_LOG("End Thread\n");
#endif 
		//ex�� CloseHandle�� ����ؾ���
		_endthreadex(0);
		return returnValue;
	}

	template<class _Ty, class... _Args>
	void Run(_Ty&& ftn, _Args&&... args)
	{
		fRunThreadFunction_ = std::bind(std::forward<_Ty>(ftn), std::forward<_Args>(args)...);
		threadHandle_ = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, OnInvoke, static_cast<void*>(this), isStartThread_ == true ? 0 : CREATE_SUSPENDED, NULL));
	}
public:
	template<class _Ty, class... _Args>
	SnowThread(bool isStartThread, _Ty&& ftn, _Args&&... args)
	{
		isStartThread_ = isStartThread;
		if (isStartThread_ == true) isRunning_ = true;
		Run(std::forward<_Ty>(ftn), std::forward<_Args>(args)...);
	}
	//���Ȯ�尡��
	virtual ~SnowThread()noexcept
	{

		if (threadHandle_ != INVALID_HANDLE_VALUE)
		{
			//Ŀ�� ��ü ��ȯ�� ������ �ƴ� �ʼ���.
			if (CloseHandle(threadHandle_) == TRUE)
			{
				threadHandle_ = NULL;
#ifdef  PRINT_THREAD_CLOSE_LOG
				PRINT_INFO_LOG("Sucess Close Thread Handle",
					"ThreadID: ", threadId_, "\n");
#endif 
			}
			else
			{
				PRINT_ERROR_LOG("Thread Close Handle", WSAGetLastError(), "\n");
			}
		}
	}

	SnowThread(const SnowThread&) = delete;
	SnowThread& operator=(const SnowThread&) = delete;
	SnowThread(SnowThread&&)noexcept = delete;
	SnowThread& operator=(SnowThread&&) = delete;
public:
	void SetThreadPriority(const int32_t priority)
	{
		if (::SetThreadPriority(threadHandle_, priority) == 0)
		{
			std::cout << "Can't Thread Priority: " << WSAGetLastError() << "\n";
		}
	}

	int32_t GetThreadPriority()const
	{
		if (threadHandle_ == INVALID_HANDLE_VALUE)return -1;
		return ::GetThreadPriority(threadHandle_);
	}

	void ContextSwitch()
	{

	}

	void WaitForThread()
	{
		WaitForSingleObject(threadHandle_, INFINITE);
	}

	bool StartThread()
	{
		if (isStartThread_ == false)
		{
			isStartThread_ = true;
			isRunning_ = true;
			ResumeThread(threadHandle_);
			return true;
		}

		return false;
	}

	ThreadID GetThreadID()const
	{
		return threadId_;
	}

	HANDLE GetHandle()const
	{
		return threadHandle_;
	}

	void SetThreadID(const ThreadID ThreadID)
	{
		threadId_ = ThreadID;
	}

	bool GetIsStartedThread()const
	{
		return isStartThread_;
	}

	bool GetIsRunningThread()const
	{
		return isRunning_;
	}
};


