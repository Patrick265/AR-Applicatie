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
	DataManager::getInstance().drawDefaultText(500,390,"You lose",GLUT_STROKE_ROMAN);
}

void DeathState::update(float elapsedTime)
{
	gameLogic.update(elapsedTime);
}
