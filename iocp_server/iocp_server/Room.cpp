#include "stdafx.h"
#include "Room.h"

bool Room::DestroyRoom()
{
	return false;
}

bool Room::LeaveRoom(std::shared_ptr<Player> playerShared, int leaveRoomUID, prop::inRoomLeaveRoomResponse & msg)
{
	return false;
}

bool Room::ReadyRequest(std::shared_ptr<Player> playerShared, bool bReadyReq, prop::inRoomReadyResponse & msg)
{
	return false;
}

bool Room::StartRequest(std::shared_ptr<Player> playerShared, bool bStartReq, prop::inRoomStartResponse & msg)
{
	return false;
}

bool Room::Chat(std::shared_ptr<Player> playerShared, const char * chat, prop::inRoomChatResponse & msg)
{
	return false;
}

int Room::GetUID()
{
	return 0;
}

char * Room::GetRoomName()
{
	return nullptr;
}

Lobby * Room::GetBeforeLobby()
{
	return nullptr;
}

int Room::GetUserMaxNum()
{
	return 0;
}

bool Room::IsClosed()
{
	return false;
}

void Room::SetClosed(bool closed)
{
}
