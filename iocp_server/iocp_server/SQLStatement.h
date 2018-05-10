#pragma once

//gamedb.PlayerTable stmt

const char* SQL_CreatePlayer = "{ call gamedb.spCreatePlayer ( ?, ?, ? ) }";
// id(varchar20), password(varchar20), nickname(varchar32)

const char* SQL_LoginPlayer = "{ call gamedb.spLoginPlayer ( ?, ? ) }";
// id(varchar20), password(varchar20)
/* all */

const char* SQL_DeletePlayer = "{ call gamedb.spDeletePlayer (?) }";
// uid(int)

//change to SQL_UpdatePlayerGameInfo(pos, state...)
const char* SQL_UpdatePlayerPosition = "{ call gamedb.spUpdatePlayerPosition (?, ?, ? ,?, ?, ? ,?) }";
// uid(int), x(float), y(float), z(float), qX(float), qY(float), qZ(float), qW(float)

//change to SQL_LoadPlayerGameInfo(pos, state...)
const char* SQL_LoadPlayerPosition = "{ call gamedb.spLoadPlayerPosition (?) }";
// uid(int)
/*
uid, posX, posY, posZ, quatX, quatY, quatZ, quatW
*/

//TODO : const char* SQL_UpdatePlayerWithUID = "{ call gamedb.spUpdatePlayerPosition (?, ?, ? ,?, ?, ? ,?) }";
// uid(int), x(float), y(float), z(float), qX(float), qY(float), qZ(float), qW(float)

const char* SQL_LoadPlayerWithUID = "{ call gamedb.spLoadPlayerWithUID (?) }";
// uid(int)
/* all */


const char* SQL_LoadPlayerWithID = "{ call gamedb.spLoadPlayerWithID (?) }";
// id(VARCHAR(32))
/* all */



const char* SQL_LoadPlayerWithNickName = "{ call gamedb.spLoadPlayerWithNickName (?) }";
// nickName(VARCHAR(32))
/* all */

