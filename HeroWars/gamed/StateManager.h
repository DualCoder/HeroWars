#ifndef _STATE_MANAGER_H
#define _STATE_MANAGER_H
#include "State.h"

#include <stack>
#include <memory>
using namespace std;

class StateManager
{
public:
	StateManager(void);
	~StateManager(void);
	void pushState(shared_ptr<State> s);
	void popState();
	void update(float alphaTime);	
	bool isEmpty();
private:
	std::stack<shared_ptr<State>> _states;
};
#endif
