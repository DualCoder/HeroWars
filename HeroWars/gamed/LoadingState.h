#ifndef _LOADING_STATE_H
#define _LOADING_STATE_H
#include "GameState.h"

class LoadingState : public GameState
{
public:
	LoadingState(StateManager* stateManager, shared_ptr<Party> currentParty);
	~LoadingState();
	void processPackets();
	void initialize();
	void update(float alphaTime);
	void release();
};
#endif