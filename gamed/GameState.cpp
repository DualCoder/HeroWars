#include "GameState.h"

GameState::GameState(StateManager* stateManager, shared_ptr<Party> currentParty, shared_ptr<PacketManager> packetManager) : State(stateManager), _currentParty(currentParty), _packetManager(packetManager)
{

}

GameState::~GameState()
{

}

void GameState::initialize()
{
	//one day we'll do something here, i'm pretty sure
}

void GameState::release()
{
	//one day we'll do something here, i'm pretty sure
}

void GameState::update(float time)
{
	//one day we'll do something here, i'm pretty sure
}
