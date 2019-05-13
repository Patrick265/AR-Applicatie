#pragma once
#include "../objects/GameObject.h"
#include <string>

class Wildling: public GameObject
{
private:
	float speed;
	Math::vec3d velocity;

public:
	bool isDead;

	Wildling(std::string objectName, int textureId, float x);

	void spawn();
	void update(float deltaTime);
	bool isHit(float xProjectile, float yProjectile);
	bool canBeDestroyed();
};
