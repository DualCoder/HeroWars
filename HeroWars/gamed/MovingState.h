#ifndef _MOVING_STATE_H
#define _MOVING_STATE_H
#include "PlayerState.h"
class MovingState : public PlayerState
{
public:
	MovingState(StateManager* stateManger);
	~MovingState();
};
#endif