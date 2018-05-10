#include "stdafx.h"
#include "Player.h"
#include "Packet.h"
#include "ClientSession.h"
#include "origin\ThreadLocal.h"
#include "SendRequestSessionConcurrentQueue.h"

#include "DBContext.h"
#include "PlayerDBContext.h"
#include "DBManager.h"
#include "LocationInfo.h"

Player::Player(ClientSession * session) : _clientSession(session)
{
	PlayerReset();
	_locInfo = new LocationInfo();
}

Player::~Player()
{
}

void Player::Start(int heartbeat)
{
}

void Player::OnTick()
{
}

void Player::PlayerReset()
{
	_playerUID = -1;
	_heartBeat = -1;

	_buffList.clear(); ///< (id, time)

	_clientSession = nullptr;
}

void Player::AddBuff(int fromPlayerId, int buffId, int duration)
{
}

void Player::DecayTickBuff()
{
}

void Player::RequestNewPlayer(std::string& strID, std::string& strPassword, std::string& strNickName)
{
	//packet recv -> requestNewPlayer : client�� �׻� live ������. (ref ��� x)
	OverlappedDBLogInOutContext* context = new OverlappedDBLogInOutContext(GetSharedFromThis<Player>());
	context->Init_create(strID, strPassword, strNickName);
	
	GDatabaseManager->PostDatabaseRequest(context);
}


void Player::RequestLogin(const std::string& id, const std::string& password)
{
	OverlappedDBLogInOutContext* context = new OverlappedDBLogInOutContext(GetSharedFromThis<Player>());
	context->Init_login(id, password);
	GDatabaseManager->PostDatabaseRequest(context);

	return;
}

void Player::RequestLogout()
{
	OverlappedDBLogInOutContext* context = new OverlappedDBLogInOutContext(GetSharedFromThis<Player>());
	context->Init_logout();
	GDatabaseManager->PostDatabaseRequest(context);

	return;
}

void Player::RequestUpdateGameInfo()
{
	OverlappedDBUpdatePlayerContext* context = new OverlappedDBUpdatePlayerContext(GetSharedFromThis<Player>());
	context->init_updatePlayerPosition();
	GDatabaseManager->PostDatabaseRequest(context);

	return;
}

void Player::RequestLoadGameInfo()
{
	OverlappedDBUpdatePlayerContext* context = new OverlappedDBUpdatePlayerContext(GetSharedFromThis<Player>());
	context->init_loadPlayerPosition();
	GDatabaseManager->PostDatabaseRequest(context);

	return;
}

void Player::RequestOtherPlayerInfoWithUID(int uid = -1)
{
	OverlappedDBUpdatePlayerContext* context = new OverlappedDBUpdatePlayerContext(GetSharedFromThis<Player>());
	context->init_loadPlayerWithUID(uid);
	GDatabaseManager->PostDatabaseRequest(context);

	return;
}

void Player::RequestOtherPlayerInfoWithID(const char* id)
{
	OverlappedDBUpdatePlayerContext* context = new OverlappedDBUpdatePlayerContext(GetSharedFromThis<Player>());
	context->init_loadPlayerWithID(id);
	GDatabaseManager->PostDatabaseRequest(context);

	return;
}

void Player::RequestOtherPlayerInfoWithNIckname(const char* nickName)
{
	OverlappedDBUpdatePlayerContext* context = new OverlappedDBUpdatePlayerContext(GetSharedFromThis<Player>());
	context->init_loadPlayerWithNickName(nickName);
	GDatabaseManager->PostDatabaseRequest(context);

	return;
}



// client�� connection�� check�ϰ� valid �ϸ� Ref���� �� cSession ����.
// ���� �ݵ�� releaseRef�ؾ���.	//Player::CreateSendBuf ���� ���.
bool Player::GetClientSessionWithAddRef(ClientSession* cSession)
{
	
	EnterReadLock();

	//check is connected
	if (!isConnWIthClient()) {
		LeaveReadLock();
		cSession = nullptr;
		return false;
	}

	_clientSession->AddRef();	// addRef�Ͽ� session return ����.

	cSession = _clientSession;

	LeaveReadLock();

	return true;
}

//recv���� context �����ϹǷ� �ٷ� postSend. ������ϸ� sendtoclient��..
bool Player::SendToClient(Packet* packet)
{
	ClientSession* targetSession = _clientSession;

	//queue�� session �߰� ��, addRef. GSendRequestSessionQueue pop�� releaseRef �ʼ�
	if (false == GetClientSessionWithAddRef(targetSession)) {
		return false;
	}

	targetSession->PostSend(packet);	//data post �� concurrnt queue push.
	
	switch (LThreadType)
	{
		case THREAD_TYPE::THREAD_IO_WORKER:
			LSendRequestSessionQueue[LSendRequestSessionQueueIndex]->push(targetSession);
			break;
		default:
			GSendRequestSessionQueue->PushSession(targetSession);
			break;
	}


	/*
		queue�� session�� �ֱ� ���� postSend�ؾ� data���� session pop�� �̷����°� ������ �� ����.
		player lock�� recvbuffer lock�� ���������� ����� �� �ۿ� ���°�?

		=> __clientSession�� ���� ���� �����ϰ� �̸� ���. Addref�� �̹� �Ǿ����Ƿ� clientSession ����.
	*/

	// sendToClient �õ� �� client ���� ������ false�� ��ȯ �� �� ������ �̴� ������ �ƴ�.
	return true;
}
