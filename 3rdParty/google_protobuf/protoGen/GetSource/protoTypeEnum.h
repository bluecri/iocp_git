enum PACKET_TYPE
{
	PACKET_TYPE_msgLobby,
	PACKET_TYPE_msgRoom,
	PACKET_TYPE_msgGame,
	PACKET_TYPE_msgUserInfo,
	PACKET_TYPE_msgUserInGameInfo,
	PACKET_TYPE_msgUserInGameStateInfo,
	PACKET_TYPE_accountCreate,
	PACKET_TYPE_accountLogin,
	PACKET_TYPE_accountLogout,
	PACKET_TYPE_anyPlayerInfoSelfRequest,
	PACKET_TYPE_anyPlayerInfoOtherRequestWithID,
	PACKET_TYPE_anyPlayerInfoOtherRequestWithNickName,
	PACKET_TYPE_anyPlayerInfoResponse,
	PACKET_TYPE_anyWhisperRequestWithUID,
	PACKET_TYPE_anyWhisperRequestWithID,
	PACKET_TYPE_anyWhisperRequestWithNickName,
	PACKET_TYPE_anyWhisperResponse,
	PACKET_TYPE_outLobbyLobbylistRequest,
	PACKET_TYPE_outLobbyLobbylistResponse,
	PACKET_TYPE_outLobbyEnterLobbyRequest,
	PACKET_TYPE_outLobbyEnterLobbyResponse,
	PACKET_TYPE_inLobbyPlayerlistRequest,
	PACKET_TYPE_inLobbyPlayerlistResponse,
	PACKET_TYPE_inLobbyEnterRoomRequest,
	PACKET_TYPE_inLobbyEnterRoomResponse,
	PACKET_TYPE_inLobbyLeaveLobbyRequest,
	PACKET_TYPE_inLobbyLeaveLobbyResponse,
	PACKET_TYPE_inLobbyChatRequest,
	PACKET_TYPE_inLobbyChatResponse,
	PACKET_TYPE_inLobbyChatBroadcast,
	PACKET_TYPE_inRoomChatRequest,
	PACKET_TYPE_inRoomChatResponse,
	PACKET_TYPE_inRoomChatBroadcast,
	PACKET_TYPE_inRoomLeaveRoomRequest,
	PACKET_TYPE_inRoomLeaveRoomResponse,
	PACKET_TYPE_inRoomReadyRequest,
	PACKET_TYPE_inRoomReadyResponse,
	PACKET_TYPE_inRoomStartRequest,
	PACKET_TYPE_inRoomStartResponse,
	PACKET_TYPE_inRoomStartBroadcast,
	PACKET_TYPE_inGameSyncPlayerPosToServer,
	PACKET_TYPE_inGameSyncPlayersPosFromServer,
	PACKET_TYPE_inGameSyncPlayerStateToServer,
	PACKET_TYPE_inGameSyncPlayersStateFromServer,
	PACKET_TYPE_inGameChatRequest,
	PACKET_TYPE_inGameChatResponse,
	PACKET_TYPE_inGameChatBroadcast
};
