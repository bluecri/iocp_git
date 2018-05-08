#pragma once

#include <sqlext.h>
#include <sqltypes.h>

struct SQL_CONN
{
	SQLHDBC		_sqlHdbc = nullptr;
	SQLHSTMT	_sqlHstmt = nullptr;
	bool		_usingNow = false;
};

class DBHelper
{
public:
	DBHelper();
	~DBHelper();

	static bool Initialize(const char* connInfoStr, int workerThreadCount);
	static void Finalize();

	bool Execute(const char* sqlstmt);		// execute query with sqlStmt & _currentSqlHstmt(params)
	bool FetchRow();						// get result to _currentSqlHstmt(results)


	/// 아래는 DbHelper 객체를 선언하고 사용해야 함
	// create sql query

	bool BindParamInt(int* param);
	bool BindParamFloat(float* param);
	bool BindParamBool(bool* param);
	bool BindParamText(const char* text);

	/// FetchRow시 데이터가 있다면 반드시 차례대로 호출해야함
	// get query result

	void BindResultColumnInt(int* r);
	void BindResultColumnFloat(float* r);
	void BindResultColumnBool(bool* r);
	void BindResultColumnText(char* text, size_t count);
	void BindResultColumnTimestamp(char* text, size_t count);

	void PrintSqlStmtError();

private:

	SQLHSTMT	_currentSqlHstmt = nullptr;		// Execute & Fetch through this stmt

	SQLSMALLINT	_currentResultCol;
	SQLSMALLINT	_currentBindParam;

	static SQLHENV _sqlHenv;		//main handle
	static SQL_CONN* _sqlConnPool;	// db connection pool (== dbThread num)
	static int _dbWorkerThreadCount;

};