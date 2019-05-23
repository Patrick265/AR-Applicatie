#include "Player.h"
#include "../util/ObjLoader.h"
#include "../components/AnimationComponent.h"
#include <string>

Player::Player()
{
	position = { 0, 21, -0.5 };
	maxSpeed = 10;
	targetX = 0;
	isDead = false;
	currentAction = Action::IDLE;
}

void Player::spawn()
{

}

void Player::update(float deltaTime)
{
	GameObject::update(deltaTime);

	if (isDead)
		return;

	float velocity = deltaTime * maxSpeed;

	//Currently running
	if (currentAction == Action::RUNLEFT || currentAction == Action::RUNRIGHT)
	{
		//Close to cursor
		if (abs(position.x - targetX) < velocity)
		{
			if (currentAction != Action::IDLE)
			{
				//Set action to idle
				currentAction = Action::IDLE;
				getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::IDLE);
				position.x = targetX;
			}
		}
		//To the left of the target
		else if (position.x < targetX)
		{
			toRight(velocity);
		}
		//To the right of the target
		else if (position.x > targetX)
		{
			toLeft(velocity);
		}
	}
	//If Idle/Attacking
	else
	{
		onIdle(velocity);
	}


	/*if (!moveRight && position.x > 10 || moveRight && position.x < -10)
		moveRight = !moveRight;
	if (moveRight)
		position.x -= deltaTime * maxSpeed;
	else
		position.x += deltaTime * maxSpeed;*/
}

void Player::kill()
{
	// TODO: Add kill logic
}

void Player::onIdle(float velocity)
{
	//If outside the idle distance
	if (abs(position.x - targetX) >= velocity * 20)
	{
		//To the left of the target
		if (position.x < targetX)
		{
			currentAction = Action::RUNRIGHT;
			getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::RUN_RIGHT);

		}
		//To the right of the target
		else if (position.x > targetX)
		{
			currentAction = Action::RUNLEFT;
			getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::RUN_LEFT);

		}

	}
}

void Player::toLeft(float velocity)
{
	//If currently moving left
	if (currentAction == Action::RUNLEFT)
	{
		//Keep moving
		position.x -= velocity;
	}
	//If not currently moving left
	else
	{
		//Start moving right
		currentAction = Action::RUNLEFT;
		getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::RUN_LEFT);
	}
}

void Player::toRight(float velocity)
{
	//If currently moving right
	if (currentAction == Action::RUNRIGHT)
	{
		//Keep moving
		position.x += velocity;
	}
	//If not currently moving right
	else
	{
		//Start moving right
		currentAction = Action::RUNRIGHT;
		getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::RUN_RIGHT);
	}
}
