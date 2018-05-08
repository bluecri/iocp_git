#pragma once

#include "origin\ThreadLocal.h"

struct OverlappedDBContext;
class DBThread;

class DBManager
{
public:
	DBManager();
	~DBManager();

	bool Initialize();
	void Finalize();

	bool StartDatabaseThreads();

	void PostDatabaseRequest(OverlappedDBContext* dbContext);

private:
	static unsigned int WINAPI FnDbWorkerThread(LPVOID lpParam);

private:

	HANDLE	_dbCompletionPort;

	DBThread* _dbWorkerThread[MAX_DB_THREAD];

};

extern DBManager* GDatabaseManager;
