#include "Player.h"
#include "../util/ObjLoader.h"
#include <string>

Player::Player()
{
	position = {0, 21, -0.5};
	maxSpeed = 10;
	targetX = 0;
	isDead = false;
}

void Player::spawn()
{

}

void Player::update(float deltaTime)
{
	if (isDead)
		return;

	float travel = deltaTime * maxSpeed;

	if (abs(position.x - targetX) < travel)
		position.x = targetX;
	else if (position.x < targetX)
		position.x += travel;
	else
		position.x -= travel;

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
