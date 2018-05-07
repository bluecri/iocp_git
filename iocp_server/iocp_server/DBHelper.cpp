#include "stdafx.h"
#include "origin\ThreadLocal.h"
// #include "origin\Log.h"
#include "DBHelper.h"


SQLHENV DBHelper::_sqlHenv;
SQL_CONN* DBHelper::_sqlConnPool;
int DBHelper::_dbWorkerThreadCount = 0;

DBHelper::DBHelper()
{
	CRASH_ASSERT(LThreadType == THREAD_DB_WORKER);

	CRASH_ASSERT(_sqlConnPool[LWorkerThreadId]._usingNow == false);

	_currentSqlHstmt = _sqlConnPool[LWorkerThreadId]._sqlHstmt;
	_currentResultCol = 1;
	_currentBindParam = 1;
	CRASH_ASSERT(_currentSqlHstmt != nullptr);

	_sqlConnPool[LWorkerThreadId]._usingNow = true;	// prevent to create multiple dbHelper in 1 thread
}

DBHelper::~DBHelper()
{
	SQLFreeStmt(_currentSqlHstmt, SQL_UNBIND);
	SQLFreeStmt(_currentSqlHstmt, SQL_RESET_PARAMS);
	SQLFreeStmt(_currentSqlHstmt, SQL_CLOSE);

	_sqlConnPool[LWorkerThreadId]._usingNow = false;
}

bool DBHelper::Initialize(const char* connInfoStr, int workerThreadCount)
{
	_sqlConnPool = new SQL_CONN[workerThreadCount];
	_dbWorkerThreadCount = workerThreadCount;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_sqlHenv))
	{
		printf_s("DbHelper Initialize SQLAllocHandle failed\n");
		return false;
	}

	if (SQL_SUCCESS != SQLSetEnvAttr(_sqlHenv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER))
	{
		printf_s("DbHelper Initialize SQLSetEnvAttr failed\n");
		return false;
	}

	/// 스레드별로 SQL connection을 풀링하는 방식
	for (int i = 0; i < _dbWorkerThreadCount; ++i)
	{
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, _sqlHenv, &_sqlConnPool[i]._sqlHdbc))
		{
			printf_s("DbHelper Initialize SQLAllocHandle failed\n");
			return false;
		}

		SQLSMALLINT resultLen = 0;
		//SQLConnect(hdbc, "dbsServer", SQL_NTS, "serverUser", SQL_NTS, "serverUser1", SQL_NTS)
		SQLRETURN ret = SQLDriverConnect(_sqlConnPool[i]._sqlHdbc, NULL, (SQLCHAR*)connInfoStr, (SQLSMALLINT)strlen(connInfoStr), NULL, 0, &resultLen, SQL_DRIVER_NOPROMPT);
		if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			SQLCHAR sqlState[1024] = { 0, };
			SQLINTEGER nativeError = 0;
			SQLCHAR msgText[1024] = { 0, };
			SQLSMALLINT textLen = 0;

			SQLGetDiagRec(SQL_HANDLE_DBC, _sqlConnPool[i]._sqlHdbc, 1, sqlState, &nativeError, msgText, 1024, &textLen);

			printf_s("DbHelper Initialize SQLDriverConnect failed: %s \n", msgText);

			return false;
		}

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, _sqlConnPool[i]._sqlHdbc, &_sqlConnPool[i]._sqlHstmt))
		{
			printf_s("DbHelper Initialize SQLAllocHandle SQL_HANDLE_STMT failed\n");
			return false;
		}

	}

	return true;
}


void DBHelper::Finalize()
{
	for (int i = 0; i < _dbWorkerThreadCount; ++i)
	{
		SQL_CONN* currConn = &_sqlConnPool[i];
		if (currConn->_sqlHstmt)
			SQLFreeHandle(SQL_HANDLE_STMT, currConn->_sqlHstmt);

		if (currConn->_sqlHdbc)
			SQLFreeHandle(SQL_HANDLE_DBC, currConn->_sqlHdbc);		// main handle
	}

	delete[] _sqlConnPool;


}

bool DBHelper::Execute(const char* sqlstmt)
{

	SQLRETURN ret = SQLExecDirect(_currentSqlHstmt, (SQLCHAR*)sqlstmt, SQL_NTSL);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::FetchRow()
{
	SQLRETURN ret = SQLFetch(_currentSqlHstmt);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		if (SQL_NO_DATA != ret)
		{
			PrintSqlStmtError();
		}

		return false;
	}

	return true;
}



bool DBHelper::BindParamInt(int* param)
{
	SQLRETURN ret = SQLBindParameter(_currentSqlHstmt, _currentBindParam++, SQL_PARAM_INPUT,
		SQL_C_LONG, SQL_INTEGER, 10, 0, param, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::BindParamFloat(float* param)
{
	SQLRETURN ret = SQLBindParameter(_currentSqlHstmt, _currentBindParam++, SQL_PARAM_INPUT,
		SQL_C_FLOAT, SQL_REAL, 15, 0, param, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::BindParamBool(bool* param)
{
	SQLRETURN ret = SQLBindParameter(_currentSqlHstmt, _currentBindParam++, SQL_PARAM_INPUT,
		SQL_C_TINYINT, SQL_TINYINT, 3, 0, param, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::BindParamText(const char* text)
{
	SQLRETURN ret = SQLBindParameter(_currentSqlHstmt, _currentBindParam++, SQL_PARAM_INPUT,
		SQL_C_CHAR, SQL_VARCHAR, strlen(text), 0, (SQLPOINTER)text, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
		return false;
	}

	return true;
}


void DBHelper::BindResultColumnInt(int* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(_currentSqlHstmt, _currentResultCol++, SQL_C_LONG, r, 4, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}
void DBHelper::BindResultColumnFloat(float* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(_currentSqlHstmt, _currentResultCol++, SQL_C_FLOAT, r, 4, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}

void DBHelper::BindResultColumnBool(bool* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(_currentSqlHstmt, _currentResultCol++, SQL_C_TINYINT, r, 1, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}
void DBHelper::BindResultColumnText(char* text, size_t count)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(_currentSqlHstmt, _currentResultCol++, SQL_C_WCHAR, text, count, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}

void DBHelper::BindResultColumnTimestamp(char* text, size_t count)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(_currentSqlHstmt, _currentResultCol++, SQL_C_TIMESTAMP, text, count, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		PrintSqlStmtError();
	}
}

void DBHelper::PrintSqlStmtError()
{
	SQLCHAR sqlState[1024] = { 0, };
	SQLINTEGER nativeError = 0;
	SQLCHAR msgText[1024] = { 0, };
	SQLSMALLINT textLen = 0;

	SQLGetDiagRec(SQL_HANDLE_STMT, _currentSqlHstmt, 1, sqlState, &nativeError, msgText, 1024, &textLen);

	printf_s("DbHelper SQL Statement Error: %s \n", msgText);
}