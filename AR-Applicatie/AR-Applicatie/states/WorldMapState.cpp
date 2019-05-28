#include "WorldMapState.h"
#include "../data/DataManager.h"

WorldMapState::WorldMapState() = default;

WorldMapState::~WorldMapState() = default;

void WorldMapState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	map.draw(meshes, textures);
}

void WorldMapState::update(float elapsedTime)
{
	map.update(elapsedTime,
		static_cast<int>(DataManager::getInstance().mousePos.x),
		static_cast<int>(DataManager::getInstance().mousePos.y),
		DataManager::getInstance().height);
}
