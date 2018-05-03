#include "stdafx.h"
#include "SendRequestSessionConcurrentQueue.h"

SendRequestSessionConcurrentQueue* GSendRequestSessionQueue = nullptr;

void SendRequestSessionConcurrentQueue::PushSession(Session * session)
{
	GSendReqestSessionQueue.push(session);
	return;
}

bool SendRequestSessionConcurrentQueue::PopSession(Session* &session)
{
	return GSendReqestSessionQueue.try_pop(session);
}
