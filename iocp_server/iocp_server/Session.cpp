#include "stdafx.h"
#include "Session.h"



#include "origin\ThreadLocal.h"

Session::Session(size_t sendBufSize, size_t recvBufSize) : _recvBuffer(0), _sendBuffer(recvBufSize)
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

	_recvBuffer.BufferReset();

	_lockSendBuffer.EnterWriteLock();
	_sendBuffer.BufferReset();
	_lockSendBuffer.LeaveWriteLock();

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

	if (0 == _recvBuffer.GetFreeSpaceSize())
	{
		return SessionErrType::POSTRECV_NOSPACE;
	}

	OverlappedPreRecvContext* preRecvContext = new OverlappedPreRecvContext(this);

	DWORD recvbytes = 0;
	DWORD flags = 0;
	//ulong unsing int64
	preRecvContext->_wsaBuf.len = _recvBuffer.GetFreeSpaceSize();
	preRecvContext->_wsaBuf.buf = _recvBuffer.GetBufferEnd();

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

SessionErrType Session::PostSend(Packet* packet)
{
	/*
	if (!IsConnected())
	{
		return SessionErrType::DISCONN;
	}

	FastSpinlockGuard criticalSection(_lockSendBuffer);

	if (_sendBuffer.GetFreeSpaceSize() < packet->GetSize())
		return SessionErrType::POSTSEND_NOSPACE;

	char* destData = _sendBuffer.GetBufferEnd();
	protobuf::io::ArrayOutputStream arrayOutputStream(destData, packet->GetSize());
	protobuf::io::CodedOutputStream codeOutputStream(&arrayOutputStream);

	codeOutputStream.WriteRaw(&packet->GetHeader(), sizeof(PacketHeader));
	(packet->GetMsg()).SerializePartialToCodedStream(&codeOutputStream);

	//memcpy(destData, data, len);

	_sendBuffer.Commit(packet->GetSize());
	*/
	return SessionErrType::SAFE;
}

SessionErrType Session::FlushSend()
{
	CRASH_ASSERT(LThreadType == THREAD_TYPE::THREAD_IO_WORKER);

	//global -> local..

	if (!IsConnected())
	{
		DisconnectRequest(DR_SENDFLUSH_ERROR);
		return SessionErrType::DISCONN;
	}
	
	FastSpinlockGuard criticalSection(_lockSendBuffer);

	/// 보낼 데이터가 없는 경우
	if (0 == _sendBuffer.GetContiguiousBytes())
	{
		/// 보내고있는 데이터도 없는 경우
		if (0 == __lSendPendingCount)
			return SessionErrType::SAFE;

		return SessionErrType::NODATA_TOSEND_BUT_PENDDING;
	}

	/// 이전의 send가 완료 안된 경우
	if (__lSendPendingCount > 0)
	{
		return SessionErrType::SAFE_SEND_PENDDING;
	}


	OverlappedSendContext* sendContext = new OverlappedSendContext(this);
	//GSendRequestSessionQueue
	DWORD sendbytes = 0;
	DWORD flags = 0;
	sendContext->_wsaBuf.len = (ULONG)_sendBuffer.GetContiguiousBytes();
	sendContext->_wsaBuf.buf = _sendBuffer.GetBufferStart();

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


SessionErrType Session::SendCompletion(DWORD transferred)
{
	FastSpinlockGuard criticalSection(_lockSendBuffer);

	_sendBuffer.Remove(transferred);

	if (0 != _sendBuffer.GetContiguiousBytes())
	{
		// 바로 이어서 전송

		OverlappedSendContext* sendContext = new OverlappedSendContext(this);
		//GSendRequestSessionQueue
		DWORD sendbytes = 0;
		DWORD flags = 0;
		sendContext->_wsaBuf.len = (ULONG)_sendBuffer.GetContiguiousBytes();
		sendContext->_wsaBuf.buf = _sendBuffer.GetBufferStart();

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
		return SessionErrType::SAFE;
	}

	__lSendPendingCount--;
	return SessionErrType::SAFE;

}
SessionErrType Session::RecvCompletion(DWORD transferred)
{
	// client session으로 내릴것
	/*
	_recvBuffer.Commit(transferred);

	if (_recvBuffer.GetContiguiousBytes() < sizeof(PacketHeader))
		return SessionErrType::SAFE;

	PacketHeader packetHeader;
	char* destData = _recvBuffer.GetBufferStart();

	protobuf::io::ArrayInputStream arrayInputStream(destData, _recvBuffer.GetContiguiousBytes());
	protobuf::io::CodedInputStream codeInputStream(&arrayInputStream);

	codeInputStream.ReadRaw(&packetHeader, sizeof(PacketHeader));

	if (_recvBuffer.GetContiguiousBytes() < packetHeader.GetSize())
		return SessionErrType::SAFE;

	PacketRecvToMsg(static_cast<ClientSession*>(this), packetHeader, codeInputStream);


	//memcpy(destData, data, len);

	_sendBuffer.Commit(packetHeader.GetSize());

	return SessionErrType::SAFE;
	*/
	return SessionErrType::SAFE;
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
