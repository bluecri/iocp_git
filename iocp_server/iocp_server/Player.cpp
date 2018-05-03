#include "stdafx.h"
#include "Player.h"
#include "Packet.h"
#include "ClientSession.h"
#include "SendRequestSessionConcurrentQueue.h"

Player::Player(ClientSession * session) : __clientSession(session)
{
	PlayerReset();
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
	__playerId = -1;
	__heartBeat = -1;

	__buffList.clear(); ///< (id, time)

	__clientSession = nullptr;
}

void Player::AddBuff(int fromPlayerId, int buffId, int duration)
{
}

void Player::DecayTickBuff()
{
}

void Player::RequestNewPlayer(DBPacket * dbPacket)
{
}

void Player::ResponseNewPlayer()
{
}

void Player::RequestLogin(DBPacket * dbPacket)
{
}

void Player::ResponseLogin()
{
}

bool Player::CreateSendBuf(Packet* packet)
{
	EnterReadLock();

	//check is connected
	if (!(__clientSession != nullptr && __clientSession->IsConnected())) {
		LeaveReadLock();
		return false;
	}

	__clientSession->AddRef();	// queue�� session �߰� ��, addRef�Ͽ� session return ����
	
	__clientSession->PostSend(packet->GetPacketStart(), packet->GetSize());	//data post �� concurrnt queue push.
	GSendRequestSessionQueue->PushSession(__clientSession);

	/*
		queue�� session�� �ֱ� ���� postSend�ؾ� data���� session pop�� �̷����°� ������ �� ����.
		player lock�� recvbuffer lock�� ���������� ����� �� �ۿ� ���°�?
	*/

	LeaveReadLock();
	
	return true;
}
