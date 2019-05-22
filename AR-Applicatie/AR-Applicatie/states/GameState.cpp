#include "GameState.h"

GameState::GameState() = default;

GameState::~GameState() = default;

void GameState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	gameLogic.draw(meshes, textures);
}

void GameState::update(float elapsedTime)
{
	gameLogic.update(elapsedTime);
}
