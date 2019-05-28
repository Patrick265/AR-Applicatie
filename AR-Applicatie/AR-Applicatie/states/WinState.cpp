#include "WinState.h"
#include <GL/freeglut.h>
#include "../util/TextureHandler.h"
#include "../data/DataManager.h"

WinState::WinState() = default;

WinState::~WinState() = default;

void WinState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	DataManager::getInstance().drawBackgroundScreen();
	DataManager::getInstance().drawDefaultText(500, 390, "You win", GLUT_STROKE_ROMAN);
}

void WinState::update(float elapsedTime)
{
}
