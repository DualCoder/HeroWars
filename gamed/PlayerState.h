#ifndef _PLAYER_STATE_H
#define _PLAYER_STATE_H
#include "State.h"

class PlayerState : public State
{
public:
	PlayerState(StateManager* stateManager);
	~PlayerState();
};
#endif