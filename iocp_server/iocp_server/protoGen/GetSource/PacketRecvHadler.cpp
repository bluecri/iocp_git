#include "../../ClientSession.h"
#include "../../Packet.h"


bool PacketRecvToMsg(ClientSession * session, PacketHeader & header, google::protobuf::io::CodedInputStream & codeInputStream)
{
	switch (header.GetType())
	{
	case PACKET_TYPE::PACKET_TYPE_msgLobby:
	{
		prop::msgLobby *msgLobby = new prop::msgLobby();
		if (false == msgLobby->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_msgLobby error\n");
			return false;
		}
		PacketRecvMsgHandle(session, msgLobby);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_msgRoom:
	{
		prop::msgRoom *msgRoom = new prop::msgRoom();
		if (false == msgRoom->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_msgRoom error\n");
			return false;
		}
		PacketRecvMsgHandle(session, msgRoom);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_msgGame:
	{
		prop::msgGame *msgGame = new prop::msgGame();
		if (false == msgGame->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_msgGame error\n");
			return false;
		}
		PacketRecvMsgHandle(session, msgGame);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_msgUserInfo:
	{
		prop::msgUserInfo *msgUserInfo = new prop::msgUserInfo();
		if (false == msgUserInfo->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_msgUserInfo error\n");
			return false;
		}
		PacketRecvMsgHandle(session, msgUserInfo);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_msgUserInGameInfo:
	{
		prop::msgUserInGameInfo *msgUserInGameInfo = new prop::msgUserInGameInfo();
		if (false == msgUserInGameInfo->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_msgUserInGameInfo error\n");
			return false;
		}
		PacketRecvMsgHandle(session, msgUserInGameInfo);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_msgUserInGameStateInfo:
	{
		prop::msgUserInGameStateInfo *msgUserInGameStateInfo = new prop::msgUserInGameStateInfo();
		if (false == msgUserInGameStateInfo->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_msgUserInGameStateInfo error\n");
			return false;
		}
		PacketRecvMsgHandle(session, msgUserInGameStateInfo);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_accountCreateRequest:
	{
		prop::accountCreateRequest *accountCreateRequest = new prop::accountCreateRequest();
		if (false == accountCreateRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_accountCreateRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, accountCreateRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_accountCreateResponse:
	{
		prop::accountCreateResponse *accountCreateResponse = new prop::accountCreateResponse();
		if (false == accountCreateResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_accountCreateResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, accountCreateResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_accountLoginRequest:
	{
		prop::accountLoginRequest *accountLoginRequest = new prop::accountLoginRequest();
		if (false == accountLoginRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_accountLoginRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, accountLoginRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_accountLoginResponse:
	{
		prop::accountLoginResponse *accountLoginResponse = new prop::accountLoginResponse();
		if (false == accountLoginResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_accountLoginResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, accountLoginResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_accountLogoutRequest:
	{
		prop::accountLogoutRequest *accountLogoutRequest = new prop::accountLogoutRequest();
		if (false == accountLogoutRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_accountLogoutRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, accountLogoutRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_accountLogoutResponse:
	{
		prop::accountLogoutResponse *accountLogoutResponse = new prop::accountLogoutResponse();
		if (false == accountLogoutResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_accountLogoutResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, accountLogoutResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_anyPlayerInfoSelfRequest:
	{
		prop::anyPlayerInfoSelfRequest *anyPlayerInfoSelfRequest = new prop::anyPlayerInfoSelfRequest();
		if (false == anyPlayerInfoSelfRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_anyPlayerInfoSelfRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, anyPlayerInfoSelfRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_anyPlayerInfoOtherRequestWithID:
	{
		prop::anyPlayerInfoOtherRequestWithID *anyPlayerInfoOtherRequestWithID = new prop::anyPlayerInfoOtherRequestWithID();
		if (false == anyPlayerInfoOtherRequestWithID->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_anyPlayerInfoOtherRequestWithID error\n");
			return false;
		}
		PacketRecvMsgHandle(session, anyPlayerInfoOtherRequestWithID);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_anyPlayerInfoOtherRequestWithNickName:
	{
		prop::anyPlayerInfoOtherRequestWithNickName *anyPlayerInfoOtherRequestWithNickName = new prop::anyPlayerInfoOtherRequestWithNickName();
		if (false == anyPlayerInfoOtherRequestWithNickName->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_anyPlayerInfoOtherRequestWithNickName error\n");
			return false;
		}
		PacketRecvMsgHandle(session, anyPlayerInfoOtherRequestWithNickName);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_anyPlayerInfoResponse:
	{
		prop::anyPlayerInfoResponse *anyPlayerInfoResponse = new prop::anyPlayerInfoResponse();
		if (false == anyPlayerInfoResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_anyPlayerInfoResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, anyPlayerInfoResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_anyWhisperRequestWithUID:
	{
		prop::anyWhisperRequestWithUID *anyWhisperRequestWithUID = new prop::anyWhisperRequestWithUID();
		if (false == anyWhisperRequestWithUID->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_anyWhisperRequestWithUID error\n");
			return false;
		}
		PacketRecvMsgHandle(session, anyWhisperRequestWithUID);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_anyWhisperRequestWithID:
	{
		prop::anyWhisperRequestWithID *anyWhisperRequestWithID = new prop::anyWhisperRequestWithID();
		if (false == anyWhisperRequestWithID->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_anyWhisperRequestWithID error\n");
			return false;
		}
		PacketRecvMsgHandle(session, anyWhisperRequestWithID);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_anyWhisperRequestWithNickName:
	{
		prop::anyWhisperRequestWithNickName *anyWhisperRequestWithNickName = new prop::anyWhisperRequestWithNickName();
		if (false == anyWhisperRequestWithNickName->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_anyWhisperRequestWithNickName error\n");
			return false;
		}
		PacketRecvMsgHandle(session, anyWhisperRequestWithNickName);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_anyWhisperResponse:
	{
		prop::anyWhisperResponse *anyWhisperResponse = new prop::anyWhisperResponse();
		if (false == anyWhisperResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_anyWhisperResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, anyWhisperResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_outLobbyLobbylistRequest:
	{
		prop::outLobbyLobbylistRequest *outLobbyLobbylistRequest = new prop::outLobbyLobbylistRequest();
		if (false == outLobbyLobbylistRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_outLobbyLobbylistRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, outLobbyLobbylistRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_outLobbyLobbylistResponse:
	{
		prop::outLobbyLobbylistResponse *outLobbyLobbylistResponse = new prop::outLobbyLobbylistResponse();
		if (false == outLobbyLobbylistResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_outLobbyLobbylistResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, outLobbyLobbylistResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_outLobbyEnterLobbyRequest:
	{
		prop::outLobbyEnterLobbyRequest *outLobbyEnterLobbyRequest = new prop::outLobbyEnterLobbyRequest();
		if (false == outLobbyEnterLobbyRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_outLobbyEnterLobbyRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, outLobbyEnterLobbyRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_outLobbyEnterLobbyResponse:
	{
		prop::outLobbyEnterLobbyResponse *outLobbyEnterLobbyResponse = new prop::outLobbyEnterLobbyResponse();
		if (false == outLobbyEnterLobbyResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_outLobbyEnterLobbyResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, outLobbyEnterLobbyResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inLobbyPlayerlistRequest:
	{
		prop::inLobbyPlayerlistRequest *inLobbyPlayerlistRequest = new prop::inLobbyPlayerlistRequest();
		if (false == inLobbyPlayerlistRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inLobbyPlayerlistRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inLobbyPlayerlistRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inLobbyPlayerlistResponse:
	{
		prop::inLobbyPlayerlistResponse *inLobbyPlayerlistResponse = new prop::inLobbyPlayerlistResponse();
		if (false == inLobbyPlayerlistResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inLobbyPlayerlistResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inLobbyPlayerlistResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inLobbyEnterRoomRequest:
	{
		prop::inLobbyEnterRoomRequest *inLobbyEnterRoomRequest = new prop::inLobbyEnterRoomRequest();
		if (false == inLobbyEnterRoomRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inLobbyEnterRoomRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inLobbyEnterRoomRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inLobbyEnterRoomResponse:
	{
		prop::inLobbyEnterRoomResponse *inLobbyEnterRoomResponse = new prop::inLobbyEnterRoomResponse();
		if (false == inLobbyEnterRoomResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inLobbyEnterRoomResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inLobbyEnterRoomResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inLobbyLeaveLobbyRequest:
	{
		prop::inLobbyLeaveLobbyRequest *inLobbyLeaveLobbyRequest = new prop::inLobbyLeaveLobbyRequest();
		if (false == inLobbyLeaveLobbyRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inLobbyLeaveLobbyRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inLobbyLeaveLobbyRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inLobbyLeaveLobbyResponse:
	{
		prop::inLobbyLeaveLobbyResponse *inLobbyLeaveLobbyResponse = new prop::inLobbyLeaveLobbyResponse();
		if (false == inLobbyLeaveLobbyResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inLobbyLeaveLobbyResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inLobbyLeaveLobbyResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inLobbyChatRequest:
	{
		prop::inLobbyChatRequest *inLobbyChatRequest = new prop::inLobbyChatRequest();
		if (false == inLobbyChatRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inLobbyChatRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inLobbyChatRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inLobbyChatResponse:
	{
		prop::inLobbyChatResponse *inLobbyChatResponse = new prop::inLobbyChatResponse();
		if (false == inLobbyChatResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inLobbyChatResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inLobbyChatResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inLobbyChatBroadcast:
	{
		prop::inLobbyChatBroadcast *inLobbyChatBroadcast = new prop::inLobbyChatBroadcast();
		if (false == inLobbyChatBroadcast->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inLobbyChatBroadcast error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inLobbyChatBroadcast);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomChatRequest:
	{
		prop::inRoomChatRequest *inRoomChatRequest = new prop::inRoomChatRequest();
		if (false == inRoomChatRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomChatRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomChatRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomChatResponse:
	{
		prop::inRoomChatResponse *inRoomChatResponse = new prop::inRoomChatResponse();
		if (false == inRoomChatResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomChatResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomChatResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomChatBroadcast:
	{
		prop::inRoomChatBroadcast *inRoomChatBroadcast = new prop::inRoomChatBroadcast();
		if (false == inRoomChatBroadcast->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomChatBroadcast error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomChatBroadcast);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomLeaveRoomRequest:
	{
		prop::inRoomLeaveRoomRequest *inRoomLeaveRoomRequest = new prop::inRoomLeaveRoomRequest();
		if (false == inRoomLeaveRoomRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomLeaveRoomRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomLeaveRoomRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomLeaveRoomResponse:
	{
		prop::inRoomLeaveRoomResponse *inRoomLeaveRoomResponse = new prop::inRoomLeaveRoomResponse();
		if (false == inRoomLeaveRoomResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomLeaveRoomResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomLeaveRoomResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomReadyRequest:
	{
		prop::inRoomReadyRequest *inRoomReadyRequest = new prop::inRoomReadyRequest();
		if (false == inRoomReadyRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomReadyRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomReadyRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomReadyResponse:
	{
		prop::inRoomReadyResponse *inRoomReadyResponse = new prop::inRoomReadyResponse();
		if (false == inRoomReadyResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomReadyResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomReadyResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomStartRequest:
	{
		prop::inRoomStartRequest *inRoomStartRequest = new prop::inRoomStartRequest();
		if (false == inRoomStartRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomStartRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomStartRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomStartResponse:
	{
		prop::inRoomStartResponse *inRoomStartResponse = new prop::inRoomStartResponse();
		if (false == inRoomStartResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomStartResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomStartResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inRoomStartBroadcast:
	{
		prop::inRoomStartBroadcast *inRoomStartBroadcast = new prop::inRoomStartBroadcast();
		if (false == inRoomStartBroadcast->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inRoomStartBroadcast error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inRoomStartBroadcast);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inGameSyncPlayerPosToServer:
	{
		prop::inGameSyncPlayerPosToServer *inGameSyncPlayerPosToServer = new prop::inGameSyncPlayerPosToServer();
		if (false == inGameSyncPlayerPosToServer->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inGameSyncPlayerPosToServer error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inGameSyncPlayerPosToServer);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inGameSyncPlayersPosFromServer:
	{
		prop::inGameSyncPlayersPosFromServer *inGameSyncPlayersPosFromServer = new prop::inGameSyncPlayersPosFromServer();
		if (false == inGameSyncPlayersPosFromServer->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inGameSyncPlayersPosFromServer error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inGameSyncPlayersPosFromServer);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inGameSyncPlayerStateToServer:
	{
		prop::inGameSyncPlayerStateToServer *inGameSyncPlayerStateToServer = new prop::inGameSyncPlayerStateToServer();
		if (false == inGameSyncPlayerStateToServer->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inGameSyncPlayerStateToServer error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inGameSyncPlayerStateToServer);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inGameSyncPlayersStateFromServer:
	{
		prop::inGameSyncPlayersStateFromServer *inGameSyncPlayersStateFromServer = new prop::inGameSyncPlayersStateFromServer();
		if (false == inGameSyncPlayersStateFromServer->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inGameSyncPlayersStateFromServer error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inGameSyncPlayersStateFromServer);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inGameChatRequest:
	{
		prop::inGameChatRequest *inGameChatRequest = new prop::inGameChatRequest();
		if (false == inGameChatRequest->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inGameChatRequest error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inGameChatRequest);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inGameChatResponse:
	{
		prop::inGameChatResponse *inGameChatResponse = new prop::inGameChatResponse();
		if (false == inGameChatResponse->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inGameChatResponse error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inGameChatResponse);
	}
		break;

	case PACKET_TYPE::PACKET_TYPE_inGameChatBroadcast:
	{
		prop::inGameChatBroadcast *inGameChatBroadcast = new prop::inGameChatBroadcast();
		if (false == inGameChatBroadcast->ParseFromCodedStream(&codeInputStream))
		{
			printf_s("[ERROR] : PacketRecvHandle PACKET_TYPE_inGameChatBroadcast error\n");
			return false;
		}
		PacketRecvMsgHandle(session, inGameChatBroadcast);
	}
		break;

	default:
		printf_s("[ERROR] : PacketRecvHandle default error\n");
		return false;
	}
return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::msgLobby *msgLobby)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::msgRoom *msgRoom)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::msgGame *msgGame)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::msgUserInfo *msgUserInfo)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::msgUserInGameInfo *msgUserInGameInfo)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::msgUserInGameStateInfo *msgUserInGameStateInfo)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::accountCreateRequest *accountCreateRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::accountCreateResponse *accountCreateResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::accountLoginRequest *accountLoginRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::accountLoginResponse *accountLoginResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::accountLogoutRequest *accountLogoutRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::accountLogoutResponse *accountLogoutResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::anyPlayerInfoSelfRequest *anyPlayerInfoSelfRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::anyPlayerInfoOtherRequestWithID *anyPlayerInfoOtherRequestWithID)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::anyPlayerInfoOtherRequestWithNickName *anyPlayerInfoOtherRequestWithNickName)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::anyPlayerInfoResponse *anyPlayerInfoResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::anyWhisperRequestWithUID *anyWhisperRequestWithUID)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::anyWhisperRequestWithID *anyWhisperRequestWithID)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::anyWhisperRequestWithNickName *anyWhisperRequestWithNickName)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::anyWhisperResponse *anyWhisperResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::outLobbyLobbylistRequest *outLobbyLobbylistRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::outLobbyLobbylistResponse *outLobbyLobbylistResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::outLobbyEnterLobbyRequest *outLobbyEnterLobbyRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::outLobbyEnterLobbyResponse *outLobbyEnterLobbyResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyPlayerlistRequest *inLobbyPlayerlistRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyPlayerlistResponse *inLobbyPlayerlistResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyEnterRoomRequest *inLobbyEnterRoomRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyEnterRoomResponse *inLobbyEnterRoomResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyLeaveLobbyRequest *inLobbyLeaveLobbyRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyLeaveLobbyResponse *inLobbyLeaveLobbyResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyChatRequest *inLobbyChatRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyChatResponse *inLobbyChatResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyChatBroadcast *inLobbyChatBroadcast)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomChatRequest *inRoomChatRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomChatResponse *inRoomChatResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomChatBroadcast *inRoomChatBroadcast)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomLeaveRoomRequest *inRoomLeaveRoomRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomLeaveRoomResponse *inRoomLeaveRoomResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomReadyRequest *inRoomReadyRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomReadyResponse *inRoomReadyResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomStartRequest *inRoomStartRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomStartResponse *inRoomStartResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomStartBroadcast *inRoomStartBroadcast)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameSyncPlayerPosToServer *inGameSyncPlayerPosToServer)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameSyncPlayersPosFromServer *inGameSyncPlayersPosFromServer)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameSyncPlayerStateToServer *inGameSyncPlayerStateToServer)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameSyncPlayersStateFromServer *inGameSyncPlayersStateFromServer)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameChatRequest *inGameChatRequest)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameChatResponse *inGameChatResponse)
{
	return true;
}
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameChatBroadcast *inGameChatBroadcast)
{
	return true;
}
