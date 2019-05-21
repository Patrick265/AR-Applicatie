#include "Wildling.h"
#include "../util/ObjLoader.h"
#include <string>

Wildling::Wildling(float x)
{
	position.x = x;
	speed = 1;
	isDead = false;
}

void Wildling::spawn()
{
	
}

void Wildling::update(float deltaTime)
{
	if (isDead)
		speed -= deltaTime * 10;

	position.y += deltaTime * speed;

	if (position.y > 20)
		position.y = 0;
}

bool Wildling::isHit(float xProjectile, float yProjectile)
{
	if (abs(xProjectile - position.x) < 2 && abs(yProjectile - position.y) < 2)
	{
		isDead = true;
		return true;
	}
	return false;
}

bool Wildling::canBeDestroyed()
{
	return position.y < -10;
}
