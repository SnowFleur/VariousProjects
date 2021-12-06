#pragma once
#include"WindowsHeader.h"
#include"DefineUtilty.h"
/*
- Developer: PDH
- Descriptor: Lock을 캡슐화 하여 Lock Guard 구현
-
*/

////////////////////////
///////// SRWLock///////
////////////////////////
struct stSRWLock {
private:
	SRWLOCK	srwLock;
public:
	stSRWLock();

	void AcquiredReadLock()noexcept;
	void ReleaseReadLock()noexcept;
	void AcquireWrtieLock()noexcept;
	void ReleaseWriteLock()noexcept;
};


////////////////////////
/////////Lock//////////
///////////////////////
constexpr int MAX_SPIN_COUNT = 4000;
struct stLock {
private:
	CRITICAL_SECTION cs;
public:
	stLock();
	~stLock();
	DELETE_COPY(stLock);
	DELETE_MOVE(stLock);

	void AcquiredLock();
	void ReleaseLock();
};

/*
RAII 패턴 기반 LockGuard Default 생성자, 복사 생성자, 복사 대입연산 삭제
*/
struct stLockGuard {
private:
	stLock&		lockGuard;
public:
	DELETE_COPY(stLockGuard);
	DELETE_MOVE(stLockGuard);

	explicit stLockGuard(stLock& lock) : lockGuard(lock) { lockGuard.AcquiredLock(); }
	~stLockGuard() { lockGuard.ReleaseLock(); }
};

struct stWriteLockGuard {
public:
	SRWLOCK&	lockGuard;
public:
	DELETE_COPY(stWriteLockGuard);
	DELETE_MOVE(stWriteLockGuard);

	explicit stWriteLockGuard(SRWLOCK& lock) : lockGuard(lock) { AcquireSRWLockExclusive(&lockGuard);}
	~stWriteLockGuard() { ReleaseSRWLockExclusive(&lockGuard); }
};

struct stReadLockGuard {
public:
	SRWLOCK&	lockGuard;
public:
	DELETE_COPY(stReadLockGuard);
	DELETE_MOVE(stReadLockGuard);

	explicit stReadLockGuard(SRWLOCK& lock) : lockGuard(lock) { AcquireSRWLockShared(&lockGuard); }
	~stReadLockGuard() { ReleaseSRWLockShared(&lockGuard); }
};


#define EXCLUSIVE_LOCKGUARD(number,lock)		stLockGuard			lock##number(lock)
#define	WRITE_LOCKGUARDD(number,lock)		    stWriteLockGuard	lock##number(lock)
#define SHARED_LOCKGUARD(number,lock)			stReadLockGuard		lock##number(lock)