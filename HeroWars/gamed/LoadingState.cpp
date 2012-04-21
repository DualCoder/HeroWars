#include "LoadingState.h"

#include "StateManager.h"
#include "InGameState.h"

LoadingState::LoadingState(StateManager* stateManager, shared_ptr<Party> currentParty, shared_ptr<PacketManager> packetManager) 
	: GameState(stateManager,currentParty,packetManager)
{
	_loadingFinished = false;
}

LoadingState::~LoadingState()
{

}

void LoadingState::initialize()
{
	GameState::initialize();
	Log::getMainInstance()->writeLine("Entering LoadingState\n");
	_packetManager->registerIPacketHandler(this); //yeah we want to handle packets
}

void LoadingState::release()
{
	GameState::release();
	Log::getMainInstance()->writeLine("Leaving LoadingState\n");
	_packetManager->unregisterIPacketHandler(this);
}

void LoadingState::update(float alphaTime)
{
	GameState::update(alphaTime);
	if(_loadingFinished)
	{
		//if almost everyone logged

	}
	else
	{
		//here we handle someone that try to reconnect

	}
}

bool LoadingState::processPacket(HANDLE_ARGS){
	
	PacketHeader* header = reinterpret_cast<PacketHeader*>(packet->data);
	switch(header->cmd)
	{
	case PKT_KeyCheck:
		if(channelID == CHL_HANDSHAKE) return handleKeyCheck(peer,packet);
			break;
	case PKT_C2S_Ping_Load_Info:
		if(channelID == CHL_C2S) return handleLoadPing(peer,packet);
			break;
	case PKT_C2S_ClientReady:
		if(channelID == CHL_LOADING_SCREEN) return handleMap(peer,packet);
			break;
	case PKT_C2S_SynchVersion:
		if(channelID == CHL_C2S) return handleSynch(peer,packet);
			break;
	case PKT_C2S_GameNumberReq:
		if(channelID == CHL_C2S) return handleGameNumber(peer,packet);
			break;
	case PKT_C2S_QueryStatusReq:
		if(channelID == CHL_C2S) return handleQueryStatus(peer,packet);
			break;
	case PKT_C2S_CharLoaded:
		if(channelID == CHL_C2S) return handleSpawn(peer,packet);
			break;
	}
	return false;
}

bool LoadingState::handleKeyCheck(ENetPeer *peer, ENetPacket *packet)
{
	KeyCheck *keyCheck = (KeyCheck*)packet->data;
	uint64 userId = _packetManager->getBlowFish()->Decrypt(keyCheck->checkId);

	if(userId == keyCheck->userId)
	{
		PDEBUG_LOG_LINE(Log::getMainInstance()," User got the same key as i do, go on!\n");
		peerInfo(peer)->keyChecked = true;
		peerInfo(peer)->userId = userId;
	}
	else
	{
		Log::getMainInstance()->errorLine(" WRONG KEY, GTFO!!!\n");
		return false;
	}

	//Send response as this is correct (OFC DO SOME ID CHECKS HERE!!!)
	KeyCheck response;
	response.userId = keyCheck->userId;
	response.netId = 0;
	
	return _packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&response), sizeof(KeyCheck), CHL_HANDSHAKE);
}

bool LoadingState::handleGameNumber(ENetPeer *peer, ENetPacket *packet)
{
	WorldSendGameNumber world;
	world.gameId = 1;
	memcpy(world.data, peerInfo(peer)->name, peerInfo(peer)->nameLen);

	return _packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&world), sizeof(WorldSendGameNumber), CHL_S2C);
}

bool LoadingState::handleSynch(ENetPeer *peer, ENetPacket *packet)
{
	SynchVersion *version = reinterpret_cast<SynchVersion*>(packet->data);
	Log::getMainInstance()->writeLine("HeroWars version: %s\n", version->version);

	SynchVersionAns answer;
	answer.mapId = 1;
	answer.players[0].userId = peerInfo(peer)->userId;
	answer.players[0].skill1 = SPL_Exhaust;
	answer.players[0].skill2 = SPL_Cleanse;

	_packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&answer), sizeof(SynchVersionAns), CHL_S2C);

	return true;
}

bool LoadingState::handleMap(ENetPeer *peer, ENetPacket *packet)
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

bool LoadingState::handleLoadPing(ENetPeer *peer, ENetPacket *packet)
{
	PingLoadInfo *loadInfo = reinterpret_cast<PingLoadInfo*>(packet->data);

	PingLoadInfo response;
	memcpy(&response, packet->data, sizeof(PingLoadInfo));
	response.header.cmd = PKT_S2C_Ping_Load_Info;
	response.userId = peerInfo(peer)->userId;


	Log::getMainInstance()->writeLine("Loading: loaded: %f, ping: %f, %i, %f\n", loadInfo->loaded, loadInfo->ping, loadInfo->unk4, loadInfo->f3);
	if(100.0f - (loadInfo->loaded) < FLT_EPSILON)
	{
		//!TODO here we should check that everyone is logged, using something like:
		//100 * sizeof(players) - sum(loadInfo->loaded) * players < FLT_EPSILON
		_loadingFinished = true;
	}
	return _packetManager->broadcastPacket(reinterpret_cast<uint8*>(&response), sizeof(PingLoadInfo), 4, UNRELIABLE);
}

bool LoadingState::handleQueryStatus(ENetPeer *peer, ENetPacket *packet)
{
	QueryStatus response;
	return _packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&response), sizeof(QueryStatus), 3);
}

//building the map
bool LoadingState::handleSpawn(ENetPeer *peer, ENetPacket *packet)
{
	//!TODO THESE PACKETS SHOULD BE SEND AT THE BEGGINING OF INGAMESTATE TO EVERY LOGGED PLAYERS AND NOT HERE. I PUT IT HERE TO HELP YOU DEBUG RIGHT NOW.
	_stateManager->pushState(shared_ptr<InGameState>(new InGameState(_stateManager,_currentParty,_packetManager)));

	HeroSpawn spawn;
	spawn.netId = 0x40000019; 
	spawn.gameId = 0;
	memcpy(spawn.name, peerInfo(peer)->name, peerInfo(peer)->nameLen);
	memcpy(spawn.type, peerInfo(peer)->type, peerInfo(peer)->typeLen);

	StatePacket end(PKT_S2C_EndSpawn);

	_packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&spawn), sizeof(HeroSpawn), CHL_S2C);
	_packetManager->sendPacket(peer, reinterpret_cast<uint8*>(&end), sizeof(StatePacket), CHL_S2C);
  	return true;
}