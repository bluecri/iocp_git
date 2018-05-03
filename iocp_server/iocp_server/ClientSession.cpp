#include "stdafx.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "Player.h"
#include "PlayerManager.h"
#include "origin\ThreadLocal.h"
#include "IocpManager.h"

ClientSession::ClientSession(size_t sendBufSize, size_t recvBufSize) : Session(sendBufSize, recvBufSize)
{
	memset(&__clientSockAddr, 0, sizeof(__clientSockAddr));
}

ClientSession::~ClientSession()
{
}

// called from GSessionManager
void ClientSession::SessionReset()
{
	Session::SessionReset();
	memset(&__clientSockAddr, 0, sizeof(__clientSockAddr));

	return;
}

bool ClientSession::PostAccept()
{
	CRASH_ASSERT(LThreadType == THREAD_IO_WORKER);

	return false;
}

void ClientSession::AcceptCompletion()
{
	CRASH_ASSERT(LThreadType == THREAD_IO_WORKER);

	if (1 == InterlockedExchange(&__bConnected, 1)) {
		// 이미 연결됨.
		CRASH_ASSERT(false);
		return;
	}

	bool resultOk = true;
	do
	{
		if (SOCKET_ERROR == setsockopt(__socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)GIocpManager->GetPtrListenSocket(), sizeof(SOCKET)))
		{
			printf_s("[DEBUG] SO_UPDATE_ACCEPT_CONTEXT error: %d\n", GetLastError());
			resultOk = false;
			break;
		}

		int opt = 1;
		if (SOCKET_ERROR == setsockopt(__socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
		{
			printf_s("[DEBUG] TCP_NODELAY error: %d\n", GetLastError());
			resultOk = false;
			break;
		}

		opt = 0;
		if (SOCKET_ERROR == setsockopt(__socket, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
		{
			printf_s("[DEBUG] SO_RCVBUF change error: %d\n", GetLastError());
			resultOk = false;
			break;
		}

		int addrlen = sizeof(SOCKADDR_IN);
		if (SOCKET_ERROR == getpeername(__socket, (SOCKADDR*)&__clientSockAddr, &addrlen))
		{
			printf_s("[DEBUG] getpeername error: %d\n", GetLastError());
			resultOk = false;
			break;
		}

		HANDLE handle = CreateIoCompletionPort((HANDLE)__socket, GIocpManager->GetHandleCompletionPort, (ULONG_PTR)this, 0);
		if (handle != GIocpManager->GetHandleCompletionPort())
		{
			printf_s("[DEBUG] CreateIoCompletionPort error: %d\n", GetLastError());
			resultOk = false;
			break;
		}

	} while (false);


	if (!resultOk)
	{
		DisconnectRequest(DR_ONCONNECT_ERROR);
		return;
	}

	printf_s("[DEBUG] Client Connected: IP=%s, PORT=%d\n", inet_ntoa(__clientSockAddr.sin_addr), ntohs(__clientSockAddr.sin_port));


	// Guest Login 처리.
	_sharedPlayer = GPlayerManager->GetGuestPlayer(this);

	if (false == PreRecv())
	{
		printf_s("[DEBUG] PreRecv error: %d\n", GetLastError());
	}

	return;
}

void ClientSession::DisconnectRequest(DisconnectReason dr)
{

	_sharedPlayer->EnterWriteLock();
	if (0 == InterlockedExchange(&__bConnected, 0))
	{
		// already disconnecting...
		_sharedPlayer->LeaveWriteLock();
		return;
	}

	OverlappedDisconnectContext* context = new OverlappedDisconnectContext(this, dr);
	_sharedPlayer->__clientSession = nullptr;

	_sharedPlayer->LeaveWriteLock();

	_sharedPlayer.reset();	// release shared_ptr Player

	if (FALSE == GIocpManager->_slpfnDisconnectEx(__socket, (LPOVERLAPPED)context, TF_REUSE_SOCKET, 0))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(context);
			printf_s("[DEBUG] : ClientSession::DisconnectRequest Error : %d\n", GetLastError());
		}
	}

	return;
}

void ClientSession::DisconnectCompletion(DisconnectReason dr)
{
	OnDisconnect(dr);

	ReleaseRef();
}

void ClientSession::OnDisconnect(DisconnectReason dr)
{
	printf_s("[LOG] ClientSession::OnDisconnect : %d\n", dr);
	return;
}

// called from Session::releaseRef
void ClientSession::OnRelease()
{
	//session reset
	GClientSessionManager->ReturnSession(this);
}
