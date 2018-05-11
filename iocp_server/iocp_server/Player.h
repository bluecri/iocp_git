#pragma once

#include "origin\SyncExecutable.h"

class ClientSession;
class PlayerManager;
class Packet;
class RigidbodyComponent;
class LocationInfo;

class Player : public SyncExecutable
{
public:
	Player(ClientSession* session);
	virtual ~Player();

	bool isConnWIthClient() { return _bClientConn; }
	void Start(int heartbeat);

	void OnTick(); ///< �α����� 1�ʸ��� �Ҹ��� ���

	void PlayerReset();

	/// �÷��̾�� ������ �ɾ��ִ� �Լ���� ġ��.
	void AddBuff(int fromPlayerId, int buffId, int duration);

	/// �ֱ������� ���� �ð� ������Ʈ�ϴ� �Լ�
	void DecayTickBuff();



	int GetPlayerUID() { return _playerUID; }
	char* GetPlayerID() { return _id; }
	char* GetPlayerNIckName() { return _nickName; }
	LocationInfo* GetLocInfo() { return _locInfo; }

	RigidbodyComponent* GetRigidbodyComponent() { return _rigidbodyComp; }
	

public:	// DB requests
	//only for guest
	void RequestNewPlayer(const std::string& strID, const std::string& strPassword, const std::string& strNickName);
	void RequestLogin(const std::string& id, const std::string& password);		//with id & pw	->call playmanager.memfunc
	void RequestLogout();

	void RequestUpdateGameInfo();
	void RequestLoadGameInfo();
	void RequestOtherPlayerInfoWithUID(int uid = -1);
	void RequestOtherPlayerInfoWithID(const char* id);
	void RequestOtherPlayerInfoWithNIckname(const char* nickName);


	bool GetClientSessionWithAddRef(ClientSession* cSession);

	//send buf
	bool SendToClient(Packet* packet);
	
private:
	
	long volatile	_playerUID;
	char* volatile	_id;
	char* volatile 	_nickName;
	long volatile	_heartBeat;

	LocationInfo* _locInfo;
	RigidbodyComponent* _rigidbodyComp;

	/// ���� ����Ʈ  GCE
	std::map<int, int> _buffList; ///< (id, time)

	bool _bClientConn;
	ClientSession* _clientSession;
	friend class ClientSession;
	friend class PlayerManager;
};