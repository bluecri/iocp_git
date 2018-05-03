#pragma once

#include "OverlappedContext.h"
#include "Player.h"

enum CKType {
	CK_DB_REQUEST,
	CK_DB_RESULT
};

enum DBType
{
	DBTYPE_NONE,
	DBTYPE_LOGINOUT

};


struct OverlappedDBContext : public OverlappedContext
{
public:
	OverlappedDBContext(std::shared_ptr<Player> owner, DBType dbType);
	
	bool SQLExecute();	// call OnSQLExecute

	void OnResult();	//call OnSuccess Or OnFail according to success
	
	

	std::shared_ptr<Player>	_playerObject;
	DBType	_dbType;
	bool	_bSuccess;

protected:
	virtual bool OnSQLExecute() = 0;	//SQL in DB Thread
	virtual void OnSuccess() {}		// Do response with Player Object in IOThread
	virtual void OnFail() {}
};


//DB contexts
//GetSharedFromThis<Player>(),
struct OverlappedDBLogInOutContext : public OverlappedDBContext, public ObjectPool<OverlappedDBLogInOutContext>
{
	enum E_TYPE { CREATE, LOGIN, LOGOUT };

	OverlappedDBLogInOutContext(std::shared_ptr<Player> owner, std::string& id, std::string& password, std::string& nickName, OverlappedDBLogInOutContext::E_TYPE type)
		: OverlappedDBContext(owner, DBTYPE_LOGINOUT), _strID(id), _strPassword(password), _strNickName(nickName), _inType(type)
	{
	}

	virtual bool OnSQLExecute();
	virtual void OnSuccess();
	virtual void OnFail();

	std::string _strID;
	std::string _strPassword;
	std::string _strNickName;

	E_TYPE _inType;

};

void DeleteDBContext(OverlappedDBContext* context);