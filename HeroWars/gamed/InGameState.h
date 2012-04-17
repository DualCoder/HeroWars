#ifndef _IN_GAME_STATE_H
#define _IN_GAME_STATE_H
#include "GameState.h"

/*
* This state is started when everyone is logged (or at least most of them) and handle the real game mechanisms
*/
class InGameState : public GameState
{
public:
	InGameState(StateManager* stateManager, shared_ptr<Party> currentParty, shared_ptr<PacketManager> PacketManager);
	~InGameState();

	virtual void initialize();
	virtual void update(float alphaTime);
	virtual void release();
	virtual bool processPacket(HANDLE_ARGS);

	//Packet Handlers
	bool InGameState::handleMap(ENetPeer *peer, ENetPacket *packet);	
	bool InGameState::handleInit(ENetPeer *peer, ENetPacket *packet);
	bool InGameState::handleAttentionPing(ENetPeer *peer, ENetPacket *packet);
	bool InGameState::handleView(ENetPeer *peer, ENetPacket *packet);
	bool InGameState::handleMove(ENetPeer *peer, ENetPacket *packet);
	bool InGameState::handleChatBoxMessage(ENetPeer *peer, ENetPacket *packet);
	bool InGameState::handleSkillUp(ENetPeer *peer, ENetPacket *packet);
};
#endif