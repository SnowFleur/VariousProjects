#pragma once
/*
- Descriptor: Lock�� ĸ��ȭ �Ͽ� Lock Guard ����
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
		// To do ��� �����ϱ�..
		// ���� �� ��Ű�� ��������ѵ�.. ��� �߳�
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
		//To do ��� �����ϱ�
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
RAII ���� ��� LockGuard Default ������, ���� ������, ���� ���Կ��� ����
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