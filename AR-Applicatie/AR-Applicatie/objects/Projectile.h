#pragma once
#include "../objects/GameObject.h"
#include <string>

class Projectile : public GameObject
{
private:
	int gravity;
	Math::vec3d velocity;

public:
	bool isActive;

	Projectile(std::string objectName, std::string texture, float x, float xVelocity, float yVelocity);

	void update(float deltaTime);
	void hasHit();
	bool canBeDestroyed();
};
