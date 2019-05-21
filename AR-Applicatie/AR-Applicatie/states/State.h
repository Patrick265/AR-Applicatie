#pragma once
#include <map>
#include "../calculations/Graphics.h"

class StateHandler;

class State
{
public:
	State();
	virtual ~State();

	virtual void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) {}
	virtual void update(float elapsedTime) {}
};
