#pragma once

#include "origin\XTL.h"
#include "Player.h"

#define ROOM_NAME_MAX_LEN 20

class Lobby;

class Room
{
public:
	Room(Lobby* beforeLobby, int uid, std::string& Roomname, int userMaxNum)
		: _beforeLobby(beforeLobby), _uid(uid), _userMaxNum(userMaxNum), _userNum(0), _bClosed(true), _roomPlayerMapLock(), _roomLock()
	{
		strcpy(_roomName, Roomname.c_str());
	};

	bool DestroyRoom();
	bool LeaveRoom(std::shared_ptr<Player> playerShared, int leaveRoomUID, prop::inRoomLeaveRoomResponse &msg);

	bool ReadyRequest(std::shared_ptr<Player> playerShared, bool bReadyReq, prop::inRoomReadyResponse &msg);
	bool StartRequest(int uid);

	bool Chat(std::shared_ptr<Player> playerShared, const char* chat, prop::inRoomChatResponse &msg);



public:

	int		GetUID();
	char*	GetRoomName();
	Lobby*	GetBeforeLobby();

	int		GetUserMaxNum();

	bool	IsClosed();
	void	SetClosed(bool closed);

public:
	Lobby* _beforeLobby;

	int _uid;
	char _roomName[ROOM_NAME_MAX_LEN];
	int _userMaxNum;
	volatile long _userNum;
	volatile long _bClosed;

	xmap<int, std::shared_ptr<Player>>::type _roomPlayerMap;
	FastSpinlock _roomPlayerMapLock;

	FastSpinlock _roomLock;
};