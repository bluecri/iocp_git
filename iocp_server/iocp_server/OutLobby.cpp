#include "stdafx.h"
#include "OutLobby.h"

OutLobby *GOutLobby = nullptr;

bool OutLobby::EnterLobby(std::shared_ptr<Player> playerShared, int lobbyUID, prop::outLobbyEnterLobbyResponse & msg)
{
	return false;
}

bool OutLobby::GetLobbyList(std::shared_ptr<Player> playerShared, prop::outLobbyLobbylistResponse & msg)
{
	return false;
}
