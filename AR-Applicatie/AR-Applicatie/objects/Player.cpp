#include "Player.h"
#include "../util/ObjLoader.h"
#include <string>

extern int cursorOffsetX, cursorOffsetY;
bool moveRight;

Player::Player(std::string objectName, int textureId)
	: GameObject(ObjLoader::loadObj(objectName), textureId)
{
	position = {0, 21, -0.5};
	maxSpeed = 10;
	isDead = false;
}

void Player::spawn()
{

}

void Player::update(float deltaTime)
{
	if (isDead)
		return;

	if (!moveRight && position.x > 10 || moveRight && position.x < -10)
		moveRight = !moveRight;
	if (moveRight)
		position.x -= deltaTime * maxSpeed;
	else
		position.x += deltaTime * maxSpeed;
}

void Player::kill()
{
	// TODO: Add kill logic
}
