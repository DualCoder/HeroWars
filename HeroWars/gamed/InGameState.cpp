#include "InGameState.h"

#include "StateManager.h"
InGameState::InGameState(StateManager* stateManager, shared_ptr<Party> currentParty, shared_ptr<PacketManager> packetManager) : GameState(stateManager,currentParty, packetManager)
{

}

InGameState::~InGameState()
{

}

void InGameState::initialize()
{
	GameState::initialize();
	Log::getMainInstance()->writeLine("Entering InGameState\n");
	_packetManager->registerIPacketHandler(this);
}

void InGameState::release()
{
	GameState::release();
	Log::getMainInstance()->writeLine("Leaving InGameState\n");
	_packetManager->unregisterIPacketHandler(this);
}

void InGameState::update(float alphaTime)
{
	GameState::update(alphaTime);
	//!TODO main loop everything will be done there : IA , towers, players, sbires and even the almighty nashor!!!
}

bool InGameState::processPacket(HANDLE_ARGS)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(packet->data);
	switch(header->cmd)
	{
	case PKT_C2S_StartGame:
		if(channelID == CHL_C2S) return handleInit(peer,packet);
			break;
	case PKT_C2S_ViewReq:
		if(channelID == CHL_C2S) return handleView(peer,packet);
			break;
	case PKT_C2S_AttentionPing:
		if(channelID == CHL_C2S) return handleAttentionPing(peer,packet);
			break;
	case PKT_ChatBoxMessage:
		if(channelID == CHL_COMMUNICATION) return handleChatBoxMessage(peer,packet);
			break;
	case PKT_C2S_MoveReq:
		if(channelID == CHL_C2S) return handleMove(peer,packet);
			break;
	case PKT_C2S_SkillUp:
		if(channelID == CHL_C2S) return handleSkillUp(peer,packet);
			break;
	}
	return false;
	return false;
}

bool InGameState::handleMap(ENetPeer *peer, ENetPacket *packet)
{
	LoadScreenPlayer *playerName = LoadScreenPlayer::create(PKT_S2C_LoadName, peerInfo(peer)->name, peerInfo(peer)->nameLen);
	playerName->userId = peerInfo(peer)->userId;

	LoadScreenPlayer *playerHero = LoadScreenPlayer::create(PKT_S2C_LoadHero,  peerInfo(peer)->type, peerInfo(peer)->typeLen);
	playerHero->userId = peerInfo(peer)->userId;
	playerHero->skinId = 4;

	//Builds team info
	LoadScreenInfo screenInfo;
	screenInfo.bluePlayerNo = 1;
	screenInfo.redPlayerNo = 0;

	screenInfo.bluePlayerIds[0] = peerInfo(peer)->userId;
	
	bool pInfo = _packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&screenInfo), sizeof(LoadScreenInfo), CHL_LOADING_SCREEN);

	//For all players send this info
	bool pName = _packetManager->sendPacket(peer, reinterpret_cast<uint8*>(playerName), playerName->getPacketLength(), CHL_LOADING_SCREEN);
	bool pHero = _packetManager->sendPacket(peer, reinterpret_cast<uint8*>(playerHero), playerHero->getPacketLength(), CHL_LOADING_SCREEN);

	//cleanup
	LoadScreenPlayer::destroy(playerName);
	LoadScreenPlayer::destroy(playerHero);

	return (pInfo && pName && pHero);

}


bool InGameState::handleInit(ENetPeer *peer, ENetPacket *packet)
{
	StatePacket start(PKT_S2C_StartGame);
	_packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&start), sizeof(StatePacket), CHL_S2C);
	//_packetManager->sendPacket(peer, reinterpret_cast<uint8*>(resp), sizeof(resp), 3);
	//sendPacket(peer, reinterpret_cast<uint8*>(fog), sizeof(fog), 4);
	
	return true;

}

bool InGameState::handleAttentionPing(ENetPeer *peer, ENetPacket *packet)
{
	AttentionPing *ping = reinterpret_cast<AttentionPing*>(packet->data);
	AttentionPingAns response(ping);

	Log::getMainInstance()->writeLine("Plong x: %f, y: %f, z: %f, type: %i\n", ping->x, ping->y, ping->z, ping->type);

	return _packetManager->broadcastPacket(reinterpret_cast<uint8*>(&response), sizeof(AttentionPing), 3);
}

bool InGameState::handleView(ENetPeer *peer, ENetPacket *packet)
{
	ViewReq *request = reinterpret_cast<ViewReq*>(packet->data);

	Log::getMainInstance()->writeLine("View (%i), x:%f, y:%f, zoom: %f\n", request->requestNo, request->x, request->y, request->zoom);

	ViewAns answer;
	answer.requestNo = request->requestNo;
	return _packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&answer), sizeof(ViewAns), 3, UNRELIABLE);
}

bool InGameState::handleMove(ENetPeer *peer, ENetPacket *packet)
{
	MoveReq *request = reinterpret_cast<MoveReq*>(packet->data);
	//MoveReqAns response(request);
	Log::getMainInstance()->writeLine("Move to: x(left->right):%f, y(height):%f, z(bot->top): %f\n", request->x, request->y, request->z);
	//return _packetManager->broadcastPacket(reinterpret_cast<uint8*>(&response), sizeof(MoveReqAns), 3, UNRELIABLE);
	return true;
}

bool InGameState::handleChatBoxMessage(ENetPeer *peer, ENetPacket *packet)
{
	ChatBoxMessage* message = reinterpret_cast<ChatBoxMessage*>(packet->data);

	switch(message->cmd)
	{
	case CMT_ALL:
	//!TODO make a player class and foreach player in game send the message
		return _packetManager->sendPacket(peer,packet->data,packet->dataLength,CHL_COMMUNICATION);
		break;
	case CMT_TEAM:
	//!TODO make a team class and foreach player in the team send the message
		return _packetManager->sendPacket(peer,packet->data,packet->dataLength,CHL_COMMUNICATION);
		break;
	default:
		Log::getMainInstance()->errorLine("Unknow ChatMessageType");
		break;
	}
	return false;
}

bool InGameState::handleSkillUp(ENetPeer *peer, ENetPacket *packet) {

	SkillUpPacket* skillUpPacket = reinterpret_cast<SkillUpPacket*>(packet->data);
	//!TODO Check if can up skill? :)
	SkillUpResponse skillUpResponse;
	
	skillUpResponse.skill = skillUpPacket->skill;
	skillUpResponse.level = 0x0001;
		uint8 cmd1[] = {0xFF, 0x06, 0x07, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x52, 0x00, 0xBA, 0x19, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x40, 0xFF, 0x01, 0x00, 0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6C, 0x74, 0x73, 0x73, 0x74, 0x61, 0x72, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2C, 0xF8, 0x18, 0x00, 0x70, 0x7B, 0x2B, 0x03, 0xAE, 0x83, 0x76, 0xB7, 0xF6, 0x24, 0x68, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x68, 0xF8, 0x18, 0x00, 0x1A, 0xF6, 0x76, 0xB7, 0x40, 0xF9, 0x18, 0x00, 0x44, 0xF9, 0x18, 0x00, 0x18, 0x25, 0x68, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4C, 0xF8, 0x18, 0x01, 0xC8, 0xF7, 0x18, 0x00, 0x68, 0xF8, 0x18, 0x00, 0x70, 0x7B, 0x2B, 0x03, 0xED, 0x88, 0xED, 0x76, 0xD8, 0x31, 0x73, 0x04, 0x00, 0x00, 0x00, 0x00, 0x32, 0x0B, 0x45, 0x00, 0xF0, 0xCA, 0xAD, 0x02, 0x34, 0xF8, 0x18, 0x00, 0x2C, 0xF8, 0x18, 0x00, 0x26, 0xF6, 0x76, 0xB7, 0x68, 0x00, 0x00, 0x00, 0x64, 0xFA, 0x18, 0x52, 0x79, 0x7A, 0x65, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x68, 0x78, 0xE9, 0x02, 0x02, 0x00, 0x00, 0x00, 0xCF, 0xEE, 0x61, 0x00, 0x07, 0x00, 0x00, 0x00, 0x68, 0xF8, 0x18, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x14, 0x8C, 0x19, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00};
		_packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&cmd1), sizeof(cmd1), 3, UNRELIABLE);

	return _packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&skillUpResponse),sizeof(skillUpResponse),CHL_GAMEPLAY);

}