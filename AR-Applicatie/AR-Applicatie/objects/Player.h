#pragma once
#include "../objects/GameObject.h"

class Player : public GameObject
{
	float maxSpeed;
	Math::vec3d velocity;

	const float GRAVITY = 10.0f;

	//The distance until the player will follow the mouse again
	const float IDLE_RANGE = 2.0f;

public:
	enum class Action { IDLE, RUNLEFT, RUNRIGHT, ATTACK, FALLING };

	float targetX;

	Player();

	void update(float deltaTime);

	Action &getCurrentAction() { return currentAction; }
	void setCurrentAction(const Action &action) { currentAction = action; }
	float getIdleRange() const { return IDLE_RANGE; }

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
