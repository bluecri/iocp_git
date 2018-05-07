#pragma once

#include "stdafx.h"

class DBThread
{
public:
	DBThread(HANDLE hThread, HANDLE hCompletionPort);
	virtual ~DBThread();

	DWORD Run();

	void DoDBJob();

public:
	HANDLE GetHandle() { return __handle; }

private:
	HANDLE __handle;
	HANDLE __hCompletionPort;	// GetQueuedCompletionStatus�� ������ completionPort. GIDBManager�ε� ���� ����.
};