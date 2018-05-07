#include "stdafx.h"
#include "IOThread.h"
#include "ClientSession.h"

#include "DBContext.h"
#include "OverlappedContext.h"

IOThread::IOThread(HANDLE hThread, HANDLE hCompletionPort) : __handle(hThread), __hCompletionPort(hCompletionPort)
{
}

IOThread::~IOThread()
{
	CloseHandle(__handle);
}

DWORD IOThread::Run()
{
	while (true) {

		DoIocpJob();

		DoTimerJob();
	}

	DWORD dwTransferred = 0;
	LPOVERLAPPED overlapped = nullptr;

	ULONG_PTR completionKey = 0;

	bool completionOk = false;

	int ret = GetQueuedCompletionStatus(__hCompletionPort, &dwTransferred, (PULONG_PTR)&completionKey, &overlapped, GQCS_TIMEOUT);

	if (CK_DB_RESULT == completionKey)
	{
		OverlappedDBContext* dbContext = reinterpret_cast<OverlappedDBContext*>(overlapped);
		dbContext->OnResult();

		delete dbContext;
		return;
	}

	/// 아래로는 일반적인 I/O 처리

	OverlappedIOContext* context = reinterpret_cast<OverlappedIOContext*>(overlapped);

	ClientSession* remote = (context != nullptr) ? static_cast<ClientSession*>(context->_sessionObject) : nullptr;

	if (ret == 0 || dwTransferred == 0)
	{
		/// check time out first 
		if (context == nullptr && GetLastError() == WAIT_TIMEOUT)
			return;


		if (context->_ioType == IOTYPE_RECV || context->_ioType == IOTYPE_SEND)
		{
			CRASH_ASSERT(nullptr != remote);

			/// In most cases in here: ERROR_NETNAME_DELETED(64)

			remote->DisconnectRequest(DR_COMPLETION_ERROR);

			DeleteIOContext(context);

			return;
		}
	}

	CRASH_ASSERT(nullptr != remote);

	switch (context->_ioType)
	{
	case IOTYPE_CONNECT:
		dynamic_cast<ServerSession*>(remote)->ConnectCompletion();
		completionOk = true;
		break;

	case IOTYPE_DISCONNECT:
		remote->DisconnectCompletion(static_cast<OverlappedDisconnectContext*>(context)->_dr);
		completionOk = true;
		break;

	case IOTYPE_ACCEPT:
		dynamic_cast<ClientSession*>(remote)->AcceptCompletion();
		completionOk = true;
		break;

	case IOTYPE_RECV_ZERO:
		completionOk = remote->PostRecv();
		break;

	case IOTYPE_SEND:
		remote->SendCompletion(dwTransferred);

		if (context->_wsaBuf.len != dwTransferred)
			printf_s("Partial SendCompletion requested [%d], sent [%d]\n", context->_wsaBuf.len, dwTransferred);
		else
			completionOk = true;

		break;

	case IOTYPE_RECV:
		remote->RecvCompletion(dwTransferred);

		completionOk = remote->PreRecv();

		break;

	default:
		printf_s("Unknown I/O Type: %d\n", context->_ioType);
		CRASH_ASSERT(false);
		break;
	}

	if (!completionOk)
	{
		/// connection closing
		remote->DisconnectRequest(DR_IO_REQUEST_ERROR);
	}

	DeleteIOContext(context);

	return 0;
}

void IOThread::DoIocpJob()
{
}

void IOThread::DoTimerJob()
{
}
