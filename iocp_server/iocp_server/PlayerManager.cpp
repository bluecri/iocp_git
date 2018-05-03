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

	// No need lock.
	guestPlayerShared->__playerId = guestGenID;
	__guestPlayerMap.insert(make_pair(guestGenID, guestPlayerShared));

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

void PlayerManager::RegisterLoginPlayer(std::shared_ptr<Player> guestPlayerShared, int playerID)
{
	guestPlayerShared->EnterWriteLock();

	// remove ptr from guestList
	__guestPlayerMaplock.EnterWriteLock();
	__guestPlayerMap.erase(guestPlayerShared->GetPlayerId());
	__guestPlayerMaplock.LeaveWriteLock();

	__loginPlayerMaplock.EnterReadLock();

	// server already has Player with id... -> Re Get that Player and connect with client
	auto it = __loginPlayerMap.find(guestPlayerShared->GetPlayerId());
	if (it != __loginPlayerMap.end()) 
	{
		// already has Player
		guestPlayerShared->__clientSession->_sharedPlayer.reset();	//client->guest player remove
		guestPlayerShared->__clientSession = nullptr;	//player->guest client remove


	}
	else
	{
		__guestPlayerMap.erase()
		__loginPlayerMap.insert(make_pair(toBeLoginPlayerShared->GetPlayerId(), toBeLoginPlayerShared));
	}


	__loginPlayerMaplock.EnterReadLock();


	guestPlayerShared->LeaveWriteLock();


	// else.. register new created player
	__stPlayerMap.insert(make_pair(guestGenID, guestPlayerShared));
}

void PlayerManager::UnregisterLoginPlayer(int plyaerID)
{
}
