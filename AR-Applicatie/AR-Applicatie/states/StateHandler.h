#pragma once
#include <map>
#include "../calculations/Graphics.h"
#include "State.h"

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

	void update(float elapsedTime) const;
	void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures) const;
	void setState(States state);
	State &getState() const;
};
