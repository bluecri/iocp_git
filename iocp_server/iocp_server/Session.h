#pragma once

#include "origin/CircularBuffer.h"
#include "origin/FastSpinlock.h"
#include "OverlappedContext.h"

enum SessionErrType {
	POSTRECV_NOSPACE,
	DISCONN,
	NOT_PENDING,
	POSTSEND_NOSPACE,
	SAFE,
	NODATA_TOSEND_BUT_PENDDING,
	SAFE_SEND_PENDDING,
	SEND_PENDING_COUNT_MISS,
	FAIL
};

class Player;
class Packet;

class Session
{
public:
	Session(size_t sendBufSize, size_t recvBufSize);
	virtual ~Session() {};

	virtual void SessionReset();

	bool IsConnected() const { return __bConnected; }
	virtual void DisconnectRequest(DisconnectReason dr) = 0;

	SessionErrType PreRecv();
	SessionErrType PostRecv();

	SessionErrType FlushSend();

	virtual void DisconnectCompletion(DisconnectReason dr) = 0;
	SessionErrType SendCompletion(DWORD transferred);
	virtual SessionErrType RecvCompletion(DWORD transferred);

	void AddRef();
	void ReleaseRef();
	long GetRef() { return __lRefCount; }

	virtual void OnDisconnect(DisconnectReason dr) = 0;
	virtual void OnRelease() = 0;

	void SetSocket(SOCKET sock) { __socket = sock; }
	SOCKET GetSocket() { return __socket; }

	//Client session
	/* SessionReset,	PostAccept,		AcceptCompletion()	 */

	//Server session
	/* ConnectReques,	ConnectCompletion		*/

protected:
	SOCKET __socket;

	CircularBuffer _recvBuffer;
	CircularBuffer _sendBuffer;
	FastSpinlock _lockSendBuffer;

	friend class Player;
	virtual SessionErrType PostSend(Packet* packet);

	volatile long __lRefCount;
	volatile long __lSendPendingCount;
	volatile long __bConnected;
};