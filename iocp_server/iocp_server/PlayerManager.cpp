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
	
	// create player object with session
	auto guestPlayerShared = std::make_shared<Player>(session);
	session->_sharedPlayer = guestPlayerShared;

	guestPlayerShared->EnterWriteLock();
	__guestPlayerMaplock.EnterWriteLock();

	// generate guset ID
	int guestGenID = -1;
	do {
		guestGenID = glm::linearRand<int>(MIN_GUEST_ID, MAX_GUEST_ID);
	} while (__guestPlayerMap.end() != __guestPlayerMap.find(guestGenID));

	InterlockedExchange(&guestPlayerShared->_playerUID, guestGenID);
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

// guest player�� ���� ������ player UID�� �̿��� guest->login player�� �ű�. guestPlayerShared�� UID�� playerUID�� update.
void PlayerManager::MoveGuestToLoginPlayer(std::shared_ptr<Player> guestPlayerShared, int playerUID)
{
	guestPlayerShared->EnterWriteLock();

	if (guestPlayerShared->GetPlayerUID() < MIN_GUEST_ID || MAX_GUEST_ID < guestPlayerShared->GetPlayerUID())
	{
		guestPlayerShared->LeaveWriteLock();
		printf_s("[LOG] : PlayerManager::MoveGuestToLoginPlayer try to change login UID to login UID : %d\n", GetLastError());
		CRASH_ASSERT(false);
	}

	// remove guest ptr from guestList
	__guestPlayerMaplock.EnterWriteLock();
	__guestPlayerMap.erase(guestPlayerShared->GetPlayerUID());
	__guestPlayerMaplock.LeaveWriteLock();

	// move player to playerList
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
	// TODO : ���Ӽ��ǿ� ���� player ã�� session<>client �����ϴ� ����� �Ʒ� �ڵ带 �̿��Ͽ� ����.
	printf_s("[LOG] : PlayerManager::MoveGuestToLoginPlayer player is already in login list : %d\n", GetLastError());
	CRASH_ASSERT(false);
	__loginPlayerMaplock.LeaveWriteLock();

	return;

	// server already has Player with id... -> Re Get that Player and connect with client
	// already has Player
	std::shared_ptr<Player> alreadyLogPlayerShared = __loginPlayerMap[playerUID];
	__loginPlayerMaplock.LeaveWriteLock();

	// remove guestPlayerShared from guest player list (Use above code)
	ClientSession* loginClientSession;

	// guest->loginClient remove
	guestPlayerShared->GetClientSessionWithAddRef(loginClientSession);
	guestPlayerShared->_clientSession = nullptr;	
	guestPlayerShared->LeaveWriteLock();

	// in game player object->loginClient
	alreadyLogPlayerShared->EnterWriteLock();
	alreadyLogPlayerShared->_clientSession = loginClientSession;	
	alreadyLogPlayerShared->_bClientConn = true;
	loginClientSession->ReleaseRef();

	// loginClient -> game player object
	alreadyLogPlayerShared->_clientSession->_sharedPlayer.reset();	//client->guest player remove
	alreadyLogPlayerShared->_clientSession->_sharedPlayer = alreadyLogPlayerShared;	//client->player
	alreadyLogPlayerShared->LeaveWriteLock();

	return;
}

bool PlayerManager::MoveLoginToPlayerGuest(std::shared_ptr<Player> playerShared)
{
	//TODO : unregister lobby, beforeLobby, Room except gamesession

	playerShared->EnterReadLock();
	__loginPlayerMaplock.EnterWriteLock();
	__loginPlayerMap.erase(playerShared->GetPlayerUID());
	__loginPlayerMaplock.LeaveWriteLock();
	playerShared->LeaveReadLock();

	// ������ player object�� ������� �ʰ� ���ο� guest player�� �޴´�.
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

std::shared_ptr<Player> PlayerManager::GetLoginPlayerSharedWithUID(int uid)
{
	FastSpinlockGuard guard(__loginPlayerMaplock, false);
	auto it = __loginPlayerMap.find(uid);
	if (it == __loginPlayerMap.end())
	{
		return nullptr;
	}
	return it->second;
}
