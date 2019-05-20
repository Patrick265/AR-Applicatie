#pragma once

#include "Component.h"

class StaticComponent : public Component
{
public:
	StaticComponent();
	~StaticComponent();

	void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures) override;
};
