#ifndef _STATE_H
#define _STATE_H
#include <memory>

class StateManager;

class State
{
public:
	State(StateManager* stateManager);
	~State(void);
	void setStateManager(StateManager* stateManager);
	virtual void initialize() = 0;
	virtual void update(float alphaTime) = 0;
	virtual void release() = 0;
protected:
	StateManager* _stateManager;
};
#endif
