#ifndef _GAME_STATE_H
#define _GAME_STATE_H
#include "State.h"
#include "Party.h"
#include "PacketManager.h"

#include <memory>

using namespace std;
/**
* GameState represent all game step: LoadingState then InGameState
*/
class GameState : public State , public IPacketHandler
{
public:
	GameState(StateManager* stateManager, shared_ptr<Party> currentParty, shared_ptr<PacketManager> packetManager);
	~GameState(void);
	
	virtual void initialize();
	virtual void release();
	virtual void update(float alphaTime);
	virtual bool processPacket(HANDLE_ARGS) = 0; //Maybe one day GameState will handle packet that can be received from both states
protected:
	shared_ptr<Party> _currentParty;
	shared_ptr<PacketManager> _packetManager;
};
#endif