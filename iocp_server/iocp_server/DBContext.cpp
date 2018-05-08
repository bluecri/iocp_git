#include "DBContext.h"
#include "origin\ThreadLocal.h"

OverlappedDBContext::OverlappedDBContext(std::shared_ptr<Player> owner, DBType dbType) : _playerShared(owner), _dbType(dbType)
{
	
}

void OverlappedDBContext::OnResult()
{
	CRASH_ASSERT(LThreadType == THREAD_IO_WORKER);

	if (_bSuccess) {
		OnSuccess();
	}
	else {
		OnFail();
	}
	return;
}

bool OverlappedDBContext::SQLExecute()
{
	CRASH_ASSERT(LThreadType == THREAD_DB_WORKER);

	return OnSQLExecute();
}


void DeleteDBContext(OverlappedDBContext * context)
{
	if (nullptr == context)
		return;

	/// ObjectPool's operate delete dispatch
	switch (context->_dbType)
	{
	case DBTYPE_NONE:
		//delete static_cast<OverlappedSendContext*>(context);
		break;

	default:
		CRASH_ASSERT(false);
	}

	return;
}
