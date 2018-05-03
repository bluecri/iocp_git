#pragma once

#include <concurrent_queue.h>

class Session;
class Player;

class SendRequestSessionConcurrentQueue
{
public:
	void PushSession(Session* session);
	bool PopSession(Session* &session);

private:
	using SessionListType = concurrency::concurrent_queue<Session*>;
	SessionListType GSendReqestSessionQueue;

};

extern SendRequestSessionConcurrentQueue* GSendRequestSessionQueue;