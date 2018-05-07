#pragma once

#include "origin\SyncExecutable.h"

class ClientSession;
class PlayerManager;
struct Packet;
class RigidbodyComponent;

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

	RigidbodyComponent* GetRigidbodyComponent() { return _rigidbodyComp; }
	

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

	bool GetClientSessionWithAddRef(ClientSession* cSession);
private:

private:

	int			_playerUID;
	char*		_id;
	char*		_nickName;
	int			_heartBeat;

	RigidbodyComponent* _rigidbodyComp;

	/// ���� ����Ʈ  GCE
	std::map<int, int> _buffList; ///< (id, time)

	bool _bClientConn;
	ClientSession* _clientSession;
	friend class ClientSession;
	friend class PlayerManager;
};