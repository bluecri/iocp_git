#include "stdafx.h"

#include "PlayerManager.h"
#include "ClientSession.h"

PlayerManager* GPlayerManager = nullptr;

std::shared_ptr<Player> PlayerManager::GetGuestPlayer(ClientSession* session)
{
	if (!session->IsConnected())
	{
		printf_s("[LOG] : PlayerManager::GetGuestPlayer player is already disconn...\n");
		return nullptr;
	}

	auto guestPlayerShared = std::make_shared<Player>(session);
	guestPlayerShared->EnterWriteLock();
	__guestPlayerMaplock.EnterWriteLock();

	int guestGenID = -1;

	do {
		guestGenID = glm::linearRand<int>(MIN_GUEST_ID, MAX_GUEST_ID);
		
	} while (__guestPlayerMap.end() != __guestPlayerMap.find(guestGenID));
	
	guestPlayerShared->_playerUID = guestGenID;
	guestPlayerShared->_clientSession = session;

	__guestPlayerMap.insert(make_pair(guestGenID, guestPlayerShared));
	
	__guestPlayerMaplock.LeaveWriteLock();
	guestPlayerShared->LeaveWriteLock();

	return guestPlayerShared;
}

void PlayerManager::UnregisterGuestPlayer(int playerGuestID)
{
	FastSpinlockGuard guard(__guestPlayerMaplock);
	if (__guestPlayerMap.end() != __guestPlayerMap.find(playerGuestID)) {
		__guestPlayerMap.erase(playerGuestID);

		return;
	}

	printf_s("[LOG] : PlayerManager::UnregisterGuestPlayer cannot find playerGuestID : %d\n", playerGuestID);

	return;
}

// guest player와 새로 접속할 player UID를 이용해 guest->login player로 승격시킴. guestPlayerShared의 UID를 playerUID로 update.
void PlayerManager::MoveGuestToLoginPlayer(std::shared_ptr<Player> guestPlayerShared, int playerUID)
{
	guestPlayerShared->EnterWriteLock();

	if (guestPlayerShared->GetPlayerUID() < MIN_GUEST_ID || MAX_GUEST_ID < guestPlayerShared->GetPlayerUID())
	{
		guestPlayerShared->LeaveWriteLock();
		printf_s("[LOG] : PlayerManager::MoveGuestToLoginPlayer try to change login UID to login UID : %d\n", GetLastError());
		CRASH_ASSERT(false);
	}

	// remove this ptr from guestList
	__guestPlayerMaplock.EnterWriteLock();
	__guestPlayerMap.erase(guestPlayerShared->GetPlayerUID());
	__guestPlayerMaplock.LeaveWriteLock();

	__loginPlayerMaplock.EnterWriteLock();

	auto it = __loginPlayerMap.find(playerUID);
	if (it == __loginPlayerMap.end())
	{
		__loginPlayerMap.insert(make_pair(playerUID, guestPlayerShared));
		__loginPlayerMaplock.LeaveWriteLock();

		guestPlayerShared->_playerUID = playerUID;
		guestPlayerShared->LeaveWriteLock();

		return;
	}
	// TODO : 게임세션에 같은 player 찾아 session<>client 연결하는 방식을 아래 코드를 이용하여 적용.
	printf_s("[LOG] : PlayerManager::MoveGuestToLoginPlayer player is already in login list : %d\n", GetLastError());
	CRASH_ASSERT(false);
	__loginPlayerMaplock.LeaveWriteLock();

	return;

	// server already has Player with id... -> Re Get that Player and connect with client
	// already has Player
	std::shared_ptr<Player> alreadyLogPlayerShared = __loginPlayerMap[playerUID];
	__loginPlayerMaplock.LeaveWriteLock();

	ClientSession* newClientSession;
	guestPlayerShared->GetClientSessionWithAddRef(newClientSession);
	guestPlayerShared->_clientSession = nullptr;	//guest->client remove
	guestPlayerShared->LeaveWriteLock();

	alreadyLogPlayerShared->EnterWriteLock();
	alreadyLogPlayerShared->_clientSession = newClientSession;	//player ->client
	alreadyLogPlayerShared->_bClientConn = true;
	newClientSession->ReleaseRef();

	alreadyLogPlayerShared->_clientSession->_sharedPlayer.reset();	//client->guest player remove
	alreadyLogPlayerShared->_clientSession->_sharedPlayer = alreadyLogPlayerShared;	//client->player
	alreadyLogPlayerShared->LeaveWriteLock();

	return;
}

bool PlayerManager::MoveLoginToPlayerGuest(std::shared_ptr<Player> playerShared)
{
	//TODO : unregister lobby, beforeLobby, Room except gamesession

	// really need read lock? : 변하지 않을 UID, ID, nickname인데..?
	playerShared->EnterReadLock();
	__loginPlayerMaplock.EnterWriteLock();
	__loginPlayerMap.erase(playerShared->GetPlayerUID());
	__loginPlayerMaplock.LeaveWriteLock();
	playerShared->LeaveReadLock();

	// 기존의 player object를 사용하지 않고 새로운 guest player를 받는다.
	ClientSession * clientSession = nullptr;
	playerShared->GetClientSessionWithAddRef(clientSession);
	if (clientSession == nullptr)
	{
		return false;
	}

	if (nullptr == GetGuestPlayer(clientSession))
	{
		return false;
	}

	clientSession->ReleaseRef();
	return true;
}
