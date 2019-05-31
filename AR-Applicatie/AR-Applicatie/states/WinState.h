#pragma once

#include "State.h"
#include "../game/GameLogic.h"

class WinState : public State
{
	GameLogic gameLogic;
	float timePassedGame;
	float timePassedMenu;
	bool isBackgroundMusicon = false;
public:
	WinState(GameLogic &gameLogic);
	virtual ~WinState();
	
	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
	void hovering(float elapsedTime);
	void checkForReturnGame(float elapsedTime);
	void checkForReturnMenu(float elapsedTime);
};
