#include "stdafx.h"
#include "Lobby.h"

bool Lobby::CreateNewRoom(std::shared_ptr<Player> playerShared, const char * roomName)
{
	return false;
}

bool Lobby::EnterRoom(std::shared_ptr<Player> playerShared, int roomUID, prop::inLobbyEnterRoomResponse & msg)
{
	return false;
}

bool Lobby::LeaveLobby(std::shared_ptr<Player> playerShared, int lobbyUID, prop::inLobbyLeaveLobbyResponse & msg)
{
	return false;
}

bool Lobby::Chat(std::shared_ptr<Player> playerShared, int lobbyUID, const char * chatCStr, prop::inLobbyChatResponse & msg)
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
	return 0;
}

char * Lobby::GetLobbyName()
{
	return nullptr;
}

OutLobby * Lobby::GetBeforeLobby()
{
	return nullptr;
}

int Lobby::GetUserMaxNum()
{
	return 0;
}

bool Lobby::IsClosed()
{
	return false;
}

void Lobby::SetClosed(bool closed)
{
}

std::shared_ptr<Room> Lobby::GetRoomShared(int uid)
{
	return std::shared_ptr<Room>();
}
