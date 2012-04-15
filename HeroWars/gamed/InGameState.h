#ifndef _IN_GAME_STATE_H
#define _IN_GAME_STATE_H
#include "GameState.h"

class InGameState : public GameState
{
public:
	InGameState(StateManager* stateManager, shared_ptr<Party> currentParty);
	~InGameState();
};
#endif