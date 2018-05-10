#include "stdafx.h"

#include "IocpManager.h"
#include "IOThread.h"
#include "ClientSessionManager.h"
#include "origin\ThreadLocal.h"
#include "DBContext.h"


//static & global
IocpManager* GIocpManager = nullptr;

LPFN_ACCEPTEX IocpManager::_slpfnAcceptEx = nullptr;
LPFN_CONNECTEX IocpManager::_slpfnConnectEx = nullptr;
LPFN_DISCONNECTEX IocpManager::_slpfnDisconnectEx = nullptr;



char IocpManager::_schAcceptBuf[64] = { 0, };

IocpManager::IocpManager() : __hCompletionPort(nullptr), __listenSocket(0)
{
	memset(__ioWorkerThread, 0, sizeof(__ioWorkerThread));
}

IocpManager::~IocpManager()
{
	
}

bool IocpManager::Initialize()
{
	/// winsock initializing
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	/// Create I/O Completion Port
	__hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (__hCompletionPort == NULL)
		return false;

	/// create TCP socket
	__listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (__listenSocket == INVALID_SOCKET)
		return false;

	// Match socket - I/O completion port
	HANDLE handle = CreateIoCompletionPort((HANDLE)__listenSocket, __hCompletionPort, 0, 0);
	if (handle != __hCompletionPort)
	{
		printf_s("[DEBUG] listen socket IOCP register error: %d\n", GetLastError());
		return false;
	}

	// set socket option : SO_REUSEADDR
	int opt = 1;
	setsockopt(__listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));

	/// bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(LISTEN_PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == bind(__listenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)))
		return false;

	// Accept, Connect, Disconnect function create with socket
	DWORD bytes = 0;

	
	GUID guidAcceptEx = WSAID_ACCEPTEX;
	if (SOCKET_ERROR == WSAIoctl(__listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(GUID), &_slpfnAcceptEx, sizeof(LPFN_ACCEPTEX), &bytes, NULL, NULL))
		return false;

	GUID guidConnectEx = WSAID_CONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(__listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidConnectEx, sizeof(GUID), &_slpfnConnectEx, sizeof(LPFN_CONNECTEX), &bytes, NULL, NULL))
		return false;

	GUID guidDisconnectEx = WSAID_DISCONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(__listenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidDisconnectEx, sizeof(GUID), &_slpfnDisconnectEx, sizeof(LPFN_DISCONNECTEX), &bytes, NULL, NULL))
		return false;
	

	/// make Global session pool
	GClientSessionManager->PrepareSessions();

	return true;
}

void IocpManager::Finalize()
{
	for (INT32 i = 0; i < MAX_IO_THREAD; i++) {
		CloseHandle(__ioWorkerThread[i]->GetHandle());
	}
	CloseHandle(__hCompletionPort);

	WSACleanup();

	//HANDLE __hCompletionPort;
	//SOCKET __listenSocket;
	//IOThread* __ioWorkerThread[MAX_IO_THREAD];
}

bool IocpManager::StartIoThreads()
{
	//create threads
	for (INT32 i = 0; i < MAX_IO_THREAD; i++) {
		DWORD dwThreadID;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, FnIoWorkerThread, (LPVOID)(IntToPtr(i)), CREATE_SUSPENDED, (unsigned int*)&dwThreadID);
		if (hThread == NULL) {
			return false;
		}
		__ioWorkerThread[i] = new IOThread(hThread, __hCompletionPort);
	}

	// resume threads
	for (INT32 i = 0; i < MAX_IO_THREAD; i++) {
		ResumeThread(__ioWorkerThread[i]->GetHandle());
	}

	return true;
}

void IocpManager::StartAcceptLoop()
{
	/// listen start
	if (SOCKET_ERROR == listen(__listenSocket, SOMAXCONN))
	{
		printf_s("[DEBUG] listen error\n");
		return;
	}

	// accept loop start
	while (GClientSessionManager->AcceptSessions())
	{
		Sleep(100);
	}
}


void IocpManager::PostContext(IOverlappedContext * context)
{
	if (FALSE == PostQueuedCompletionStatus(__hCompletionPort, 0, (ULONG_PTR)0, (LPOVERLAPPED)context)) 
	{
		printf_s("IocpManager::PostContext PostQueuedCompletionStatus Error: %d\n", GetLastError());
		CRASH_ASSERT(false);
	}
}

void IocpManager::PostDatabaseResult(OverlappedDBContext * context)
{
	if (FALSE == PostQueuedCompletionStatus(__hCompletionPort, 0, (ULONG_PTR)CK_DB_REQUEST, (LPOVERLAPPED)context))
	{
		printf_s("IocpManager::PostDatabaseResult PostQueuedCompletionStatus Error: %d\n", GetLastError());
		CRASH_ASSERT(false);
	}
}

unsigned int IocpManager::FnIoWorkerThread(LPVOID lpParam)
{
	// ThreadLocal init
	LThreadType = THREAD_TYPE::THREAD_IO_WORKER;
	LWorkerThreadId = MAX_DB_THREAD + PtrToInt(lpParam);

	for (int i = 0; i < SENDREQUESET_QUEUE_SIZE; i++)
	{
		LSendRequestSessionQueue[i] = new std::queue<Session*>();
	}
	LSendRequestSessionQueueIndex = 0;

	CRASH_ASSERT(LWorkerThreadId >= MAX_DB_THREAD);

	return GIocpManager->__ioWorkerThread[LWorkerThreadId - MAX_DB_THREAD]->Run();
}
