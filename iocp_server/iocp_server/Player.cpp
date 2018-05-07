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

// client�� connection�� check�ϰ� valid �ϸ� Ref���� �� cSession ����.
// ���� �ݵ�� releaseRef�ؾ���.
bool Player::GetClientSessionWithAddRef(ClientSession* cSession)
{
	EnterReadLock();

	//check is connected
	if (!isConnWIthClient()) {
		LeaveReadLock();
		return false;
	}

	__clientSession->AddRef();	// addRef�Ͽ� session return ����.

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

	targetSession->PostSend(packet->GetPacketStart(), packet->GetSize());	//data post �� concurrnt queue push.
	GSendRequestSessionQueue->PushSession(targetSession);

	//queue�� session �߰� ��, addRef. GSendRequestSessionQueue pop�� releaseRef �ʼ�

	/*
		queue�� session�� �ֱ� ���� postSend�ؾ� data���� session pop�� �̷����°� ������ �� ����.
		player lock�� recvbuffer lock�� ���������� ����� �� �ۿ� ���°�?

		=> __clientSession�� ���� ���� �����ϰ� �̸� ���. Addref�� �̹� �Ǿ����Ƿ� clientSession ����.
	*/

	
	return true;
}
