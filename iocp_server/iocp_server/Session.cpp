#include "stdafx.h"
#include "Session.h"

Session::Session(size_t sendBufSize, size_t recvBufSize) : __recvBuffer(0), __sendBuffer(recvBufSize)
{
	__socket = 0;
	__lRefCount = 0;
	__bConnected = 0;
}

void Session::SessionReset()
{
	__bConnected = 0;
	__lSendPendingCount = 0;
	__lRefCount = 0;

	__recvBuffer.BufferReset();

	__lockSendBuffer.EnterWriteLock();
	__sendBuffer.BufferReset();
	__lockSendBuffer.LeaveWriteLock();

	LINGER lingerOption;
	lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;

	/// no TCP TIME_WAIT
	if (SOCKET_ERROR == setsockopt(__socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof(LINGER)))
	{
		printf_s("[DEBUG] setsockopt linger option error: %d\n", GetLastError());
	}
	closesocket(__socket);

	__socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);


	// Do not player reset in here. Just Let go Player shared_ptr.
	return;
}


SessionErrType Session::PreRecv()
{
	if (!IsConnected()) 
	{
		return SessionErrType::DISCONN;
	}

	OverlappedPreRecvContext* preRecvContext = new OverlappedPreRecvContext(this);

	DWORD recvbytes = 0;
	DWORD flags = 0;

	if (SOCKET_ERROR == WSARecv(__socket, &preRecvContext->_wsaBuf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)preRecvContext, NULL)) 
	{
		if (WSAGetLastError() != WSA_IO_PENDING) 
		{
			DeleteIOContext(preRecvContext);
			printf_s("[DEBUG] : SessionPreRecv Error : %d\n", GetLastError());
			return SessionErrType::NOT_PENDING;
		}
	}

	return SessionErrType::SAFE;
}

SessionErrType Session::PostRecv()
{
	if (!IsConnected())
	{
		return SessionErrType::DISCONN;
	}

	if (0 == __recvBuffer.GetFreeSpaceSize())
	{
		return SessionErrType::POSTRECV_NOSPACE;
	}

	OverlappedPreRecvContext* preRecvContext = new OverlappedPreRecvContext(this);

	DWORD recvbytes = 0;
	DWORD flags = 0;
	preRecvContext->_wsaBuf.len = __recvBuffer.GetFreeSpaceSize();
	preRecvContext->_wsaBuf.buf = __recvBuffer.GetBufferEnd();

	if (SOCKET_ERROR == WSARecv(__socket, &preRecvContext->_wsaBuf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)preRecvContext, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIOContext(preRecvContext);
			printf_s("[DEBUG] : SessionPreRecv Error : %d\n", GetLastError());
			return NOT_PENDING;
		}
	}

	return SessionErrType::SAFE;
}

SessionErrType Session::PostSend(const char * data, size_t len)
{
	if (!IsConnected())
	{
		return SessionErrType::DISCONN;
	}

	FastSpinlockGuard criticalSection(__lockSendBuffer);

	if (__sendBuffer.GetFreeSpaceSize() < len)
		return SessionErrType::POSTSEND_NOSPACE;

	char* destData = __sendBuffer.GetBufferEnd();

	memcpy(destData, data, len);

	__sendBuffer.Commit(len);

	return SessionErrType::SAFE;
}

SessionErrType Session::FlushSend()
{
	if (!IsConnected())
	{
		DisconnectRequest(DR_SENDFLUSH_ERROR);
		return SessionErrType::DISCONN;
	}


	FastSpinlockGuard criticalSection(__lockSendBuffer);

	/// 보낼 데이터가 없는 경우
	if (0 == __sendBuffer.GetContiguiousBytes())
	{
		/// 보낼 데이터도 없는 경우
		if (0 == __lSendPendingCount)
			return SessionErrType::SAFE;

		return SessionErrType::NODATA_TOSEND_BUT_PENDDING;
	}

	/// 이전의 send가 완료 안된 경우
	if (__lSendPendingCount > 0)
		return SessionErrType::SEND_PENDDING;


	OverlappedSendContext* sendContext = new OverlappedSendContext(this);
	//GSendRequestSessionQueue
	DWORD sendbytes = 0;
	DWORD flags = 0;
	sendContext->_wsaBuf.len = (ULONG)__sendBuffer.GetContiguiousBytes();
	sendContext->_wsaBuf.buf = __sendBuffer.GetBufferStart();

	/// start async send
	if (SOCKET_ERROR == WSASend(__socket, &sendContext->_wsaBuf, 1, &sendbytes, flags, (LPWSAOVERLAPPED)sendContext, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIOContext(sendContext);
			printf_s("Session::FlushSend Error : %d\n", GetLastError());

			DisconnectRequest(DR_SENDFLUSH_ERROR);
			return SessionErrType::NOT_PENDING;
		}

	}

	__lSendPendingCount++;

	if (__lSendPendingCount == 1) {
		return SessionErrType::SAFE;
	}
	return SessionErrType::SEND_PENDING_COUNT_MISS;
}


void Session::SendCompletion(DWORD transferred)
{
}

void Session::RecvCompletion(DWORD transferred)
{
}

void Session::AddRef()
{
	CRASH_ASSERT(InterlockedIncrement(&__lRefCount) > 0);
}

void Session::ReleaseRef()
{
	long refCount = InterlockedDecrement(&__lRefCount);
	CRASH_ASSERT(refCount >= 0);

	if (refCount == 0)
	{
		OnRelease();	//virtual
	}
}

