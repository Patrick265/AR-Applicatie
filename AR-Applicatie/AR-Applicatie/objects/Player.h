#pragma once
#include "../objects/GameObject.h"
#include <string>

class Player : public GameObject
{
private:
	float maxSpeed;
	Math::vec3d velocity;

public:
	bool isDead;
	float targetX;

	Player(const std::string &objectName, const std::string &texture);

	void spawn();
	void update(float deltaTime);
	void kill();
};
