#include "GameState.h"
#include <iostream>

GameState::GameState() = default;

GameState::~GameState() = default;

void GameState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	std::cout << "GameState draw...";
}

void GameState::update(float elapsedTime)
{
	std::cout << "GameState update...";
}
