#include "Player.h"
#include "../util/ObjLoader.h"
#include "../components/AnimationComponent.h"
#include <string>

Player::Player()
{
	position = {0, 21, -0.5};
	maxSpeed = 10;
	targetX = 0;
	isDead = false;
	isMoving = false;
}

void Player::spawn()
{

}

void Player::update(float deltaTime)
{
	GameObject::update(deltaTime);

	if (isDead)
		return;

	float travel = deltaTime * maxSpeed;

	if (position.x == targetX) 
	{
	}
	if (abs(position.x - targetX) < travel) 
	{
		if (isMoving)
		{
			isMoving = false;
			getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animations::IDLE);
		}
		//position.x = targetX;
	}		
	else if (position.x < targetX) 
	{
		if (!isMoving)
		{
			isMoving = true;
			getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animations::RUN_RIGHT);

		}
		position.x += travel;
	}		
	else 
	{
		if (!isMoving)
		{
			isMoving = true;
			getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animations::RUN_LEFT);

		}
		position.x -= travel;
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
