#include "stdafx.h"
#include "OverlappedContext.h"
#include "Session.h"

OverlappedContext::OverlappedContext(Session * owner) : _sessionObject(owner)
{
	memset(&_overlapped, 0, sizeof(_overlapped));
	memset(&_wsaBuf, 0, sizeof(_wsaBuf));

	_sessionObject->AddRef();
}

OverlappedIOContext::OverlappedIOContext(Session * owner, IOType ioType) : OverlappedContext(owner)
{
}

OverlappedDBContext::OverlappedDBContext(Session * owner, DBType ioType) : OverlappedContext(owner)
{
}

void DeleteIoContext(OverlappedContext * context)
{
}
