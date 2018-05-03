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

	__clientSession->AddRef();	// queue에 session 추가 전, addRef하여 session return 방지
	
	__clientSession->PostSend(packet->GetPacketStart(), packet->GetSize());	//data post 후 concurrnt queue push.
	GSendRequestSessionQueue->PushSession(__clientSession);

	/*
		queue에 session을 넣기 전에 postSend해야 data없이 session pop이 이뤄지는걸 방지할 수 있음.
		player lock과 recvbuffer lock을 연속적으로 사용할 수 밖에 없는가?
	*/

	LeaveReadLock();
	
	return true;
}
