#pragma once

#include "stdafx.h"

class IOThread
{
public:
	IOThread(HANDLE hThread, HANDLE hCompletionPort);
	virtual ~IOThread();

	DWORD Run();

	void DoIocpJob();
	void DoTimerJob();

public:
	HANDLE GetHandle() { return __handle; }

private:
	HANDLE __handle;
	HANDLE __hCompletionPort;	// GetQueuedCompletionStatus�� ������ completionPort. GIocpManager�ε� ���� ����.
};