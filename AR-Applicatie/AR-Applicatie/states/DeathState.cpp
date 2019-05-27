#include "DeathState.h"
#include <GL/freeglut.h>
#include "../util/TextureHandler.h"
#include "../data/DataManager.h"

DeathState::DeathState() = default;
DeathState::~DeathState() = default;

void DeathState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	DataManager::getInstance().drawBackgroundScreen();
	DataManager::getInstance().DrawScreenText();
}

void DeathState::update(float elapsedTime)
{
}
