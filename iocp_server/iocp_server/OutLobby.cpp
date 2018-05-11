#include "stdafx.h"
#include "OutLobby.h"
#include "Lobby.h"
#include "LocationInfo.h"

OutLobby *GOutLobby = nullptr;

bool OutLobby::EnterLobby(std::shared_ptr<Player> playerShared, int targetLobbyUID, prop::outLobbyEnterLobbyResponse & msg)
{
	bool success = true;
	Lobby* targetLobby = nullptr;

	// lobby with targetLobbyUID is in lobbyList
	if (_lobbyMap.end() != _lobbyMap.find(targetLobbyUID))
	{
		targetLobby = _lobbyMap[targetLobbyUID];
	}
	
	// try user connect num ++
	if (targetLobby != nullptr && targetLobby->InterlockAddUserNum(1) <= _userMaxNum)
	{
		playerShared->EnterWriteLock();
		targetLobby->_lobbyPlayerMapLock.EnterWriteLock();

		// enter in lobby
		int playerUID = playerShared->GetPlayerUID();
		playerShared->GetLocInfo()->_lobbyUID = targetLobbyUID;
		targetLobby->_lobbyPlayerMap.insert(make_pair(playerUID, playerShared));

		targetLobby->_lobbyPlayerMapLock.LeaveWriteLock();
		playerShared->LeaveWriteLock();

		msg.set_success(true);
		msg.set_lobbyuid(targetLobbyUID);
		return true;
	}
	else
	{
		targetLobby->InterlockAddUserNum(-1);	//rollback userNum
	}
	
	msg.set_success(false);
	msg.set_lobbyuid(targetLobbyUID);
	return false;
}

bool OutLobby::GetLobbyList(std::shared_ptr<Player> playerShared, prop::outLobbyLobbylistResponse & msg)
{
	msg.set_success(true);

	for (auto elem : _lobbyMap)
	{
		prop::msgLobby* lobby = msg.add_lobbylist();

		lobby->set_uid(elem.second->GetUID());
		lobby->set_name(elem.second->GetLobbyName());
		lobby->set_usernum(elem.second->GetUserNum());
		lobby->set_useramaxnum(elem.second->GetUserMaxNum());
		lobby->set_closed(elem.second->IsClosed());
	}

	return true;
}
