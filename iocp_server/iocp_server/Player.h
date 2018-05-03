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
	bool isConnWIthClient() { return (__clientSession != nullptr); }
	void Start(int heartbeat);

	void OnTick(); ///< �α����� 1�ʸ��� �Ҹ��� ���

	void PlayerReset();

	/// �÷��̾�� ������ �ɾ��ִ� �Լ���� ġ��.
	void AddBuff(int fromPlayerId, int buffId, int duration);

	/// �ֱ������� ���� �ð� ������Ʈ�ϴ� �Լ�
	void DecayTickBuff();

public:	// DB requests
	//send buf
	bool CreateSendBuf(Packet* packet);

	//only for guest
	void RequestNewPlayer(DBPacket* dbPacket);	
	void ResponseNewPlayer();

	void RequestLogin(DBPacket* dbPacket);		//with id & pw
	void ResponseLogin();							//ger ID from DB
	
	//only for login
	void RequestLogout(DBPacket* dbPacket);
	void RequestSaveState();
	void RequestLoadState();

private:


private:

	int		__playerId;
	int		__heartBeat;

	vec3	__posVec3;

	/// ���� ����Ʈ  GCE
	std::map<int, int> __buffList; ///< (id, time)

	ClientSession* __clientSession;
	friend class ClientSession;
	friend class PlayerManager;
};