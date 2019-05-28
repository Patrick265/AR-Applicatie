#include "WinState.h"
#include <GL/freeglut.h>
#include "../data/DataManager.h"

WinState::WinState(GameLogic &gameLogic)
	: gameLogic(gameLogic)
{
}

WinState::~WinState() = default;

void WinState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	gameLogic.draw(meshes, textures);
	DataManager::getInstance().drawBackgroundScreen();
	DataManager::getInstance().drawDefaultText(500, 390, "You win", GLUT_STROKE_ROMAN);
}

void WinState::update(float elapsedTime)
{
	gameLogic.update(elapsedTime);
}
