#pragma once

#include "State.h"
#include "../game/GameLogic.h"

class GameState : public State
{
public:
	GameLogic gameLogic;

	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
};
