#include "stdafx.h"

#include "IocpManager.h"
#include "DBThread.h"
#include "DBContext.h"

DBThread::DBThread(HANDLE hThread, HANDLE hCompletionPort) : __handle(hThread), __hCompletionPort(hCompletionPort)
{
}

DBThread::~DBThread()
{
	CloseHandle(__handle);
}

DWORD DBThread::Run()
{
	while (true) {
		DoDBJob();
	}
	return 0;
}

void DBThread::DoDBJob()
{
	DWORD dwTransferred;
	LPOVERLAPPED overlapped = nullptr;
	ULONG_PTR completionKey = 0;

	int ret = GetQueuedCompletionStatus(__hCompletionPort, &dwTransferred, &completionKey, &overlapped, GQCS_TIMEOUT);

	OverlappedDBContext* dbContext = reinterpret_cast<OverlappedDBContext*>(overlapped);

	if (CK_DB_REQUEST != completionKey) {
		CRASH_ASSERT(false);
		return;
	}

	dbContext->_bSuccess = dbContext->SQLExecute();

	GIocpManager->PostDatabaseResult(dbContext);
	
	return;
}
