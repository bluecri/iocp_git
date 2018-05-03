#include "stdafx.h"

#include "ClientSessionManager.h"
#include "Session.h"
#include "origin\ThreadLocal.h"
#include "ClientSession.h"
#include "IocpManager.h"


ClientSessionManager::~ClientSessionManager()
{
}

void ClientSessionManager::PrepareSessions()
{
	CRASH_ASSERT(LThreadType == THREAD_TYPE::THREAD_MAIN);
	
	for (int i = 0; i < MAX_CLIENT_CONNECTION; i++) {

		ClientSession* client = new ClientSession();
		__freeClientSessionList.push_back(client);
	}
}

bool ClientSessionManager::AcceptSessions()
{
	FastSpinlockGuard guard(__lock);

	while (__lCurrentIssueCount - __lCurrentReturnCount < MAX_CLIENT_CONNECTION)
	{
		ClientSession* newSession = __freeClientSessionList.front();
		__freeClientSessionList.pop_back();

		++__lCurrentIssueCount;

		newSession->AddRef(); ///< refcount +1 for issuing 

		if (false == newSession->PostAccept())
			return false;
	}


	return true;
}

void ClientSessionManager::ReturnSession(ClientSession * returnedSession)
{
	FastSpinlockGuard guard(__lock);

	CRASH_ASSERT(returnedSession->GetRef() == 0 && !returnedSession->IsConnected());

	returnedSession->SessionReset();

	__freeClientSessionList.push_back(returnedSession);

	__lCurrentReturnCount++;

	return;
}

ClientSessionManager* GClientSessionManager = nullptr;
