#pragma once
#include "State.h"

class WorldMapState : public State
{
public:
	WorldMapState();
	virtual ~WorldMapState();

	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
};

