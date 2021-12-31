#pragma once
#include"WindowsHeader.h"

struct stLockGuard
{
public:
    CRITICAL_SECTION& ownerLock;
    explicit stLockGuard(CRITICAL_SECTION& lock) :
        ownerLock(lock)
    {
        EnterCriticalSection(&ownerLock);
    }

    ~stLockGuard()noexcept
    {
        LeaveCriticalSection(&ownerLock);
    }

    stLockGuard(stLockGuard&)                      = delete;
    stLockGuard(stLockGuard&&)noexcept             = delete;
    stLockGuard& operator=(const stLockGuard&)     = delete;
    stLockGuard& operator=(stLockGuard&&)noexcept  = delete;
};

