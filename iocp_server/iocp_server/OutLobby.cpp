#include "stdafx.h"
#include "OutLobby.h"

OutLobby *GOutLobby = nullptr;


bool OutLobby::EnterLobby(std::shared_ptr<Player> playerShared, int lobbyUID, prop::outLobbyEnterLobbyResponse & msg)
{

	msg.set_success(success);
	if (success)
	{
		msg.set_lobbyuid(outLobbyEnterLobbyRequest->lobbyuid);
	}
	else
	{
		msg.set_lobbyuid(-1);
	}

	return false;
}

bool OutLobby::GetLobbyList(std::shared_ptr<Player> playerShared, prop::outLobbyLobbylistResponse & msg)
{
	return false;
}

