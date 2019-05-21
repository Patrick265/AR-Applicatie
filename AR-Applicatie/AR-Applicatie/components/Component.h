#pragma once
#include <map>
#include "../calculations/Graphics.h"

class GameObject;

class Component
{
protected:
	GameObject *gameObject{};
public:
	Component();
	virtual ~Component();

	virtual void update(float elapsedTime) {}
	virtual void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures) {}

	void setGameObject(GameObject *gameObject) { this->gameObject = gameObject; }
};
