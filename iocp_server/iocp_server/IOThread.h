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
	void DoSendFlushJob();

public:
	HANDLE GetHandle() { return _handle; }

private:
	HANDLE _handle;
	HANDLE _hCompletionPort;	// GetQueuedCompletionStatus�� ������ completionPort. GIocpManager�ε� ���� ����.
};