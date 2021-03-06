#pragma once

#include "protoPack.pb.h"

class ClientSession;
class PacketHeader;

bool PacketRecvToMsg(ClientSession* session, PacketHeader& header, google::protobuf::io::CodedInputStream& codeInputStream);
bool PacketRecvMsgHandle(ClientSession * session, prop::msgLobby *msgLobby);
bool PacketRecvMsgHandle(ClientSession * session, prop::msgRoom *msgRoom);
bool PacketRecvMsgHandle(ClientSession * session, prop::msgGame *msgGame);
bool PacketRecvMsgHandle(ClientSession * session, prop::msgUserInfo *msgUserInfo);
bool PacketRecvMsgHandle(ClientSession * session, prop::msgUserInGameInfo *msgUserInGameInfo);
bool PacketRecvMsgHandle(ClientSession * session, prop::msgUserInGameStateInfo *msgUserInGameStateInfo);
bool PacketRecvMsgHandle(ClientSession * session, prop::accountCreateRequest *accountCreateRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::accountCreateResponse *accountCreateResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::accountLoginRequest *accountLoginRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::accountLoginResponse *accountLoginResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::accountLogoutRequest *accountLogoutRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::accountLogoutResponse *accountLogoutResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::anyPlayerInfoSelfRequest *anyPlayerInfoSelfRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::anyPlayerInfoOtherRequestWithID *anyPlayerInfoOtherRequestWithID);
bool PacketRecvMsgHandle(ClientSession * session, prop::anyPlayerInfoOtherRequestWithNickName *anyPlayerInfoOtherRequestWithNickName);
bool PacketRecvMsgHandle(ClientSession * session, prop::anyPlayerInfoResponse *anyPlayerInfoResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::anyWhisperRequestWithUID *anyWhisperRequestWithUID);
bool PacketRecvMsgHandle(ClientSession * session, prop::anyWhisperRequestWithID *anyWhisperRequestWithID);
bool PacketRecvMsgHandle(ClientSession * session, prop::anyWhisperRequestWithNickName *anyWhisperRequestWithNickName);
bool PacketRecvMsgHandle(ClientSession * session, prop::anyWhisperResponse *anyWhisperResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::outLobbyLobbylistRequest *outLobbyLobbylistRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::outLobbyLobbylistResponse *outLobbyLobbylistResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::outLobbyEnterLobbyRequest *outLobbyEnterLobbyRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::outLobbyEnterLobbyResponse *outLobbyEnterLobbyResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyPlayerlistRequest *inLobbyPlayerlistRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyPlayerlistResponse *inLobbyPlayerlistResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyEnterRoomRequest *inLobbyEnterRoomRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyEnterRoomResponse *inLobbyEnterRoomResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyLeaveLobbyRequest *inLobbyLeaveLobbyRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyLeaveLobbyResponse *inLobbyLeaveLobbyResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyChatRequest *inLobbyChatRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyChatResponse *inLobbyChatResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inLobbyChatBroadcast *inLobbyChatBroadcast);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomChatRequest *inRoomChatRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomChatResponse *inRoomChatResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomChatBroadcast *inRoomChatBroadcast);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomLeaveRoomRequest *inRoomLeaveRoomRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomLeaveRoomResponse *inRoomLeaveRoomResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomReadyRequest *inRoomReadyRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomReadyResponse *inRoomReadyResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomStartRequest *inRoomStartRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomStartResponse *inRoomStartResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inRoomStartBroadcast *inRoomStartBroadcast);
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameSyncPlayerPosToServer *inGameSyncPlayerPosToServer);
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameSyncPlayersPosFromServer *inGameSyncPlayersPosFromServer);
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameSyncPlayerStateToServer *inGameSyncPlayerStateToServer);
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameSyncPlayersStateFromServer *inGameSyncPlayersStateFromServer);
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameChatRequest *inGameChatRequest);
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameChatResponse *inGameChatResponse);
bool PacketRecvMsgHandle(ClientSession * session, prop::inGameChatBroadcast *inGameChatBroadcast);
