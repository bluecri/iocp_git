#include "stdafx.h"
#include "IOThread.h"


IOThread::IOThread(HANDLE hThread, HANDLE hCompletionPort) : __handle(hThread), __hCompletionPort(hCompletionPort)
{
}

IOThread::~IOThread()
{
}

DWORD IOThread::Run()
{
	return 0;
}

void IOThread::DoIocpJob()
{
}

void IOThread::DoTimerJob()
{
}
