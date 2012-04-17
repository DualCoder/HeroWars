#ifndef _STATE_MANAGER_H
#define _STATE_MANAGER_H
#include "State.h"

#include <vector>
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
	std::vector<shared_ptr<State>> _states;
};
#endif
