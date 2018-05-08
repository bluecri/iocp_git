#include "ThreadLocal.h"
#include "../../iocp_server/ClientSession.h"
// todo : #include "Log.h"

__declspec(thread) int LThreadType = -1;
__declspec(thread) int LWorkerThreadId = -1;
__declspec(thread) int LSendRequestSessionQueueIndex = -1;
__declspec(thread) std::queue<Session*>* LSendRequestSessionQueue[SENDREQUESET_QUEUE_SIZE] = { nullptr, nullptr };
__declspec(thread) LockOrderChecker* LLockOrderChecker = nullptr;
__declspec(thread) ThreadCallHistory* LThreadCallHistory = nullptr;
__declspec(thread) ThreadCallElapsedRecord* LThreadCallElapsedRecord = nullptr;
__declspec(thread) void* LRecentThisPointer = nullptr;

ThreadCallHistory* GThreadCallHistory[MAX_WORKER_THREAD] = { 0, };
ThreadCallElapsedRecord* GThreadCallElapsedRecord[MAX_WORKER_THREAD] = { 0, };
