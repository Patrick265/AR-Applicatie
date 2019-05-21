#pragma once

#include "Component.h"

class StaticComponent : public Component
{
	std::string mesh;
	std::string texture;
public:
	StaticComponent(const std::string &mesh, const std::string &texture);
	~StaticComponent();

	void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures) override;
};
