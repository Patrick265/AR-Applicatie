#pragma once
#include "State.h"
#include "../game/WorldMap.h"

class WorldMapState : public State
{
private:
	WorldMap map;

public:

	WorldMapState();
	virtual ~WorldMapState();

	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
};

