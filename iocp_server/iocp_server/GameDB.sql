#pragma once
use gamedb

DROP TABLE IF EXISTS gamedb.PlayerTable;

CREATE TABLE gamedb.PlayerTable (
  `playerUID` INT NOT NULL AUTO_INCREMENT 
  ,`playerID` VARCHAR(20) NOT NULL
  ,`playerPassword` VARCHAR(20) NOT NULL
  ,`playerNickName` VARCHAR(32) NOT NULL
  ,`posX` FLOAT NULL
  ,`posY` FLOAT NULL
  ,`posZ` FLOAT NULL
  ,`quatX` FLOAT NULL
  ,`quatY` FLOAT NULL
  ,`quatZ` FLOAT NULL
  ,`quatW` FLOAT NULL
  ,`createTime` TIMESTAMP
  , PRIMARY KEY(`playerUID`)
  , UNIQUE INDEX(`playerNickName`)
  , UNIQUE INDEX(`playerID`)
);


DROP PROCEDURE IF EXISTS gamedb.spCreatePlayer;

DELIMITER //
CREATE PROCEDURE gamedb.spCreatePlayer(_playerID VARCHAR(20), _playerPassword VARCHAR(20), _playerNickName VARCHAR(32) )
BEGIN
	INSERT INTO gamedb.PlayerTable(playerID, playerPassword, playerNickName, createTime) 
	VALUES (_playerID, _playerPassword, _playerNickName, GETDATE());

	SELECT `playerUID` FROM `gamedb`.`PlayerTable` WHERE playerID = _playerID;
END
//
DELIMITER ;


DROP PROCEDURE IF EXISTS gamedb.spDeletePlayer;

DELIMITER //
CREATE PROCEDURE gamedb.spDeletePlayer(_playerUID INT)
BEGIN
	DELETE FROM gamedb.PlayerTable WHERE playerUID=_playerUID;
END	//
DELIMITER ;


DROP PROCEDURE IF EXISTS gamedb.spLoginPlayer;

DELIMITER //
CREATE PROCEDURE gamedb.spLoginPlayer(_playerID VARCHAR(20), _playerPassword VARCHAR(20) )
BEGIN
	SELECT * FROM gamedb.PlayerTable WHERE playerID=_playerID AND playerPassword = _playerPassword;
END	//
DELIMITER ;


DROP PROCEDURE IF EXISTS gamedb.spUpdatePlayerPosition;

DELIMITER //
CREATE PROCEDURE gamedb.spUpdatePlayerPosition(_playerUID INT, _x FLOAT, _y FLOAT, _z FLOAT, _qX FLOAT, _qY FLOAT, _qZ FLOAT, _qW FLOAT )
BEGIN
	UPDATE gamedb.PlayerTable SET posX=_x, posY=_y, posZ=_z, quatX=_qX, quatY=_qY, quatZ=_qZ, quatW=_qW  WHERE playerUID=_playerUID;
END	//
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS gamedb.spLoadPlayerPosition;
CREATE PROCEDURE gamedb.spLoadPlayerPosition(_playerUID INT)
BEGIN
	SELECT playerUID, posX, posY, posZ, quatX, quatY, quatZ, quatW FROM gamedb.PlayerTable WHERE playerUID=_playerUID;
END	//
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS gamedb.spLoadPlayerWithUID;
CREATE PROCEDURE gamedb.spLoadPlayerWithUID(_playerUID INT)
BEGIN
	SELECT * FROM gamedb.PlayerTable WHERE  playerUID=_playerUID;
END	//
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS gamedb.spLoadPlayerWithID;
CREATE PROCEDURE gamedb.spLoadPlayerWithID(_playerID INT)
BEGIN
	SELECT * FROM gamedb.PlayerTable WHERE  playerID=_playerID;
END	//
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS gamedb.spLoadPlayerWithNickName;
CREATE PROCEDURE gamedb.spLoadPlayerWithNickName(_playerNickName VARCHAR(32))
BEGIN
	SELECT * FROM gamedb.PlayerTable WHERE  playerNickName=_playerNickName;
END	//
DELIMITER ;

