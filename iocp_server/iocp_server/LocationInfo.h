#pragma once

class LocationInfo
{
public:
	LocationInfo() : _lobbyUID(-1), _roomUID(-1)
	{};

	int _lobbyUID;
	int _roomUID;
};