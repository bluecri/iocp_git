#pragma once

#include "origin\XTL.h"
#include "Player.h"

#define LOBBY_NAME_MAX__LEN 20
#define MIN_LOBBY_ID	1
#define MAX_LOBBY_ID	(1<<30)

class Room;
class OutLobby;

class Lobby
{
public:
	Lobby(OutLobby* beforeLobby, int uid, std::string& lobbyname, int userMaxNum)
		: _uid(uid), _userMaxNum(userMaxNum), _userNum(0),_bClosed(true), _roomMapLock()
	{
		strcpy_s(_lobbyName, lobbyname.c_str());
		playerListResponseMsg = new prop::inLobbyPlayerlistResponse();
		playerListResponseMsg->set_lobbyuid(uid);
		playerListResponseMsg->set_success(true);
	};

	//io thread(packetRecvHandler)

	bool CreateNewRoom(std::shared_ptr<Player> playerShared, const char* roomName, int maxPlayerNum, prop::inLobbyCreateRoomResponse &msg);
	bool EnterRoom(std::shared_ptr<Player> playerShared, int roomUID, prop::inLobbyEnterRoomResponse &msg);
	bool LeaveLobby(std::shared_ptr<Player> playerShared, int lobbyUID, prop::inLobbyLeaveLobbyResponse &msg);
	bool Chat(std::shared_ptr<Player> playerShared, int lobbyUID, const char* chatCStr, prop::inLobbyChatResponse &msg);
	bool GetPlayerList(std::shared_ptr<Player> playerShared, prop::inLobbyPlayerlistResponse &msg);
	bool GetRoomList(std::shared_ptr<Player> playerShared);

public:

	int		GetUID();
	char*	GetLobbyName();
	//OutLobby*	GetBeforeLobby();

	int		GetUserMaxNum();
	int		GetUserNum();
	int		InterlockAddUserNum(int n);

	bool	IsClosed();
	void	SetClosed(bool closed);

	std::shared_ptr<Room> GetRoomShared(int uid);
private:
	//OutLobby* _beforeLobby;

	int _uid;
	char _lobbyName[LOBBY_NAME_MAX__LEN];
	int _userMaxNum;
	volatile long _userNum;
	volatile long _bClosed;

	xmap<int, std::shared_ptr<Player>>::type _lobbyPlayerMap;
	// std::vector<std::shared_ptr<Player>> cachePlayerListInLobbyVec;
	prop::inLobbyPlayerlistResponse* playerListResponseMsg;	// msg를 아예 생성해놓는다.

	FastSpinlock _lobbyPlayerMapLock;
	FastSpinlock _playerListResponseMsgLock;
	// FastSpinlock _cachePlayerListInLobbyVecLock;

	xmap<int, std::shared_ptr<Room>>::type _roomMap;
	FastSpinlock _roomMapLock;

	friend class OutLobby;
	friend class Room;
};