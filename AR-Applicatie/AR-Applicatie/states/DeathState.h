#ifndef DEATHSTATE
#define DEATHSTATE


#include "State.h"
#include "../game/GameLogic.h"

class DeathState : public State
{
	GameLogic gameLogic;
	
	float timePassedGame;
	float timePassedMenu;
	bool isBackgroundMusicon = false;
	int width;
	int height;
	const float headerSize = 0.75f;
	const float textSize = 0.25f;
	std::vector<std::string> text;
public:
	DeathState(GameLogic &gameLogic);
	virtual ~DeathState();
	//Drawing the death screen
	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	// Update method for the death screen
	void update(float elapsedTime) override;
	// Checking for the two button
	void hovering(float elapsedTime);

	//Checking if mouse is over the button returning game
	void checkForReturnGame(float elapsedTime);

	//Checking if mouse is over the button returning menu
	void checkForReturnMenu(float elapsedTime);
};
#endif // !DEATHSTATE