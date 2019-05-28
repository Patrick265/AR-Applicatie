#pragma once
#include "../objects/GameObject.h"
#include "../objects/Player.h"

class Wildling: public GameObject
{
private:
	//float speed;
	Math::vec3d velocity;

	const float GRAVITY = 10.0f;

	// Reference to the player and wildlings
	Player *player;
	std::vector<Wildling *> *wildlings;

	enum class Action { CLIMB, RUNLEFT, RUNRIGHT, PULL_UP, ATTACK, FALLING, CHEER };
	Action currentAction;

public:

	/*
		Creates a wildling

		@param player - A reference to the player object
		@param x - The x position on which the wildling spawns

	*/
	Wildling(Player *player, std::vector<Wildling *> *wildlings, float x);

	void spawn();
	void update(const float deltaTime);
	bool isHit(float xProjectile, float yProjectile);
	void die();
	bool canBeDestroyed();

private:

	void climb(const float deltaTime);
	void pullUp(const float deltaTime);
	void run(const float deltaTime);
	void attack(const float deltaTime);
};
