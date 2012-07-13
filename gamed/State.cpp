#include "State.h"


State::State(StateManager* stateManager)
{
	_stateManager = stateManager;
}


State::~State(void)
{

}

void State::setStateManager(StateManager* stateManager)
{
	_stateManager = stateManager;
}
