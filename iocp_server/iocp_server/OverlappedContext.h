#pragma once
#include "origin\ObjectPool.h"

class Session;

enum IOType 
{
	IOTYPE_SEND,
	IOTYPE_RECV,
	IOTYPE_RECV_ZERO,
	IOTYPE_ACCEPT,
	IOTYPE_DISCONNECT,
	IOTYPE_CONNECT
};

enum DisconnectReason
{
	DR_NONE,
	DR_ACTIVE,
	DR_ONCONNECT_ERROR,
	DR_IO_REQUEST_ERROR,
	DR_COMPLETION_ERROR,
	DR_SENDFLUSH_ERROR
};

struct OverlappedContext
{
public:
	OverlappedContext();

	OVERLAPPED	_overlapped;
	WSABUF		_wsaBuf;
};


struct OverlappedIOContext : public OverlappedContext
{
public:
	OverlappedIOContext(Session* owner, IOType ioType);
	
	Session*	_sessionObject;		// OverlappedContext가 남아있는 경우(ref>0) session은 reset되지 않음.
	IOType		_ioType;
};

//IO Contexts

struct OverlappedPreRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedPreRecvContext>
{
	OverlappedPreRecvContext(Session* owner) : OverlappedIOContext(owner, IOTYPE_RECV_ZERO)
	{
		_wsaBuf.buf = nullptr;
		_wsaBuf.len = 0;
	}
};

struct OverlappedRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedRecvContext>
{
	OverlappedRecvContext(Session* owner) : OverlappedIOContext(owner, IOTYPE_RECV)
	{
	}
};

struct OverlappedSendContext : public OverlappedIOContext, public ObjectPool<OverlappedSendContext>
{
	OverlappedSendContext(Session* owner) : OverlappedIOContext(owner, IOTYPE_SEND)
	{
	}
};

struct OverlappedAcceptContext : public OverlappedIOContext, public ObjectPool<OverlappedAcceptContext>
{
	OverlappedAcceptContext(Session* owner) : OverlappedIOContext(owner, IOTYPE_ACCEPT)
	{
	}
};

struct OverlappedConnectContext : public OverlappedIOContext, public ObjectPool<OverlappedConnectContext>
{
	OverlappedConnectContext(Session* owner, DisconnectReason dr) : OverlappedIOContext(owner, IOTYPE_CONNECT), _disconnectReason(dr)
	{
	}

	DisconnectReason _disconnectReason;
};

struct OverlappedDisconnectContext : public OverlappedIOContext, public ObjectPool<OverlappedDisconnectContext>
{
	OverlappedDisconnectContext(Session* owner, DisconnectReason dr) : OverlappedIOContext(owner, IOTYPE_DISCONNECT), _dr(dr)
	{
	}

	DisconnectReason _dr;
};



//Delete context

void DeleteIOContext(OverlappedIOContext* context);
