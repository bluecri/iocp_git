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

void Player::RequestNewPlayer(std::string& strID, std::string& strPassword, std::string& strNickName)
{
	/*
	ClientSession* targetSession = __clientSession;

	if (false == GetClientSessionWithAddRef(targetSession)) {
		return;
	}

	__clientSession->ReleaseRef();
	*/

	
	OverlappedDBContext* context = new OverlappedDBLogInOutContext(GetSharedFromThis<Player>(), strID, strPassword, strNickName, OverlappedDBLogInOutContext::E_TYPE::CREATE);

}

void Player::ResponseNewPlayer()
{
}

void Player::RequestLogin()
{
}

void Player::ResponseLogin()
{
}

// client와 connection을 check하고 valid 하면 Ref증가 후 cSession 저장.
// 사용시 반드시 releaseRef해야함.
bool Player::GetClientSessionWithAddRef(ClientSession* cSession)
{
	EnterReadLock();

	//check is connected
	if (!isConnWIthClient()) {
		LeaveReadLock();
		return false;
	}

	__clientSession->AddRef();	// addRef하여 session return 방지.

	cSession = __clientSession;

	LeaveReadLock();

	return true;
}

bool Player::CreateSendBuf(Packet* packet)
{
	ClientSession* targetSession = __clientSession;

	if (false == GetClientSessionWithAddRef(targetSession)) {
		return false;
	}

	targetSession->PostSend(packet->GetPacketStart(), packet->GetSize());	//data post 후 concurrnt queue push.
	GSendRequestSessionQueue->PushSession(targetSession);

	//queue에 session 추가 전, addRef. GSendRequestSessionQueue pop시 releaseRef 필수

	/*
		queue에 session을 넣기 전에 postSend해야 data없이 session pop이 이뤄지는걸 방지할 수 있음.
		player lock과 recvbuffer lock을 연속적으로 사용할 수 밖에 없는가?

		=> __clientSession을 따로 빼서 저장하고 이를 사용. Addref는 이미 되었으므로 clientSession 보장.
	*/

	
	return true;
}
