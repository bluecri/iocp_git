#pragma once

#include "origin\XTL.h"
#include "Player.h"

#define LOBBY_NAME_MAX__LEN 20

class Room;
class OutLobby;

class Lobby
{
public:
	Lobby(OutLobby* beforeLobby, int uid, std::string& lobbyname, int userMaxNum)
		: _beforeLobby(beforeLobby), _uid(uid), _userMaxNum(userMaxNum), _userNum(0),_bClosed(true), _roomMapLock()
	{
		strcpy_s(_lobbyName, lobbyname.c_str());
	};

	//io thread(packetRecvHandler

	bool CreateNewRoom(std::shared_ptr<Player> playerShared, const char* roomName);
	bool EnterRoom(std::shared_ptr<Player> playerShared, int roomUID, prop::inLobbyEnterRoomResponse &msg);
	bool LeaveLobby(std::shared_ptr<Player> playerShared, int lobbyUID, prop::inLobbyLeaveLobbyResponse &msg);
	bool Chat(std::shared_ptr<Player> playerShared, int lobbyUID, const char* chatCStr, prop::inLobbyChatResponse &msg);
	bool GetPlayerList(std::shared_ptr<Player> playerShared, prop::inLobbyPlayerlistResponse &msg);
	bool GetRoomList(std::shared_ptr<Player> playerShared);

public:

	int		GetUID();
	char*	GetLobbyName();
	OutLobby*	GetBeforeLobby();

	int		GetUserMaxNum();

	bool	IsClosed();
	void	SetClosed(bool closed);

	std::shared_ptr<Room> GetRoomShared(int uid);
private:
	OutLobby* _beforeLobby;

	int _uid;
	char _lobbyName[LOBBY_NAME_MAX__LEN];
	int _userMaxNum;
	volatile long _userNum;
	volatile long _bClosed;

	xmap<int, std::shared_ptr<Player>>::type _lobbyPlayerMap;
	FastSpinlock _lobbyPlayerMapLock;

	xmap<int, std::shared_ptr<Room>>::type _roomMap;
	FastSpinlock _roomMapLock;
};