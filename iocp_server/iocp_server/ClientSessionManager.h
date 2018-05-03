#pragma once

#include "origin/XTL.h"
#include "origin\FastSpinlock.h"

class Session;
class ClientSession;

class ClientSessionManager
{
public:
	ClientSessionManager() : __lCurrentIssueCount(0), __lCurrentReturnCount(0) {  };
	virtual ~ClientSessionManager();

	void PrepareSessions();
	bool AcceptSessions();

	void ReturnSession(ClientSession* returnedSession);

private:
	using SessionList = xlist<ClientSession*>::type;
	SessionList __freeClientSessionList;

	FastSpinlock __lock;
	long __lCurrentIssueCount;
	long __lCurrentReturnCount;
};

extern ClientSessionManager* GClientSessionManager;
