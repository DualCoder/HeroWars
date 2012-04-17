#include "StateManager.h"
#include "Log.h"

#include <memory>
#include <iterator>

StateManager::StateManager(void)
{

}

StateManager::~StateManager(void)
{
	
}

void StateManager::pushState(shared_ptr<State> s)
{
	s->setStateManager(this);
	_states.push_back(s);
	s->initialize();
}

void StateManager::popState()
{
	if (!_states.empty())
	{
		shared_ptr<State> state = _states.back();
		_states.pop_back();
		state->release();
	}
}

void StateManager::update(float alphaTime)
{
	for(auto it =  _states.begin(); it != _states.end(); ++it)
	{
		it->get()->update(alphaTime);
	}
}

bool StateManager::isEmpty()
{
	return _states.empty();
}