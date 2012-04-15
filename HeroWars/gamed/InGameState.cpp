#include "InGameState.h"

InGameState::InGameState(StateManager* stateManager, shared_ptr<Party> currentParty) : GameState(stateManager,currentParty)
{

}

InGameState::~InGameState()
{

}