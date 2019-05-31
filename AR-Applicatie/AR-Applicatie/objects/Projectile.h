#pragma once

#include "../objects/GameObject.h"

class Projectile : public GameObject
{
	int gravity;
	Math::vec3d velocity;

public:
	bool isActive;

	Projectile(float x, float xVelocity, float yVelocity);

	void update(float deltaTime);
	void hasHit();
	bool canBeDestroyed() const;
};
