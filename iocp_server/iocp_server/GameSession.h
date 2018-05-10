#pragma once

#include "origin\XTL.h"
#include "Player.h"

class GameSession
{
public:
	
private:
	xmap<int, std::shared_ptr<Player>>::type _playerMap;
	FastSpinlock _PlayerMapLock;

};