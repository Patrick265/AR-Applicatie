#pragma once
#include "../objects/GameObject.h"
#include <string>

class Player : public GameObject
{
	float maxSpeed;
	Math::vec3d velocity;

	

	const float IDLE_RANGE = 2.0f;

public:
	enum class Action { IDLE, RUNLEFT, RUNRIGHT, ATTACK };

	bool isDead;
	float targetX;

	Player();

	void spawn();
	void update(float deltaTime);
	void kill();


	const Action &getCurrentAction() { return currentAction; }
	const float &getIdleRange() { return IDLE_RANGE; }

private:

	Action currentAction;
	
	/*
		What the player does when Idle

		@param velocity - The current velocity of the player

	*/
	void onIdle(float velocity);

	void toLeft(float velocity);
	void toRight(float velocity);

};
