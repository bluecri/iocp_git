#pragma once

#include "origin\XTL.h"
#include "Player.h"


#define MIN_GUEST_ID	(1<<30)
#define MAX_GUEST_ID	~(1<<31)

class Packet;
class ClientSession;

class PlayerManager
{
public:
	PlayerManager() {}
	virtual ~PlayerManager() {}

	std::shared_ptr<Player> GetGuestPlayer(ClientSession* session);
	void UnregisterGuestPlayer(int plyaerID);

	void RegisterLoginPlayer(std::shared_ptr<Player> loginPlayerShared);
	void UnregisterLoginPlayer(int plyaerID);

private:
	FastSpinlock __loginPlayerMaplock;
	FastSpinlock __guestPlayerMaplock;

	xmap<int, std::shared_ptr<Player>>::type __loginPlayerMap;
	xmap<int, std::shared_ptr<Player>>::type __guestPlayerMap;

};

extern PlayerManager* GPlayerManager;