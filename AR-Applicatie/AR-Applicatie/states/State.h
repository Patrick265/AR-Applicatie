#pragma once
#include "StateHandler.h"

class State
{
public:
	State();
	~State();

	virtual void on(StateHandler *m);
	virtual void off(StateHandler *m);
};
