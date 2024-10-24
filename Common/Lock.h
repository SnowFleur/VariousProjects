#pragma once
/*
- Descriptor: Lock을 캡슐화 하여 Lock Guard 구현
-
*/

#include"../Common/WindowsHeader.h"
#include"../Common/DefineUtilty.h"


////////////////////////
///////// SRWLock///////
////////////////////////
class SlimRWLock
{
private:
	SRWLOCK	srwLock_;
public:
	SlimRWLock()
	{
		InitializeSRWLock(&srwLock_);
	}

	void AcquiredReadLock()noexcept
	{
		AcquireSRWLockShared(&srwLock_);
	}

	void ReleaseReadLock()noexcept
	{
		ReleaseSRWLockShared(&srwLock_);
	}

	void AcquireWriteLock()noexcept
	{
		AcquireSRWLockExclusive(&srwLock_);
	}

	void ReleaseWriteLock()noexcept
	{
		// To do 경고 수정하기..
		// 순서 잘 지키면 상관없긴한데.. 경고가 뜨네
		ReleaseSRWLockExclusive(&srwLock_);
	}
};


////////////////////////
/////////Lock//////////
///////////////////////

constexpr int MAX_SPIN_COUNT = 4000;

class Lock
{
private:
	CRITICAL_SECTION cs_;
public:
	Lock()
	{
		//To do 경고 수정하기
		InitializeCriticalSectionAndSpinCount(&cs_, MAX_SPIN_COUNT);
	}
	~Lock()
	{
		DeleteCriticalSection(&cs_);
	}
	DELETE_COPY(Lock);
	DELETE_MOVE(Lock);
public:
	void AcquiredLock()
	{
		EnterCriticalSection(&cs_);
	}
	void ReleaseLock()
	{
		LeaveCriticalSection(&cs_);
	}
};

/*
RAII 패턴 기반 LockGuard Default 생성자, 복사 생성자, 복사 대입연산 삭제
*/
class LockGuard
{
private:
	Lock& lockGuard_;
public:
	DELETE_COPY(LockGuard);
	DELETE_MOVE(LockGuard);

	explicit LockGuard(Lock& lock) : lockGuard_(lock) { lockGuard_.AcquiredLock(); }
	~LockGuard() { lockGuard_.ReleaseLock(); }
};

class WriteLockGuard
{
public:
	SRWLOCK& lockGuard_;
public:
	DELETE_COPY(WriteLockGuard);
	DELETE_MOVE(WriteLockGuard);

	explicit WriteLockGuard(SRWLOCK& lock) : lockGuard_(lock) { AcquireSRWLockExclusive(&lockGuard_); }
	~WriteLockGuard() { ReleaseSRWLockExclusive(&lockGuard_); }
};

class ReadLockGuard
{
public:
	SRWLOCK& lockGuard_;
public:
	DELETE_COPY(ReadLockGuard);
	DELETE_MOVE(ReadLockGuard);

	explicit ReadLockGuard(SRWLOCK& lock) : lockGuard_(lock) { AcquireSRWLockShared(&lockGuard_); }
	~ReadLockGuard() { ReleaseSRWLockShared(&lockGuard_); }
};


#define EXCLUSIVE_LOCK_GUARD(number,lock)		LockGuard		lock##number(lock)
#define	WRITE_LOCK_GUARD(number,lock)		    WriteLockGuard	lock##number(lock)
#define SHARED_LOCK_GUARD(number,lock)			ReadLockGuard	lock##number(lock)