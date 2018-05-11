#include "stdafx.h"
#include "Room.h"
#include "Lobby.h"
#include "LocationInfo.h"

bool Room::DestroyRoom()
{
	return false;
}

bool Room::LeaveRoom(std::shared_ptr<Player> playerShared, prop::inRoomLeaveRoomResponse & msg)
{
	if (_bClosed == true)
	{
		//already closed
		msg.set_success(false);
		msg.set_roomuid(_uid);
		msg.set_lobbyuid();

		return false;
	}

	playerShared->EnterReadLock();
	int outPlayerUID = playerShared->GetPlayerUID();
	msg.set_success(true);
	msg.set_lobbyuid(playerShared->GetLocInfo()->_lobbyUID);
	msg.set_roomuid(_uid);
	playerShared->LeaveReadLock();

	FastSpinlockGuard guard(_roomLock, true);

	if (outPlayerUID == _managerUID)		// all player out
	{
		// close room
		_bClosed = false;

		// out all players. if not manager, send inRoomForceOustRoomBroadcast
		std::vector<std::shared_ptr<Player>> savedPlayerVec;
		_roomPlayerMapLock.EnterWriteLock();
		for (auto elem : _roomPlayerMap)
		{
			savedPlayerVec.push_back(elem);		// get player list in room
		}
		_roomPlayerMap.clear();
		_roomPlayerMapLock.LeaveWriteLock();

		for (auto outPlayer : savedPlayerVec)
		{
			outPlayer->EnterWriteLock();

			// check player is in correct room
			if (outPlayer->GetLocInfo()->_roomUID != -1 || outPlayer->GetLocInfo()->_roomUID != _uid)
			{
				outPlayer->LeaveWriteLock();
				continue;
			}
			outPlayer->GetLocInfo()->_roomUID = -1;
			_userNum -= 1;

			// out playerlist in room
			_roomPlayerMapLock.EnterWriteLock();
			_roomPlayerMap.insert(make_pair(outPlayerUID, outPlayer));
			_roomPlayerMapLock.LeaveWriteLock();

			// add playerlist in lobby
			_beforeLobby->_lobbyPlayerMapLock.EnterWriteLock();
			_beforeLobby->_lobbyPlayerMap.insert(make_pair(outPlayerUID, outPlayer));
			_beforeLobby->_lobbyPlayerMapLock.LeaveWriteLock();

			// if outPlayer is not manager, post inRoomForceOustRoomBroadcast msg.
			if (outPlayer->GetPlayerUID() != _managerUID)
			{
				// TODO : post inRoomForceOustRoomBroadcast msg
				outPlayer->LeaveWriteLock();
			}
			outPlayer->LeaveWriteLock();
		}

		// remove room in lobby room list
		CRASH_ASSERT(0 == _userNum);

		_beforeLobby->_roomMapLock.EnterWriteLock();
		// PacketRecvMsgHandle(ClientSession * session, prop::inRoomLeaveRoomRequest..)에서 shared_ptr을 가지고 있으므로 safe
		_beforeLobby->_roomMap.erase(_uid);
		_beforeLobby->_roomMapLock.LeaveWriteLock();
	}
	else
	{
		playerShared->EnterWriteLock();

		// check player is in correct room
		if (playerShared->GetLocInfo()->_roomUID != -1 || playerShared->GetLocInfo()->_roomUID != _uid)
		{
			msg.set_success(false);
			msg.set_lobbyuid(playerShared->GetLocInfo()->_lobbyUID);
			msg.set_roomuid(_uid);

			playerShared->LeaveWriteLock();
			return false;
		}
		playerShared->GetLocInfo()->_roomUID = -1;
		_userNum -= 1;

		// out playerlist in room
		_roomPlayerMapLock.EnterWriteLock();
		_roomPlayerMap.insert(make_pair(outPlayerUID, playerShared));
		_roomPlayerMapLock.LeaveWriteLock();

		// add playerlist in lobby
		_beforeLobby->_lobbyPlayerMapLock.EnterWriteLock();
		_beforeLobby->_lobbyPlayerMap.insert(make_pair(outPlayerUID, playerShared));
		_beforeLobby->_lobbyPlayerMapLock.LeaveWriteLock();

		msg.set_success(true);
		msg.set_lobbyuid(playerShared->GetLocInfo()->_lobbyUID);
		msg.set_roomuid(_uid);

		playerShared->LeaveWriteLock();
	}

	return true;
}

bool Room::ReadyRequest(std::shared_ptr<Player> playerShared, bool bReadyReq, prop::inRoomReadyResponse & msg)
{
	msg.set_success(false);

	//if playerShared uid == _managerUID -> false
	playerShared->EnterReadLock();
	int playerUID = playerShared->GetPlayerUID();
	if (playerUID == _managerUID)
	{
		playerShared->LeaveReadLock();
		return false;
	}
	playerShared->LeaveReadLock();

	// room is disconn -> false
	FastSpinlockGuard guard(_roomLock, true);
	if (_bClosed == true)
	{
		return false;
	}

	// lock
	playerShared->EnterReadLock();
	_roomPlayerMapLock.EnterWriteLock();

	// set roomreadyMap with check valid
	if (!(_roomPlayerMap.end() != _roomPlayerMap.find(playerUID) && _roomReadyMap.end() != _roomPlayerMap.find(playerUID)))
	{
		// fail
		_roomPlayerMapLock.LeaveWriteLock();
		playerShared->LeaveReadLock();
		return false;
	}

	_roomReadyMap[playerUID] = 1;

	_roomPlayerMapLock.LeaveWriteLock();
	playerShared->LeaveReadLock();

	return false;
}

bool Room::StartRequest(std::shared_ptr<Player> playerShared, bool bStartReq, prop::inRoomStartResponse & msg)
{
	msg.set_success(false);

	//if playerShared uid != _managerUID -> false
	playerShared->EnterReadLock();
	int managerPlayerUID = playerShared->GetPlayerUID();
	if (managerPlayerUID != _managerUID)
	{
		playerShared->LeaveReadLock();
		return false;
	}
	playerShared->LeaveReadLock();

	// room is disconn -> false
	FastSpinlockGuard guard(_roomLock, true);
	if (_bClosed == true)
	{
		return false;
	}

	// playerlist lock
	_roomPlayerMapLock.EnterReadLock();
	int readyCnt = 0;
	for (auto elem : _roomReadyMap)
	{
		if (elem.second == true)
		{
			readyCnt++;
		}
	}
	_roomPlayerMapLock.LeaveReadLock();

	if (readyCnt == _userNum - 1)
	{
		// start
		_bStart = true;
		// todo : start game

		// todo : broadcast start packet
		msg.set_success(success);
	}
	else
	{
		// not all player is ready
		return false;
	}

	// todo : if bStart is true... prevent leave room
	return true;
}

bool Room::Chat(std::shared_ptr<Player> playerShared, const char * chat, prop::inRoomChatResponse & msg)
{
	return false;
}
