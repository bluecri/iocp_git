#include "stdafx.h"
#include "OverlappedContext.h"
#include "Session.h"

OverlappedContext::OverlappedContext()
{
	memset(&_overlapped, 0, sizeof(_overlapped));
	memset(&_wsaBuf, 0, sizeof(_wsaBuf));
}

OverlappedIOContext::OverlappedIOContext(Session * owner, IOType ioType) : OverlappedContext() ,_sessionObject(owner), _ioType(ioType)
{
	_sessionObject->AddRef();
}


void DeleteIOContext(OverlappedIOContext * context)
{
	if (nullptr == context)
		return;

	context->_sessionObject->ReleaseRef();

	/// ObjectPool's operate delete dispatch
	switch (context->_ioType)
	{
	case IOTYPE_SEND:
		delete static_cast<OverlappedSendContext*>(context);
		break;

	case IOTYPE_RECV_ZERO:
		delete static_cast<OverlappedPreRecvContext*>(context);
		break;

	case IOTYPE_RECV:
		delete static_cast<OverlappedRecvContext*>(context);
		break;

	case IOTYPE_DISCONNECT:
		delete static_cast<OverlappedDisconnectContext*>(context);
		break;

	case IOTYPE_ACCEPT:
		delete static_cast<OverlappedAcceptContext*>(context);
		break;

	case IOTYPE_CONNECT:
		delete static_cast<OverlappedConnectContext*>(context);
		break;

	default:
		CRASH_ASSERT(false);
	}

	return;
}
