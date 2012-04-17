#ifndef _LOADING_STATE_H
#define _LOADING_STATE_H
#include "GameState.h"

/*
* This state will handle all packets related to loading\connecting stuff
* After everyone logged (or almost), the LoadingState will still in background of InGameState in case someone want to reconnect (or late connect)
*/
class LoadingState : public GameState
{
public:
	LoadingState(StateManager* stateManager, shared_ptr<Party> currentParty, shared_ptr<PacketManager> packetManager);
	~LoadingState();
	
	virtual void initialize();
	virtual void update(float alphaTime);
	virtual void release();
	virtual bool processPacket(HANDLE_ARGS);

	
private:
	//Packet Handlers
	bool handleKeyCheck(ENetPeer *peer, ENetPacket *packet);
	bool handleGameNumber(ENetPeer *peer, ENetPacket *packet);
	bool handleSynch(ENetPeer *peer, ENetPacket *packet);
	bool handleMap(ENetPeer *peer, ENetPacket *packet);
	bool handleLoadPing(ENetPeer *peer, ENetPacket *packet);
	bool handleQueryStatus(ENetPeer *peer, ENetPacket *packet);
	bool handleSpawn(ENetPeer *peer, ENetPacket *packet); //This should be move in InGameState but later
private:
	bool _loadingFinished;
};
#endif