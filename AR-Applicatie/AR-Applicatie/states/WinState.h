#pragma once

#include "State.h"
#include "../game/GameLogic.h"

class WinState : public State
{
	GameLogic gameLogic;
public:
	WinState(GameLogic &gameLogic);
	virtual ~WinState();

	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
};
