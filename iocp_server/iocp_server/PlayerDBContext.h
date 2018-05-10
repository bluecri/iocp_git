#pragma once

#include "DBContext.h"
#include "origin\ObjectPool.h"
#include "ContentsConfig.h"

class DBHelper;

//DB contexts

struct OverlappedDBLogInOutContext : public OverlappedDBContext, public ObjectPool<OverlappedDBLogInOutContext>
{
public:
	enum E_TYPE { CREATE, LOGIN, LOGOUT };

	OverlappedDBLogInOutContext(std::shared_ptr<Player> playerShared)
		: OverlappedDBContext(playerShared, DBTYPE_LOGINOUT)
	{
	}

	void Init_login(const std::string& id, const std::string& password);
	void Init_logout();		//TODO
	void Init_create(std::string& id, std::string& password, std::string& nickName);


	virtual bool OnSQLExecute();
	virtual void OnSuccess();
	virtual void OnFail();

	//	int _intUID;	int OverlappedDBContext
	char _cstrID[PLAYER_ID_MAX_LEN];
	char _cstrPassword[PLAYER_PASSWORD_MAX_LEN];
	char _cstrNickName[PLAYER_NICKNAME_MAX_LEN];

	float _x, _y, _z;
	float _qx, _qy, _qz, _qw;

	char _cstr_timeStamp[PLAYER_TIMESTAMP_LEN];

	E_TYPE _inType;

protected:
	void BindParamPos(DBHelper& dbHelper);
	void BindRequestPos(DBHelper& dbHelper);
	void BindRequestAll(DBHelper& dbHelper);
};

struct OverlappedDBUpdatePlayerContext : public OverlappedDBContext, public ObjectPool<OverlappedDBLogInOutContext>
{
public:
	enum E_TYPE { LOAD_PLAYER_POS, UPDATE_PLAYER_POS, LOAD_PLAYER_INFO_WITH_UID, LOAD_PLAYER_INFO_WITH_ID
	, LOAD_PLAYER_INFO_WITH_NICKNAME
	};

	OverlappedDBUpdatePlayerContext(std::shared_ptr<Player> owner)
		: OverlappedDBContext(owner, DBTYPE_PLAYER)
	{
	}

	//self
	void init_updatePlayerPosition();
	void init_loadPlayerPosition();

	//not self : A가 B의 info를 원함.
	void init_loadPlayerWithUID(int uid = -1);		// -1 == self
	void init_loadPlayerWithID(const char* id);
	void init_loadPlayerWithNickName(const char* nickName);


	virtual bool OnSQLExecute();
	virtual void OnSuccess();
	virtual void OnFail();

	char _cstrID[PLAYER_ID_MAX_LEN];
	char _cstrPassword[PLAYER_PASSWORD_MAX_LEN];
	char _cstrNickName[PLAYER_NICKNAME_MAX_LEN];

	int _uID;
	float _x, _y, _z;
	float _qx, _qy, _qz, _qw;

	E_TYPE _inType;

protected:
	void BindParamAll(DBHelper& dbHelper);
	void BindParamPos(DBHelper& dbHelper);
	void BindRequestPos(DBHelper& dbHelper);
	void BindRequestAll(DBHelper& dbHelper);
};
