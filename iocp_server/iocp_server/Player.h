#pragma once

#include "origin\SyncExecutable.h"

class ClientSession;
class PlayerManager;
class Packet;
class DBPacket;

class Player : public SyncExecutable
{
public:
	Player(ClientSession* session);
	virtual ~Player();

	int GetPlayerId() { return __playerId; }
	bool isConnWIthClient() { return __bClientConn; }
	void Start(int heartbeat);

	void OnTick(); ///< 로그인후 1초마다 불리는 기능

	void PlayerReset();

	/// 플레이어에게 버프를 걸어주는 함수라고 치자.
	void AddBuff(int fromPlayerId, int buffId, int duration);

	/// 주기적으로 버프 시간 업데이트하는 함수
	void DecayTickBuff();

public:	// DB requests
	//send buf
	bool CreateSendBuf(Packet* packet);

	//only for guest
	void RequestNewPlayer(std::string& strID, std::string& strPassword, std::string& strNickName);
	void ResponseNewPlayer();

	void RequestLogin();		//with id & pw
	void ResponseLogin();							//ger ID from DB
	
	//only for login
	void RequestLogout();
	void RequestSaveState();
	void RequestLoadState();

private:
	bool GetClientSessionWithAddRef(ClientSession* cSession);

private:

	int		__playerId;
	int		__heartBeat;

	vec3	__posVec3;

	/// 버프 리스트  GCE
	std::map<int, int> __buffList; ///< (id, time)

	bool __bClientConn;
	ClientSession* __clientSession;
	friend class ClientSession;
	friend class PlayerManager;
};