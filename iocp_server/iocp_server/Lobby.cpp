#include "stdafx.h"
#include "Lobby.h"

bool Lobby::CreateNewRoom(std::shared_ptr<Player> playerShared, const char * roomName)
{
	return false;
}

bool Lobby::EnterRoom(std::shared_ptr<Player> playerShared, int roomUID)
{
	return false;
}

bool Lobby::LeaveLobby(std::shared_ptr<Player> playerShared)
{
	return false;
}

bool Lobby::Chat(std::shared_ptr<Player> playerShared, const char * chatCStr)
{
	return false;
}

bool Lobby::GetPlayerList(std::shared_ptr<Player> playerShared, prop::inLobbyPlayerlistResponse & msg)
{
	return false;
}

bool Lobby::GetRoomList(std::shared_ptr<Player> playerShared)
{
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

OutLobby * Lobby::GetBeforeLobby()
{
	return _beforeLobby;
}

int Lobby::GetUserMaxNum()
{
	return _userMaxNum;
}

bool Lobby::IsClosed()
{
	return false;
}

void Lobby::SetClosed(bool closed)
{
}
