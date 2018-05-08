#include "stdafx.h"
#include "PlayerDBContext.h"
#include "DBHelper.h"
#include "SQLStatement.h"
#include "RigidbodyComponent.h"

void OverlappedDBLogInOutContext::Init_login(std::string& id, std::string& password)
{
	strcpy_s(_cstrID, id.c_str());
	strcpy_s(_cstrPassword, password.c_str());
	_inType = OverlappedDBLogInOutContext::E_TYPE::LOGIN;
}

void OverlappedDBLogInOutContext::Init_logout()
{
	_inType = OverlappedDBLogInOutContext::E_TYPE::LOGOUT; 

	return;
}

void OverlappedDBLogInOutContext::Init_create(std::string & id, std::string & password, std::string & nickName)
{
	strcpy_s(_cstrID, id.c_str());
	strcpy_s(_cstrPassword, password.c_str());
	strcpy_s(_cstrNickName, nickName.c_str());
	_inType = OverlappedDBLogInOutContext::E_TYPE::CREATE;
}

bool OverlappedDBLogInOutContext::OnSQLExecute()
{
	DBHelper dbHelper;

	int result = 0;
	const char* bindedStmt = nullptr;

	switch (_inType) {
	case E_TYPE::CREATE:
		bindedStmt = SQL_CreatePlayer;

		dbHelper.BindParamText(_cstrID);
		dbHelper.BindParamText(_cstrPassword);
		dbHelper.BindParamText(_cstrNickName);
		break;
	case E_TYPE::LOGIN:
		bindedStmt = SQL_LoginPlayer;

		dbHelper.BindParamText(_cstrID);
		dbHelper.BindParamText(_cstrPassword);

		BindRequestAll(dbHelper);
		//dbHelper.BindResultColumnInt(&mPlayerId);

		break;
	case E_TYPE::LOGOUT:
		
		break;
	default:
		printf_s("[DEBUG] : default type in OverlappedDBLogInOutContext::OnSQLExecute : %d\n", GetLastError());
	}

	if (dbHelper.Execute(SQL_CreatePlayer))
	{
		if (dbHelper.FetchRow())
		{
			return result != 0;
		}
	}

	return false;
}

void OverlappedDBLogInOutContext::OnSuccess()
{
	switch (_inType) {
	case E_TYPE::CREATE:
		printf_s("[LOG] : OverlappedDBLogInOutContext::CREATE success\n");
		break;
	case E_TYPE::LOGIN:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOGIN success\n");
		break;
	case E_TYPE::LOGOUT:
		//printf_s("[LOG] : OverlappedDBLogInOutContext::LOGOUT success\n");
		break;
	default:
		printf_s("[DEBUG] : default type in OverlappedDBLogInOutContext::OnSQLExecute : %d\n", GetLastError());
	}
	return;
}

void OverlappedDBLogInOutContext::OnFail()
{
	switch (_inType) {
	case E_TYPE::CREATE:
		printf_s("[LOG] : OverlappedDBLogInOutContext::CREATE fail\n");
		break;
	case E_TYPE::LOGIN:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOGIN fail\n");
		break;
	case E_TYPE::LOGOUT:
		//printf_s("[LOG] : OverlappedDBLogInOutContext::LOGOUT fail\n");
		break;
	default:
		printf_s("[DEBUG] : default type in OverlappedDBLogInOutContext::OnFail : %d\n", GetLastError());
	}
	return;
}


// --------------------------------------------------------------------------


void OverlappedDBUpdatePlayerContext::init_updatePlayerPosition()
{
	_inType = OverlappedDBUpdatePlayerContext::E_TYPE::UPDATE_PLAYER_POS;

	_playerObject->EnterReadLock();
	_intUID = _playerObject->GetPlayerUID();

	RigidbodyComponent* rigidbodyComp = _playerObject->GetRigidbodyComponent();
	vec3 posVec3 = *(rigidbodyComp->GetPosVec3());	//copy
	quat quat = *(rigidbodyComp->GetQuat());	//copy
	_playerObject->LeaveReadLock();

	_x = posVec3.x;
	_y = posVec3.y;
	_z = posVec3.z;

	_qx = quat.x;
	_qy = quat.y;
	_qz = quat.z;
	_qw = quat.w;

	return;
}

void OverlappedDBUpdatePlayerContext::init_loadPlayerPosition()
{
	_inType = OverlappedDBUpdatePlayerContext::E_TYPE::LOAD_PLAYER_POS;

	if (_playerObject != nullptr) {
		_playerObject->EnterReadLock();
		_intUID = _playerObject->GetPlayerUID();
		_playerObject->LeaveReadLock();
	}
}

void OverlappedDBUpdatePlayerContext::init_loadPlayerWithUID(int uid)
{
	_inType = OverlappedDBUpdatePlayerContext::E_TYPE::LOAD_PLAYER_INFO_WITH_UID;
	_intUID = uid;

}

void OverlappedDBUpdatePlayerContext::init_loadPlayerWithID(const char* id)
{
	_inType = OverlappedDBUpdatePlayerContext::E_TYPE::LOAD_PLAYER_INFO_WITH_ID;
	strcpy_s(_cstrID, id);
}

void OverlappedDBUpdatePlayerContext::init_loadPlayerWithNickName(const char* nickName)
{
	_inType = OverlappedDBUpdatePlayerContext::E_TYPE::LOAD_PLAYER_INFO_WITH_NICKNAME;
	strcpy_s(_cstrNickName, nickName);
}

bool OverlappedDBUpdatePlayerContext::OnSQLExecute()
{
	DBHelper dbHelper;

	int result = 0;
	const char* bindedStmt = nullptr;

	switch (_inType) {
	case E_TYPE::UPDATE_PLAYER_POS:
		bindedStmt = SQL_UpdatePlayerPosition;

		dbHelper.BindParamInt(&_intUID);
		BindParamPos(dbHelper);

		break;
	case E_TYPE::LOAD_PLAYER_POS:
		bindedStmt = SQL_LoadPlayerPosition;

		dbHelper.BindParamInt(&_intUID);

		dbHelper.BindResultColumnInt(&_intUID);
		BindRequestPos(dbHelper);

		break;
	case E_TYPE::LOAD_PLAYER_INFO_WITH_UID:
		bindedStmt = SQL_LoadPlayerWithUID;
		dbHelper.BindParamInt(&_intUID);
		BindRequestAll(dbHelper);

		break;
	case E_TYPE::LOAD_PLAYER_INFO_WITH_ID:
		bindedStmt = SQL_LoadPlayerWithID;
		dbHelper.BindParamText(_cstrID);
		BindRequestAll(dbHelper);

		break;
	case E_TYPE::LOAD_PLAYER_INFO_WITH_NICKNAME:
		bindedStmt = SQL_LoadPlayerWithNickName;
		dbHelper.BindParamText(_cstrNickName);
		BindRequestAll(dbHelper);

		break;
	default:
		printf_s("[DEBUG] : default type in OverlappedDBLogInOutContext::OnSQLExecute : %d\n", GetLastError());
	}

	if (dbHelper.Execute(SQL_CreatePlayer))
	{
		if (dbHelper.FetchRow())
		{
			return result != 0;
		}
	}

	return false;
}

void OverlappedDBUpdatePlayerContext::OnSuccess()
{
	switch (_inType) {
	case E_TYPE::UPDATE_PLAYER_POS:
		printf_s("[LOG] : OverlappedDBLogInOutContext::UPDATE_PLAYER_POS success\n");

		break;
	case E_TYPE::LOAD_PLAYER_POS:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOAD_PLAYER_POS success\n");

		break;
	case E_TYPE::LOAD_PLAYER_INFO_WITH_UID:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOAD_PLAYER_INFO_WITH_UID success\n");

		break;
	case E_TYPE::LOAD_PLAYER_INFO_WITH_ID:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOAD_PLAYER_INFO_WITH_ID success\n");

		break;
	case E_TYPE::LOAD_PLAYER_INFO_WITH_NICKNAME:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOAD_PLAYER_INFO_WITH_NICKNAME success\n");

		break;
	default:
		printf_s("[DEBUG] : default type in OverlappedDBUpdatePlayerContext::OnSQLExecute : %d\n", GetLastError());
	}
}

void OverlappedDBUpdatePlayerContext::OnFail()
{
	switch (_inType) {
	case E_TYPE::UPDATE_PLAYER_POS:
		printf_s("[LOG] : OverlappedDBLogInOutContext::UPDATE_PLAYER_POS fail\n");

		break;
	case E_TYPE::LOAD_PLAYER_POS:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOAD_PLAYER_POS fail\n");

		break;
	case E_TYPE::LOAD_PLAYER_INFO_WITH_UID:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOAD_PLAYER_INFO_WITH_UID fail\n");

		break;
	case E_TYPE::LOAD_PLAYER_INFO_WITH_ID:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOAD_PLAYER_INFO_WITH_ID fail\n");

		break;
	case E_TYPE::LOAD_PLAYER_INFO_WITH_NICKNAME:
		printf_s("[LOG] : OverlappedDBLogInOutContext::LOAD_PLAYER_INFO_WITH_NICKNAME fail\n");

		break;
	default:
		printf_s("[DEBUG] : default type in OverlappedDBUpdatePlayerContext::OnFail : %d\n", GetLastError());
	}
}



void OverlappedDBLogInOutContext::BindParamPos(DBHelper& dbHelper)
{
	dbHelper.BindParamFloat(&_x);
	dbHelper.BindParamFloat(&_y);
	dbHelper.BindParamFloat(&_z);
	dbHelper.BindParamFloat(&_qx);
	dbHelper.BindParamFloat(&_qy);
	dbHelper.BindParamFloat(&_qz);
	dbHelper.BindParamFloat(&_qw);

	return;
}

void OverlappedDBLogInOutContext::BindRequestPos(DBHelper & dbHelper)
{
	dbHelper.BindResultColumnFloat(&_x);
	dbHelper.BindResultColumnFloat(&_y);
	dbHelper.BindResultColumnFloat(&_z);
	dbHelper.BindResultColumnFloat(&_qx);
	dbHelper.BindResultColumnFloat(&_qy);
	dbHelper.BindResultColumnFloat(&_qz);
	dbHelper.BindResultColumnFloat(&_qw);
	dbHelper.BindResultColumnFloat(&_qw);

	return;
}

void OverlappedDBLogInOutContext::BindRequestAll(DBHelper& dbHelper)
{
	dbHelper.BindResultColumnInt(&_intUID);
	dbHelper.BindResultColumnText(_cstrID, PLAYER_ID_MAX_LEN);
	dbHelper.BindResultColumnText(_cstrPassword, PLAYER_PASSWORD_MAX_LEN);
	dbHelper.BindResultColumnText(_cstrNickName, PLAYER_NICKNAME_MAX_LEN);
	BindRequestPos(dbHelper);

	return;
}

void OverlappedDBUpdatePlayerContext::BindParamPos(DBHelper& dbHelper)
{
	dbHelper.BindParamFloat(&_x);
	dbHelper.BindParamFloat(&_y);
	dbHelper.BindParamFloat(&_z);
	dbHelper.BindParamFloat(&_qx);
	dbHelper.BindParamFloat(&_qy);
	dbHelper.BindParamFloat(&_qz);
	dbHelper.BindParamFloat(&_qw);

	return;
}


void OverlappedDBUpdatePlayerContext::BindParamAll(DBHelper& dbHelper)
{
	dbHelper.BindParamInt(&_intUID);
	dbHelper.BindParamText(_cstrID);
	dbHelper.BindParamText(_cstrPassword);
	dbHelper.BindParamText(_cstrNickName);

	BindParamPos(dbHelper);

	return;
}

void OverlappedDBUpdatePlayerContext::BindRequestPos(DBHelper & dbHelper)
{
	dbHelper.BindResultColumnFloat(&_x);
	dbHelper.BindResultColumnFloat(&_y);
	dbHelper.BindResultColumnFloat(&_z);
	dbHelper.BindResultColumnFloat(&_qx);
	dbHelper.BindResultColumnFloat(&_qy);
	dbHelper.BindResultColumnFloat(&_qz);
	dbHelper.BindResultColumnFloat(&_qw);
	dbHelper.BindResultColumnFloat(&_qw);
	return;
}

void OverlappedDBUpdatePlayerContext::BindRequestAll(DBHelper& dbHelper)
{
	dbHelper.BindResultColumnInt(&_intUID);
	dbHelper.BindResultColumnText(_cstrID, PLAYER_ID_MAX_LEN);
	dbHelper.BindResultColumnText(_cstrPassword, PLAYER_PASSWORD_MAX_LEN);
	dbHelper.BindResultColumnText(_cstrNickName, PLAYER_NICKNAME_MAX_LEN);

	BindRequestPos(dbHelper);
	return;
}