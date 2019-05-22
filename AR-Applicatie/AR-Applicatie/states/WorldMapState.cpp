#include "WorldMapState.h"

WorldMapState::WorldMapState() = default;

WorldMapState::~WorldMapState() = default;

void WorldMapState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	map.draw(meshes, textures);
}

void WorldMapState::update(float elapsedTime)
{
	map.update(elapsedTime , 0, 0, 1080);
}
