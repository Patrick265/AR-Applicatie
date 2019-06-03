#include "WorldMapState.h"
#include "../data/DataManager.h"

/*
This class manages the state for the worldmap
*/

WorldMapState::WorldMapState() = default;

WorldMapState::~WorldMapState() = default;

void WorldMapState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	// Drawing the whole map
	map.draw(meshes, textures);
}

void WorldMapState::update(float elapsedTime)
{
	// Executing the update method from the map object with correct parameters
	map.update(elapsedTime,
		static_cast<int>(DataManager::getInstance().mousePos.x),
		static_cast<int>(DataManager::getInstance().mousePos.y),
		DataManager::getInstance().height);
}
