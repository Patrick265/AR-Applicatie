#include "Player.h"
#include "../util/ObjLoader.h"
#include "../components/AnimationComponent.h"
#include <string>

Player::Player()
{
	position = { 0, 21, -0.5 };
	maxSpeed = 10;
	targetX = 0;
	currentAction = Action::IDLE;
}

void Player::update(const float deltaTime)
{
	GameObject::update(deltaTime);

	//If the player is currently falling
	if (currentAction == Action::FALLING)
	{
		velocity.y -= deltaTime * GRAVITY;
		position.y += deltaTime * velocity.y;
	}
	else
	{
		velocity.x = deltaTime * maxSpeed;

		//Currently running
		if (currentAction == Action::RUNLEFT || currentAction == Action::RUNRIGHT)
		{
			//Close to cursor
			if (abs(position.x - targetX) < velocity.x)
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
				toRight(velocity.x);
			}
			//To the right of the target
			else if (position.x > targetX)
			{
				toLeft(velocity.x);
			}
		}
		//If Idle/Attacking
		else
		{
			onIdle(velocity.x);
		}
	}
}

void Player::onIdle(const float velocity)
{
	//If outside the idle distance
	if (abs(position.x - targetX) >= 2)
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
	//If in idle distance
	else
	{
		if (currentAction == Action::IDLE)
		{
			currentAction = Action::ATTACK;
			getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::ATTACK_MOUSE);
		}
	}
}

void Player::toLeft(const float velocity)
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

void Player::toRight(const float velocity)
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
