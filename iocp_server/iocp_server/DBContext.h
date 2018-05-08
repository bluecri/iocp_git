#pragma once

#include "Player.h"

enum CKType {
	CK_DB_REQUEST,
	CK_DB_RESULT
};

enum DBType
{
	DBTYPE_NONE,
	DBTYPE_LOGINOUT,
	DBTYPE_PLAYER
};


struct OverlappedDBContext
{
public:
	OverlappedDBContext(std::shared_ptr<Player> owner, DBType dbType);
	
	bool SQLExecute();	// call OnSQLExecute

	void OnResult();	//call OnSuccess Or OnFail according to success
	
	
	std::shared_ptr<Player>	_playerObject;
	int _intUID;	//if _playerObject is null -> use this. If _playerObject is needed... do something/

	DBType	_dbType;
	bool	_bSuccess;

protected:
	virtual bool OnSQLExecute() = 0;	//SQL in DB Thread
	virtual void OnSuccess() {}		// Do response with Player Object in IOThread
	virtual void OnFail() {}
};


void DeleteDBContext(OverlappedDBContext* context);