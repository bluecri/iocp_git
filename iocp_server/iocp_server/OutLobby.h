#pragma once

#include "origin\XTL.h"
#include "Player.h"

#define LOBBY_NAME_MAX__LEN 20

class Lobby;

class OutLobby
{
public:
	OutLobby(int userMaxNum)
		: _userMaxNum(userMaxNum), _userNum(0)
	{
	};

	// IO Thread work
	bool EnterLobby(std::shared_ptr<Player> playerShared, int lobbyUID, prop::outLobbyEnterLobbyResponse& msg);
	bool GetLobbyList(std::shared_ptr<Player> playerShared, prop::outLobbyLobbylistResponse &msg);

public:

	int _userMaxNum;
	volatile long _userNum;

	// xmap<int, std::shared_ptr<Player>>::type _lobbyPlayerMap;
	// use loginPlayerMap in GPlayerManager

	xmap<int, Lobby*>::type _lobbyMap;
};

extern OutLobby *GOutLobby;
