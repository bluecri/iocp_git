#include "stdafx.h"
#include "origin\ThreadLocal.h"

#include "IocpManager.h"
#include "IOThread.h"
#include "OverlappedContext.h"
#include "ClientSession.h"
#include "SendRequestSessionConcurrentQueue.h"
#include "DBContext.h"

IOThread::IOThread(HANDLE hThread, HANDLE hCompletionPort) : _handle(hThread), _hCompletionPort(hCompletionPort)
{
}

IOThread::~IOThread()
{
	CloseHandle(_handle);
}

DWORD IOThread::Run()
{
	while (true) {

		DoIocpJob();

		//DoTimerJob();

		DoSendFlushJob();
	}


	return 0;
}

void IOThread::DoIocpJob()
{

	DWORD dwTransferred = 0;
	LPOVERLAPPED overlapped = nullptr;

	ULONG_PTR completionKey = 0;

	SessionErrType completionOk = SessionErrType::FAIL;

	int ret = GetQueuedCompletionStatus(_hCompletionPort, &dwTransferred, (PULONG_PTR)&completionKey, &overlapped, GQCS_TIMEOUT);

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
		/*
		case IOTYPE_CONNECT:
		dynamic_cast<ServerSession*>(remote)->ConnectCompletion();
		completionOk = SessionErrType::SAFE;
		break;
		*/

	case IOTYPE_DISCONNECT:
		remote->DisconnectCompletion(static_cast<OverlappedDisconnectContext*>(context)->_dr);
		completionOk = SessionErrType::SAFE;
		break;

	case IOTYPE_ACCEPT:
		dynamic_cast<ClientSession*>(remote)->AcceptCompletion();
		completionOk = SessionErrType::SAFE;
		break;

	case IOTYPE_RECV_ZERO:
		completionOk = remote->PostRecv();
		break;

	case IOTYPE_SEND:
		//flush send뒤 바로 남은 buffer를 send한다면?, pending인 경우 해당 session은 재검사 될 것이므로 session list에서 제외해도 되는가? : 일단 시도.
		completionOk = remote->SendCompletion(dwTransferred);

		if (context->_wsaBuf.len != dwTransferred)
		{
			printf_s("Partial SendCompletion requested [%d], sent [%d]\n", context->_wsaBuf.len, dwTransferred);
			completionOk = SessionErrType::FAIL;
		}
		break;

	case IOTYPE_RECV:
		remote->RecvCompletion(dwTransferred);		// Check Packet with first length
		completionOk = remote->PreRecv();

		break;

	default:
		printf_s("Unknown I/O Type: %d\n", context->_ioType);
		CRASH_ASSERT(false);
		break;
	}

	if (!(completionOk == SessionErrType::SAFE || completionOk == SessionErrType::SAFE_SEND_PENDDING))
	{
		/// connection closing
		printf_s("DWORD IOThread::Run() completionOK fail.. try disconnect : %d\n", completionOk);
		remote->DisconnectRequest(DR_IO_REQUEST_ERROR);
	}

	DeleteIOContext(context);

	return;
}

void IOThread::DoTimerJob()
{
}

void IOThread::DoSendFlushJob()
{
	//global session flush
	Session* gSession;

	while (GSendRequestSessionQueue->PopSession(gSession))	// can exit? : 많은 경우 다른 IO Thread도 같이 시행하여 exit하는가?
	{
		SessionErrType completionOk = gSession->FlushSend();
		if (!(completionOk == SessionErrType::SAFE || completionOk == SessionErrType::SAFE_SEND_PENDDING))
		{
			printf_s("DWORD IOThread::DoSendFlushJob() GSession fail.. try disconnect : %d\n", completionOk);
			gSession->DisconnectRequest(DR_IO_REQUEST_ERROR);
			return;
		}
	}

	// local session flush
	while (!LSendRequestSessionQueue[0]->empty())
	{
		// LSendRequestSessionQueueIndex switch 방법 사용 x
		// LSendRequestSessionQueue[(LSendRequestSessionQueueIndex + 1 % 2)]->push(this);
	
		Session* session = LSendRequestSessionQueue[0]->front();
		LSendRequestSessionQueue[0]->pop();

		SessionErrType completionOk = session->FlushSend();
		if (!(completionOk == SessionErrType::SAFE || completionOk == SessionErrType::SAFE_SEND_PENDDING))
		{
			printf_s("DWORD IOThread::DoSendFlushJob() LSession fail.. try disconnect : %d\n", completionOk);
			session->DisconnectRequest(DR_IO_REQUEST_ERROR);
			return;
		}
	}
	
	return;
}
