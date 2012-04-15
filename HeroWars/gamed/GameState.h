#ifndef _GAME_STATE_H
#define _GAME_STATE_H
#include "State.h"
#include "Party.h"

#include <memory>

using namespace std;

class GameState : public State
{
public:
	GameState(StateManager* stateManager, shared_ptr<Party> currentParty);
	~GameState(void);
	virtual void processPackets();
private:
	shared_ptr<Party> _currentParty;
};
#endif