#include"Lock.h"

stSRWLock::stSRWLock()							{InitializeSRWLock(&srwLock);}
void stSRWLock::AcquiredReadLock()noexcept		{AcquireSRWLockShared(&srwLock);}
void stSRWLock::ReleaseReadLock()noexcept		{ReleaseSRWLockShared(&srwLock);}
void stSRWLock::AcquireWrtieLock()noexcept		{AcquireSRWLockExclusive(&srwLock);}
void stSRWLock::ReleaseWriteLock()noexcept      {ReleaseSRWLockExclusive(&srwLock);}

////////////////////////
/////////Lock//////////
///////////////////////

stLock::stLock()                    { InitializeCriticalSectionAndSpinCount(&cs, MAX_SPIN_COUNT); }
stLock::~stLock()                   { DeleteCriticalSection(&cs); }
void stLock::AcquiredLock()         { EnterCriticalSection(&cs); }
void stLock::ReleaseLock()          { LeaveCriticalSection(&cs); }