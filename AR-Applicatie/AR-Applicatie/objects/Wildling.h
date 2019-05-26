#pragma once
#include "../objects/GameObject.h"
#include "../objects/Player.h"
#include <string>

class Wildling: public GameObject
{
private:
	//float speed;
	Math::vec3d velocity;

	const float GRAVITY = 10.0f;

	// Reference to the player
	Player *player;

	enum class Action { CLIMB, RUNLEFT, RUNRIGHT, ATTACK, FALLING };
	Action currentAction;

public:

	/*
		Creates a wildling

		@param player - A reference to the player object
		@param x - The x position on which the wildling spawns

	*/
	Wildling(Player *player, float x);

	void spawn();
	void update(float deltaTime);
	bool isHit(float xProjectile, float yProjectile);
	bool canBeDestroyed();
};
