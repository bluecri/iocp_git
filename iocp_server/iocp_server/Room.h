#pragma once

#include "origin\XTL.h"
#include "Player.h"

#define ROOM_NAME_MAX_LEN 20

class Lobby;

class Room
{
public:
	Room(Lobby* beforeLobby, int managerUID, int uid, const char* roomname, int userMaxNum)
		: _beforeLobby(beforeLobby), _uid(uid), _managerUID(managerUID), _userMaxNum(userMaxNum), _userNum(0), _bClosed(false), _roomPlayerMapLock(), _roomLock(), _bStart(false)
	{
		strcpy_s(_roomName, roomname);
	};

	bool DestroyRoom();	//called if roomManager leave room
	bool LeaveRoom(std::shared_ptr<Player> playerShared, prop::inRoomLeaveRoomResponse &msg);

	bool ReadyRequest(std::shared_ptr<Player> playerShared, bool bReadyReq, prop::inRoomReadyResponse &msg);
	bool StartRequest(std::shared_ptr<Player> playerShared, bool bStartReq, prop::inRoomStartResponse &msg);

	bool Chat(std::shared_ptr<Player> playerShared, const char* chat, prop::inRoomChatResponse &msg);



public:

	/* removed	:	must use lock for members
	int		GetUID();
	char*	GetRoomName();
	Lobby*	GetBeforeLobby();

	int		GetUserMaxNum();

	bool	IsClosed();
	void	SetClosed(bool closed);
	*/

public:
	Lobby* _beforeLobby;

	int _uid;
	int _managerUID;
	char _roomName[ROOM_NAME_MAX_LEN];
	int _userMaxNum;
	volatile long _userNum;
	volatile long _bClosed;
	volatile long _bStart;

	xmap<int, std::shared_ptr<Player>>::type _roomPlayerMap;
	xmap<int, bool>::type _roomReadyMap;	// use _roomPlayerMapLock
	FastSpinlock _roomPlayerMapLock;

	FastSpinlock _roomLock;		// 1 lock or 2 lock?
};