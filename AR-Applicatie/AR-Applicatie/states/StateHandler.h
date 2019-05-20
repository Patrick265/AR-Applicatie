#pragma once
#include "State.h"
#include <map>
#include "../calculations/Graphics.h"

class StateHandler
{
	enum class States
	{
		MENU, GAME, DEATH
	};

	State *current;
public:
	StateHandler();
	~StateHandler();

	void update(float elapsedTime);
	void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures);

	void setState(State *state);
};
