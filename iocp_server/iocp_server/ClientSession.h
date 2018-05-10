#pragma once

#include "OverlappedContext.h"
#include "Session.h"
#include "origin/ObjectPool.h"
#include "origin\XTL.h"

#define CLIENT_BUFSIZE	65536

class Player;
class PacketRecvHandler;
class ClientSessionManager;

class ClientSession : public Session, public ObjectPool<ClientSession>
{
public:
	ClientSession(size_t sendBufSize = CLIENT_BUFSIZE, size_t recvBufSize = CLIENT_BUFSIZE);
	virtual ~ClientSession();

	virtual void SessionReset();

	bool PostAccept();
	void AcceptCompletion();

	virtual SessionErrType RecvCompletion(DWORD transferred);

	virtual void DisconnectRequest(DisconnectReason dr);
	virtual void DisconnectCompletion(DisconnectReason dr);
	virtual void OnDisconnect(DisconnectReason dr);
	virtual void OnRelease();

	virtual SessionErrType PostSend(Packet* packet);
	SessionErrType PostSend(Packet & packet);
public:
	std::shared_ptr<Player> _sharedPlayer;

protected:
	
private:
	SOCKADDR_IN __clientSockAddr;

	//use PostSend class
	//friend class PacketRecvHandler;
	//friend class ClientSessionManager;
	//friend class Player;
};
