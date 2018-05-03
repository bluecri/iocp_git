#pragma once

class ServerSession
{
	virtual void DisconnectRequest(DisconnectReason dr);
	virtual void DisconnectCompletion(DisconnectReason dr);
	virtual void OnDisconnect(DisconnectReason dr);
};