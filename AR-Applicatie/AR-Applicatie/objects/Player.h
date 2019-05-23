#pragma once
#include "../objects/GameObject.h"
#include <string>

class Player : public GameObject
{
	float maxSpeed;
	Math::vec3d velocity;


	enum class Action {IDLE, RUNLEFT, RUNRIGHT, ATTACK};

	Action currentAction;

public:
	bool isDead;
	float targetX;

	Player();

	void spawn();
	void update(float deltaTime);
	void kill();


private:
	/*
		What the player does when Idle

		@param velocity - The current velocity of the player

	*/
	void onIdle(float velocity);

	void toLeft(float velocity);
	void toRight(float velocity);

};
