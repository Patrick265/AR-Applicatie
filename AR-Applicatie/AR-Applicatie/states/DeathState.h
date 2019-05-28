#pragma once

#include "State.h"
#include "../game/GameLogic.h"

class DeathState : public State
{
	GameLogic gameLogic;
private:
	float timePassedGame;
	float timePassedMenu;
	bool isBackgroundMusicon = false;
	std::vector<std::string> text;
public:
	DeathState(GameLogic &gameLogic);
	virtual ~DeathState();


	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
	void hovering(float elapsedTime);
	void checkForReturnGame(float elapsedTime);
	void checkForReturnMenu(float elapsedTime);
};
