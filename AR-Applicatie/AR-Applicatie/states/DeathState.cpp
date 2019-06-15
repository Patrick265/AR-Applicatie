#include "DeathState.h"
#include <GL/freeglut.h>
#include "../data/DataManager.h"

DeathState::DeathState(GameLogic &gameLogic)
	: gameLogic(gameLogic), timePassedGame(0), timePassedMenu(0)
{
	this->width = DataManager::getInstance().width;
	this->height = DataManager::getInstance().height;
}

DeathState::~DeathState() = default;

void DeathState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	
	gameLogic.draw(meshes, textures);
	DataManager::getInstance().drawBackgroundScreen();
	DataManager::getInstance().drawDefaultText(this->width / 2 - 210,
	                                           (this->height / 2) + 40, "You Lose",
	                                           GLUT_STROKE_ROMAN, this->headerSize, this->headerSize);
	DataManager::getInstance().drawDefaultText(this->width - 300, 50, "Return to map",
	                                           GLUT_STROKE_ROMAN, this->textSize, this->textSize);
	DataManager::getInstance().drawDefaultText(this->width - 300, 100, "Return to game",
	                                           GLUT_STROKE_ROMAN, this->textSize, this->textSize);
}

void DeathState::update(const float elapsedTime)
{
	//Checking for mouse pos and if its hovering on the buttons
	hovering(elapsedTime);
	//Updating the gamelogic
	gameLogic.update(elapsedTime);
	//Saving the width every update cycle
	this->width = DataManager::getInstance().width;
	this->height = DataManager::getInstance().height;
}

void DeathState::hovering(const float elapsedTime)
{
	checkForReturnGame(elapsedTime);
	checkForReturnMenu(elapsedTime);
}

void DeathState::checkForReturnGame(const float elapsedTime)
{
	//Checking if mouse pos is in boundaries
	if (DataManager::getInstance().mousePos.y >= 5 && DataManager::getInstance().mousePos.y <= 50 &&
		DataManager::getInstance().mousePos.x >= static_cast<float>(this->width) - 300.0f &&
		DataManager::getInstance().mousePos.x <= static_cast<float>(this->width))
	{
		// Adding to the total time
		timePassedGame += elapsedTime;

		//Checking if time is around 3s this is for the loading animation
		if (timePassedGame >= 3.0f && timePassedGame <= 3.1f)
		{
			//Changing to the menu
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
	else
	{
		//When moving away from the button reset the scale of animation
		DataManager::getInstance().scaleLoading = 0;
		timePassedGame = 0;
	}
}

void DeathState::checkForReturnMenu(const float elapsedTime)
{
	if (DataManager::getInstance().mousePos.y >= 60 && DataManager::getInstance().mousePos.y <= 110 &&
		DataManager::getInstance().mousePos.x >= DataManager::getInstance().width - 300 &&
		DataManager::getInstance().mousePos.x <= DataManager::getInstance().width)
	{
		timePassedMenu += elapsedTime;

		if (timePassedMenu >= 3.0f && timePassedMenu <= 3.1f)
		{
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
	else
	{
		timePassedMenu = 0;
	}
}
