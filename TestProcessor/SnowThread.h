#pragma once
/*
- Developer: PDH
- Descriptor: Thread를 캡슐화 하여 여러가지 기능을 가지는 클래스
-
*/

#include<functional>
#include<iostream>
#include"WindowsHeader.h"
#include"LogCollector.h"

#define PRINT_THREAD_CLOSE_LOG

#ifdef _WIN64
using ThreadID = DWORD64;
#else
using ThreadID = DWORD32;
#endif // _WIN64

class CSnowThread 
{
protected:
    using CallBackThreadFtn = std::function<uint32_t()>;
private:
    HANDLE                      threadHandle_;
    bool                        isStartThread_;
    CallBackThreadFtn           fRunThreadFunction_;
    ThreadID                    threadID_;

    static  uint32_t __stdcall    OnInvoke(LPVOID arg)
    {
        if (reinterpret_cast<CSnowThread*>(arg) == nullptr) return 0;
        uint32_t returnValue = reinterpret_cast<CSnowThread*>(arg)->fRunThreadFunction_();
#ifdef  PRINT_THREAD_CLOSE_LOG
        PRINT_INFO_LOG("End Thread\n");
#endif 
        _endthreadex(0);
        return returnValue;
    }

public:
    //상속확장가능
    virtual ~CSnowThread()noexcept;
    CSnowThread(const CSnowThread&)                 = delete;
    CSnowThread& operator=(const CSnowThread&)      = delete;
    CSnowThread(CSnowThread&&)noexcept              = delete;
    CSnowThread& operator=(CSnowThread&&)           = delete;
public:

    /*여러 가지 기능들 */
    void        SetThreadPriority(const int32_t priority);
    int32_t     GetThreadPriority()const;
    void        ContextSwitch();
    void        WaitForThread();
    bool        StartThread();

    inline ThreadID GetThreadID()const                      { return threadID_; }
    inline HANDLE   GetHandle()const                        { return threadHandle_; }
    inline void     SetThreadID(const ThreadID ThreadID)    { threadID_ = ThreadID; }
    inline bool     GetIsStartedThread()const               { return isStartThread_; }

    /*Template Functions*/
private:
    template<class _Ty, class... _Args>
    void Run(_Ty&& ftn, _Args&&... args)
    {
        fRunThreadFunction_ = std::bind(std::forward<_Ty>(ftn), std::forward<_Args>(args)...);
        threadHandle_ = reinterpret_cast<HANDLE>(_beginthreadex(NULL, isStartThread_ == true ? 0 : CREATE_SUSPENDED, OnInvoke, static_cast<void*>(this), 0, NULL));
    }
public:
    template<class _Ty, class... _Args>
    CSnowThread(bool isStartThread, _Ty&& ftn, _Args&&... args)
    {
        isStartThread_ = isStartThread;
        Run(std::forward<_Ty>(ftn), std::forward<_Args>(args)...);
    }


};
