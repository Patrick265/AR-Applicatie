#include "DeathState.h"
#include <GL/freeglut.h>
#include "../util/TextureHandler.h"
#include "../data/DataManager.h"

DeathState::DeathState() = default;
DeathState::~DeathState() = default;

void DeathState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	DataManager::getInstance().drawBackgroundScreen();
	DataManager::getInstance().drawDefaultText(DataManager::getInstance().width / 2 - 210, (DataManager::getInstance().height / 2) + 40, "You Lose", GLUT_STROKE_ROMAN, 0.75, 0.75);
	DataManager::getInstance().drawDefaultText(DataManager::getInstance().width - 300, 50, "Return to map", GLUT_STROKE_ROMAN, 0.25, 0.25);
	DataManager::getInstance().drawDefaultText(DataManager::getInstance().width - 300, 100, "Return to game", GLUT_STROKE_ROMAN, 0.25, 0.25);

}

void DeathState::update(float elapsedTime){
	hovering(elapsedTime);
}

void DeathState::hovering(float elapsedTime)
{
	if (DataManager::getInstance().mousePos.y >= 5 && DataManager::getInstance().mousePos.y <= 50 &&
		DataManager::getInstance().mousePos.x >= DataManager::getInstance().width - 300 &&
		DataManager::getInstance().mousePos.x <= DataManager::getInstance().width)
	{
		timePassed += elapsedTime;

		if (timePassed >= 3.0f && timePassed <= 3.1f) {
			timePassed = 0;
			DataManager::getInstance().stateHandler.setState(StateHandler::States::MENU);
		}
		DataManager::getInstance().scaleLoading = timePassed * 10;
	}
	else if (DataManager::getInstance().mousePos.y >= 60 && DataManager::getInstance().mousePos.y <= 110 &&
		DataManager::getInstance().mousePos.x >= DataManager::getInstance().width - 300 &&
		DataManager::getInstance().mousePos.x <= DataManager::getInstance().width)
	{
		timePassed += elapsedTime;

		if (timePassed >= 3.0f && timePassed <= 3.1f) {
			timePassed = 0;
			DataManager::getInstance().stateHandler.setState(StateHandler::States::GAME);
		}
		DataManager::getInstance().scaleLoading = timePassed * 10;
	}
	else {
		DataManager::getInstance().scaleLoading = 0;
		timePassed = 0;
	}
}

