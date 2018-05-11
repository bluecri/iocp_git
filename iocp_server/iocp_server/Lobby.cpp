#include "stdafx.h"
#include "Lobby.h"
#include "Room.h"
#include "LocationInfo.h"
#include "Packet.h"
#include "ContentsConfig.h"

bool Lobby::CreateNewRoom(std::shared_ptr<Player> playerShared, const char * roomName, int maxPlayerNum, prop::inLobbyCreateRoomResponse & msg)
{
	playerShared->EnterReadLock();
	int managerID = playerShared->GetPlayerUID();
	playerShared->LeaveReadLock();

	_roomMapLock.EnterWriteLock();

	// generate random room id
	int roomGenID = -1;
	do {
		roomGenID = glm::linearRand<int>(MIN_LOBBY_ID, MAX_LOBBY_ID);
	} while (_roomMap.end() != _roomMap.find(roomGenID));

	// create room
	std::shared_ptr<Room> roomShared = std::make_shared<Room>(this, roomGenID, managerID, roomName, maxPlayerNum);
	_roomMap.insert(make_pair(roomGenID, roomShared));

	_roomMapLock.LeaveWriteLock();

	// playerShared -> enter room
	playerShared->EnterWriteLock();
	roomShared->_roomLock.EnterWriteLock();

	playerShared->GetLocInfo()->_roomUID = roomGenID;

	roomShared->_userNum += 1;
	roomShared->_roomPlayerMapLock.EnterWriteLock();
	roomShared->_roomPlayerMap.insert(std::make_pair(playerShared->GetPlayerUID(), playerShared));
	roomShared->_roomPlayerMapLock.LeaveWriteLock();

	roomShared->_roomLock.LeaveWriteLock();
	playerShared->LeaveWriteLock();

	msg.set_success(true);
	msg.set_roomuid(roomGenID);
	msg.set_roomname(roomName);

	return true;
}

bool Lobby::EnterRoom(std::shared_ptr<Player> playerShared, int roomUID, prop::inLobbyEnterRoomResponse & msg)
{
	msg.set_roomuid(roomUID);
	
	// get target room in roomList
	std::shared_ptr<Room> targetRoom = nullptr;

	_roomMapLock.EnterReadLock();
	auto it = _roomMap.find(roomUID);
	if (_roomMap.end() == it)	//fail to find room
	{
		_roomMapLock.LeaveReadLock();
		msg.set_success(false);
		return false;
	}
	targetRoom = it->second;

	_roomMapLock.LeaveReadLock();

	FastSpinlockGuard guard(targetRoom->_roomLock, true);

	// check room is closed
	if (targetRoom->_bClosed == true)
	{
		msg.set_success(false);
		return false;
	}

	// check room is full
	if (targetRoom->_userMaxNum >= targetRoom->_userNum)
	{
		msg.set_success(false);
		return false;
	}
	
	playerShared->EnterWriteLock();

	// check player is in lobby, not room
	if (playerShared->GetLocInfo()->_roomUID != -1)
	{
		msg.set_success(false);
		playerShared->LeaveWriteLock();
		return false;
	}

	// player room uid update
	playerShared->GetLocInfo()->_roomUID = targetRoom->_uid);

	// room player list add
	targetRoom->_userNum += 1;
	targetRoom->_roomPlayerMapLock.EnterWriteLock();
	targetRoom->_roomPlayerMap.insert(std::make_pair(playerShared->GetPlayerUID(), playerShared));
	targetRoom->_roomPlayerMapLock.LeaveWriteLock();

	// lobby player list remove
	_lobbyPlayerMapLock.EnterWriteLock();
	_lobbyPlayerMap.erase(playerShared->GetPlayerUID());
	_lobbyPlayerMapLock.LeaveWriteLock();

	playerShared->LeaveWriteLock();

	msg.set_success(true);

	return true;;
}

bool Lobby::LeaveLobby(std::shared_ptr<Player> playerShared, int targetLobbyUID, prop::inLobbyLeaveLobbyResponse & msg)
{
	playerShared->EnterWriteLock();
	_lobbyPlayerMapLock.EnterWriteLock();

	int playerLobbyUID = playerShared->GetLocInfo()->_lobbyUID;

	// player의 현재 lobby 위치와 나가고자하는 lobby 불일치 || lobby player list에 player 없는 경우
	if (playerLobbyUID != targetLobbyUID || _lobbyPlayerMap.end() == _lobbyPlayerMap.find(playerLobbyUID))
	{
		msg.set_success(false);
		msg.set_lobbyuid(targetLobbyUID);
		return false;
	}

	// leave lobby : player lobby info -> -1 && erase player from lobby player list
	playerShared->GetLocInfo()->_lobbyUID = -1;
	_lobbyPlayerMap.erase(playerShared->GetPlayerUID());

	_lobbyPlayerMapLock.LeaveWriteLock();
	playerShared->LeaveWriteLock();

	msg.set_success(false);
	msg.set_lobbyuid(targetLobbyUID);

	return true;
}

bool Lobby::Chat(std::shared_ptr<Player> playerShared, int lobbyUID, const char * chatCStr, prop::inLobbyChatResponse & msg)
{
	// creaet chat with nickname appended
	char broadChatStr[MAX_CHAT_LEN + PLAYER_NICKNAME_MAX_LEN + 4];
	int nickNameLen = strlen(playerShared->GetPlayerNIckName());

	playerShared->EnterReadLock();
	strcpy_s(broadChatStr, playerShared->GetPlayerNIckName());
	playerShared->LeaveReadLock();

	broadChatStr[nickNameLen] = ' ';
	broadChatStr[nickNameLen + 1] = ':';
	broadChatStr[nickNameLen + 2] = ' ';
	int cpySize = min(strlen(chatCStr), size_t(MAX_CHAT_LEN));
	strcpy_s(broadChatStr + nickNameLen + 3, cpySize, chatCStr);
	broadChatStr[cpySize + nickNameLen + 3] = '\0';

	prop::inLobbyChatBroadcast broadMsg;
	broadMsg.set_lobbyuid(lobbyUID);
	broadMsg.set_chat(broadChatStr);

	Packet pack(PACKET_TYPE::PACKET_TYPE_inLobbyChatBroadcast, broadMsg);

	//broadcast all player in lobby
	_lobbyPlayerMapLock.EnterReadLock();
	for (auto elem : _lobbyPlayerMap)
	{
		elem.second->SendToClient(&pack);
	}

	_lobbyPlayerMapLock.LeaveReadLock();

	msg.set_success(true);
	return true;
}

bool Lobby::GetPlayerList(std::shared_ptr<Player> playerShared, prop::inLobbyPlayerlistResponse & msg)
{
	msg.set_success(true);
	
	std::vector<std::shared_ptr<Player>> cachePlayerListInLobbyVec;

	// TODO update tick
	/*
	_playerListResponseMsgLock.EnterWriteLock();
	playerListResponseMsg->clear_userlist();

	// copy shared_ptr from player list
	_lobbyPlayerMapLock.EnterReadLock();
	cachePlayerListInLobbyVec.clear();
	for (auto elem : _lobbyPlayerMap)
	{
		cachePlayerListInLobbyVec.push_back(elem.second);
	}
	_lobbyPlayerMapLock.LeaveReadLock();

	//create lobby player list msg
	for (auto elem : cachePlayerListInLobbyVec)
	{
		prop::msgUserInfo* msgUserInfo = playerListResponseMsg->add_userlist();

		elem->EnterReadLock();
		msgUserInfo->set_uid(elem->GetPlayerUID());
		msgUserInfo->set_id(elem->GetPlayerID());
		msgUserInfo->set_nickname(elem->GetPlayerNIckName());
		elem->LeaveReadLock();
	}
	msg.set_lobbyuid(_uid);
	msg.set_success(true);

	_playerListResponseMsgLock.LeaveWriteLock();
	
	*/

	_playerListResponseMsgLock.EnterReadLock();
	msg.CopyFrom(*_playerListResponseMsg);
	_playerListResponseMsgLock.LeaveReadLock();
	return true;
}

bool Lobby::GetRoomList(std::shared_ptr<Player> playerShared)
{
	// todo
	return false;
}

int Lobby::GetUID()
{
	return _uid;
}

char * Lobby::GetLobbyName()
{
	return _lobbyName;
}

int Lobby::GetUserMaxNum()
{
	return _userMaxNum;
}

int Lobby::GetUserNum()
{
	return _userNum;
}

int Lobby::InterlockAddUserNum(int n)
{
	return InterlockedAdd(&_userNum, n);
}

bool Lobby::IsClosed()
{
	return _bClosed;
}

void Lobby::SetClosed(bool closed)
{
	InterlockedExchange(&_bClosed, (long)closed);
}

std::shared_ptr<Room> Lobby::GetRoomShared(int uid)
{
	FastSpinlockGuard guard(_roomMapLock, false);
	auto it = _roomMap.find(uid);

	if (it == roomMap.end())
	{
		return nullptr;
	}

	return it->second;
}
