#pragma once

#include "../calculations/Graphics.h"

class GameObject {

private:

	Math::vec3d position;
	Math::vec3d rotation;
	Math::vec3d scale;

	Graphics::mesh mesh;
	uint16_t texture_id;


public:
	GameObject(Graphics::mesh mesh, uint16_t texture_id);

	void setPosition(Math::vec3d pos);
	void setRotation(Math::vec3d rot);
	void setScale(Math::vec3d scale);

	Math::vec3d getPosition();
	Math::vec3d getRotation();
	Math::vec3d getScale();

	Graphics::mesh getMesh();
	uint16_t getTextureId();
};