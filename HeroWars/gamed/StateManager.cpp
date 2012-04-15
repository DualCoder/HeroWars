#include "StateManager.h"
#include "Log.h"
#include <memory>

StateManager::StateManager(void)
{

}

StateManager::~StateManager(void)
{
	
}

void StateManager::pushState(shared_ptr<State> s)
{
	s->setStateManager(this);
	_states.push(s);
}

void StateManager::popState()
{
	if (!_states.empty())
	{
		shared_ptr<State> state = _states.top();
		_states.pop();
		state->release();
	}
}

void StateManager::update(float time)
{
	if (!_states.empty())
	{
		_states.top()->update(time);
	}
}

bool StateManager::isEmpty()
{
	return _states.empty();
}