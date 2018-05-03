#pragma once

#define LISTEN_PORT		4646
#define MAX_CLIENT_CONNECTION	10000

#define CONNECT_SERVER_ADDR	"127.0.0.1"
#define CONNECT_SERVER_PORT 1
#define SQL_SERVER_CONN_STR	L"Driver={SQL Server};Server=127.0.0.1\\SQL4GAMESERVER;Database=GameDB;UID=sa;PWD=...."

#define GQCS_TIMEOUT	10 //INFINITE
#define MAX_IO_THREAD	4


class IOThread;
class IOverlappedContext;


class IocpManager
{
public:
	IocpManager();
	virtual ~IocpManager();

	bool Initialize();
	void Finalize();

	bool StartIoThreads();
	void StartAcceptLoop();

	void PostContext(IOverlappedContext * context);

public:
	HANDLE GetHandleCompletionPort() { return __hCompletionPort; }
	SOCKET* GetPtrListenSocket() { return &__listenSocket; }

	static char _schAcceptBuf[64];
	static LPFN_ACCEPTEX _slpfnAcceptEx;
	static LPFN_CONNECTEX _slpfnConnectEx;
	static LPFN_DISCONNECTEX _slpfnDisconnectEx;

private:
	static unsigned int WINAPI FnIoWorkerThread(LPVOID lpParam);

private:
	HANDLE __hCompletionPort;
	SOCKET __listenSocket;

	IOThread* __ioWorkerThread[MAX_IO_THREAD];
};


extern IocpManager* GIocpManager;


