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
	DataManager::getInstance().drawDefaultText(DataManager::getInstance().width / 2 - 210, (DataManager::getInstance().height / 2) + 40, "You Lose", GLUT_STROKE_ROMAN, 0.75, 0.75);
	DataManager::getInstance().drawDefaultText(DataManager::getInstance().width - 300, 50, "Return to map", GLUT_STROKE_ROMAN, 0.25, 0.25);
	DataManager::getInstance().drawDefaultText(DataManager::getInstance().width - 300, 100, "Return to game", GLUT_STROKE_ROMAN, 0.25, 0.25);

}

void DeathState::update(float elapsedTime){
	hovering(elapsedTime);
	gameLogic.update(elapsedTime);
}

void DeathState::hovering(float elapsedTime)
{
	checkForReturnGame(elapsedTime);
	checkForReturnMenu(elapsedTime);
}

void DeathState::checkForReturnGame(float elapsedTime) 
{
	if (DataManager::getInstance().mousePos.y >= 5 && DataManager::getInstance().mousePos.y <= 50 &&
		DataManager::getInstance().mousePos.x >= DataManager::getInstance().width - 300 &&
		DataManager::getInstance().mousePos.x <= DataManager::getInstance().width)
	{
		timePassedGame += elapsedTime;

		if (timePassedGame >= 3.0f && timePassedGame <= 3.1f) {
			timePassedGame = 0;
			DataManager::getInstance().soundManager.setVolume(0.2);
			DataManager::getInstance().stateHandler.setState(StateHandler::States::MENU);
			if (!isBackgroundMusicon)
			{
				DataManager::getInstance().soundManager.playSound(SoundManager::Sound::BACKGROUND, true);
				isBackgroundMusicon = true;
			}
		}
		DataManager::getInstance().scaleLoading = timePassedGame * 10;
	}
	else {
		DataManager::getInstance().scaleLoading = 0;
		timePassedGame= 0;
	}
}

void DeathState::checkForReturnMenu(float elapsedTime) 
{
	if (DataManager::getInstance().mousePos.y >= 60 && DataManager::getInstance().mousePos.y <= 110 &&
		DataManager::getInstance().mousePos.x >= DataManager::getInstance().width - 300 &&
		DataManager::getInstance().mousePos.x <= DataManager::getInstance().width)
	{
		timePassedMenu += elapsedTime;

		if (timePassedMenu >= 3.0f && timePassedMenu <= 3.1f) {
			timePassedMenu = 0;
			DataManager::getInstance().soundManager.setVolume(0.2);
			DataManager::getInstance().stateHandler.setState(StateHandler::States::GAME);
			if (!isBackgroundMusicon)
			{
				DataManager::getInstance().soundManager.playSound(SoundManager::Sound::BACKGROUND, true);
				if (!isBackgroundMusicon)
				{
					DataManager::getInstance().soundManager.playSound(SoundManager::Sound::BACKGROUND, true);
					isBackgroundMusicon = true;
				}
			}

		}
		DataManager::getInstance().scaleLoading = timePassedMenu * 10;
	}
	else {
		timePassedMenu = 0;
	}
}

