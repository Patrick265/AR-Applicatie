#include "DeathState.h"
#include <GL/freeglut.h>
#include "../data/DataManager.h"

DeathState::DeathState(GameLogic &gameLogic)
: gameLogic(gameLogic)
{
}

DeathState::~DeathState() = default;

void DeathState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	gameLogic.draw(meshes, textures);
	DataManager::getInstance().drawBackgroundScreen();
	DataManager::getInstance().drawDefaultText(450,400,"You lose",GLUT_STROKE_ROMAN,0.75,0.75);
	DataManager::getInstance().drawDefaultText(DataManager::getInstance().width - 250, 50, "Return to map", GLUT_STROKE_ROMAN,0.25,0.25);

}

void DeathState::update(float elapsedTime)
	gameLogic.update(elapsedTime);
void DeathState::update(float elapsedTime){
	if (DataManager::getInstance().mousePos.y >= 5 && DataManager::getInstance().mousePos.y <= 50 &&
		DataManager::getInstance().mousePos.x >= DataManager::getInstance().width - 250 && 
		DataManager::getInstance().mousePos.x <= DataManager::getInstance().width)
	{
		timePassed += elapsedTime;

		if (timePassed >= 3.0f && timePassed <= 3.1f){
				timePassed = 0;
				DataManager::getInstance().stateHandler.setState(StateHandler::States::MENU);
		}
		DataManager::getInstance().scaleLoading = timePassed * 10;
	}
	else {
		DataManager::getInstance().scaleLoading = 0;
		timePassed = 0;
	}
}
		

