#include "../stdafx.h"
#include "Exception.h"
#include "FastSpinlock.h"
#include "LockOrderChecker.h"
#include "ThreadLocal.h"

FastSpinlock::FastSpinlock(const int lockOrder) : mLockFlag(0), mLockOrder(lockOrder)
{
}


FastSpinlock::~FastSpinlock()
{
}


void FastSpinlock::EnterWriteLock()
{
	/// 락 순서 신경 안써도 되는 경우는 그냥 패스
	if ( mLockOrder != LO_DONT_CARE)
		LLockOrderChecker->Push(this);

	while (true)
	{
		/// 다른놈이 writelock 풀어줄때까지 기다린다.
		while (mLockFlag & LF_WRITE_MASK)
			YieldProcessor();

		if ((InterlockedAdd(&mLockFlag, LF_WRITE_FLAG) & LF_WRITE_MASK) == LF_WRITE_FLAG)
		{
			/// 다른놈이 readlock 풀어줄때까지 기다린다.
			while (mLockFlag & LF_READ_MASK)
				YieldProcessor();

			return;
		}

		//write lock을 진입했는데 이미 write lock이 2개 이상 할당된 경우
		InterlockedAdd(&mLockFlag, -LF_WRITE_FLAG);
	}

}

void FastSpinlock::LeaveWriteLock()
{
	InterlockedAdd(&mLockFlag, -LF_WRITE_FLAG);

	/// 락 순서 신경 안써도 되는 경우는 그냥 패스
	if (mLockOrder != LO_DONT_CARE)
		LLockOrderChecker->Pop(this);
}

void FastSpinlock::EnterReadLock()
{
	if (mLockOrder != LO_DONT_CARE)
		LLockOrderChecker->Push(this);

	while (true)
	{
		/// 다른놈이 writelock 풀어줄때까지 기다린다.
		while (mLockFlag & LF_WRITE_MASK)
			YieldProcessor();

		//TOOODO: Readlock 진입 구현 (mLockFlag를 어떻게 처리하면 되는지)
		// if ( readlock을 얻으면 )
			//return;
		// else
			// mLockFlag 원복

		if ((InterlockedIncrement(&mLockFlag) & LF_WRITE_MASK) == LF_WRITE_FLAG) {
			InterlockedDecrement(&mLockFlag);
		}
		else {
			return;
		}
		
	}
}

void FastSpinlock::LeaveReadLock()
{
	//TooODO: mLockFlag 처리 
	InterlockedDecrement(&mLockFlag);

	if (mLockOrder != LO_DONT_CARE)
		LLockOrderChecker->Pop(this);
}