#pragma once
#include "../objects/GameObject.h"
#include <string>

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

	void spawn();
	void update(const float deltaTime);
	void kill();


	const Action &getCurrentAction() { return currentAction; }
	void setCurrentAction(const Action &action) { currentAction = action; }
	const float &getIdleRange() { return IDLE_RANGE; }

private:

	Action currentAction;
	
	/*
		What the player does when Idle

		@param velocity - The current velocity of the player

	*/
	void onIdle(const float velocity);

	/*
		What the player does when falling down
	
		@param velocity - The current velocity of the player

	*/
	void onFalling(const float velocity);

	void toLeft(const float velocity);
	void toRight(const float velocity);

};
