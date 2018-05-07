#include "stdafx.h"
#include "DBManager.h"
#include "DBHelper.h"
#include "DBThread.h"
#include "DBContext.h"
#include "IocpManager.h"

DBManager* GDatabaseManager = nullptr;

DBManager::DBManager() : _dbCompletionPort(nullptr)
{
	memset(_dbWorkerThread, 0, sizeof(_dbWorkerThread));
}

DBManager::~DBManager()
{
}

bool DBManager::Initialize()
{
	/// Create I/O Completion Port
	_dbCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (_dbCompletionPort == NULL)
		return false;

	if (false == DBHelper::Initialize(SQL_SERVER_CONN_STR, MAX_DB_THREAD))
		return false;

	return true;
}

void DBManager::Finalize()
{
	DBHelper::Finalize();

	for (int i = 0; i < MAX_DB_THREAD; ++i)
	{
		CloseHandle(_dbWorkerThread[i]->GetHandle());
	}

	CloseHandle(_dbCompletionPort);
}

bool DBManager::StartDatabaseThreads()
{
	return false;
}

void DBManager::PostDatabaseRequest(OverlappedDBContext * dbContext)
{
	if (FALSE == PostQueuedCompletionStatus(_dbCompletionPort, 0, (ULONG_PTR)CK_DB_REQUEST, (LPOVERLAPPED)dbContext))
	{
		printf_s("DBManager::PostDatabaseResult PostQueuedCompletionStatus Error: %d\n", GetLastError());
		CRASH_ASSERT(false);
	}
}

unsigned int DBManager::FnDbWorkerThread(LPVOID lpParam)
{
	LThreadType = THREAD_DB_WORKER;
	LWorkerThreadId = reinterpret_cast<int>(lpParam);
	//GThreadCallHistory[LWorkerThreadId] = LThreadCallHistory = new ThreadCallHistory(LWorkerThreadId);
	//GThreadCallElapsedRecord[LWorkerThreadId] = LThreadCallElapsedRecord = new ThreadCallElapsedRecord(LWorkerThreadId);

	/// 반드시 DB 쓰레드가 먼저 띄우도록..
	CRASH_ASSERT(LWorkerThreadId < MAX_DB_THREAD);

	return GDatabaseManager->_dbWorkerThread[LWorkerThreadId]->Run();

}