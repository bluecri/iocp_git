#pragma once

#include "OverlappedContext.h"
#include "Session.h"
#include "origin/ObjectPool.h"
#include "origin\XTL.h"

#define CLIENT_BUFSIZE	65536

class Player;

class ClientSession : public Session, public ObjectPool<ClientSession>
{
public:
	ClientSession(size_t sendBufSize = CLIENT_BUFSIZE, size_t recvBufSize = CLIENT_BUFSIZE);
	virtual ~ClientSession();

	virtual void SessionReset();

	bool PostAccept();
	void AcceptCompletion();

	virtual void DisconnectRequest(DisconnectReason dr);
	virtual void DisconnectCompletion(DisconnectReason dr);
	virtual void OnDisconnect(DisconnectReason dr);
	virtual void OnRelease();

public:
	std::shared_ptr<Player> _sharedPlayer;

private:
	SOCKADDR_IN __clientSockAddr;

	friend class ClientSessionManager;
};
