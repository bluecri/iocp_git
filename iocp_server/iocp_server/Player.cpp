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
	//packet recv -> requestNewPlayer : client가 항상 live 상태임. (ref 고려 x)
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



// client와 connection을 check하고 valid 하면 Ref증가 후 cSession 저장.
// 사용시 반드시 releaseRef해야함.	//Player::CreateSendBuf 에서 사용.
bool Player::GetClientSessionWithAddRef(ClientSession* cSession)
{
	
	EnterReadLock();

	//check is connected
	if (!isConnWIthClient()) {
		LeaveReadLock();
		cSession = nullptr;
		return false;
	}

	_clientSession->AddRef();	// addRef하여 session return 방지.

	cSession = _clientSession;

	LeaveReadLock();

	return true;
}

//recv에서 context 보장하므로 바로 postSend. 보장못하면 sendtoclient로..
bool Player::SendToClient(Packet* packet)
{
	ClientSession* targetSession = _clientSession;

	//queue에 session 추가 전, addRef. GSendRequestSessionQueue pop시 releaseRef 필수
	if (false == GetClientSessionWithAddRef(targetSession)) {
		return false;
	}

	targetSession->PostSend(packet);	//data post 후 concurrnt queue push.
	
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
		queue에 session을 넣기 전에 postSend해야 data없이 session pop이 이뤄지는걸 방지할 수 있음.
		player lock과 recvbuffer lock을 연속적으로 사용할 수 밖에 없는가?

		=> __clientSession을 따로 빼서 저장하고 이를 사용. Addref는 이미 되었으므로 clientSession 보장.
	*/

	// sendToClient 시도 중 client 접속 해제시 false가 반환 될 수 있으며 이는 오류가 아님.
	return true;
}
