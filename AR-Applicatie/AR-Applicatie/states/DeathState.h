#pragma once

#include "State.h"
#include "../game/GameLogic.h"

class DeathState : public State
{
	GameLogic gameLogic;
	std::vector<std::string> text;
public:
	DeathState(GameLogic &gameLogic);
	virtual ~DeathState();

	float timePassed;
	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
};
