#include "stdafx.h"

#include "PlayerManager.h"
#include "ClientSession.h"

PlayerManager* GPlayerManager = nullptr;

std::shared_ptr<Player> PlayerManager::GetGuestPlayer(ClientSession* session)
{
	FastSpinlockGuard guard(__guestPlayerMaplock);
	int guestGenID = -1;

	do {
		guestGenID = glm::linearRand<int>(MIN_GUEST_ID, MAX_GUEST_ID);
		
	} while (__guestPlayerMap.end() != __guestPlayerMap.find(guestGenID));
	
	auto guestPlayerShared = std::make_shared<Player>(session);
	guestPlayerShared->EnterWriteLock();
	guestPlayerShared->_playerUID = guestGenID;
	guestPlayerShared->_clientSession = session;

	__guestPlayerMaplock.EnterWriteLock();
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

	// server already has Player with id... -> Re Get that Player and connect with client
	// already has Player
	std::shared_ptr<Player> alreadyLogPlayerShared = __loginPlayerMap[playerUID];
	__loginPlayerMaplock.LeaveWriteLock();

	ClientSession* newClientSession = guestPlayerShared->_clientSession;
	guestPlayerShared->_clientSession = nullptr;	//guest->client remove
	guestPlayerShared->LeaveWriteLock();

	alreadyLogPlayerShared->EnterWriteLock();
	alreadyLogPlayerShared->_clientSession = newClientSession;	//player ->client
	alreadyLogPlayerShared->_bClientConn = true;

	alreadyLogPlayerShared->_clientSession->_sharedPlayer.reset();	//client->guest player remove
	alreadyLogPlayerShared->_clientSession->_sharedPlayer = alreadyLogPlayerShared;	//client->player
	alreadyLogPlayerShared->LeaveWriteLock();

	return;
}

void PlayerManager::UnregisterLoginPlayer(int plyaerID)
{
}
