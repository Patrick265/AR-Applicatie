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

	Player(std::string objectName, int textureId);

	void spawn();
	void update(float deltaTime);
	void kill();
};
