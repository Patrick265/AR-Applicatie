#pragma once
#include "../objects/GameObject.h"
#include <string>

class Player : public GameObject
{
	float maxSpeed;
	Math::vec3d velocity;

public:
	bool isDead;
	float targetX;

	Player();

	void spawn();
	void update(float deltaTime);
	void kill();
};
