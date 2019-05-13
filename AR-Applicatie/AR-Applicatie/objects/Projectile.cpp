#include "Projectile.h"
#include "../util/ObjLoader.h"
#include <string>
#include <iostream>

Projectile::Projectile(std::string objectName, int textureId, float x, float xVelocity, float yVelocity)
	: GameObject(ObjLoader::loadObj(objectName), textureId)
{
	position = {x, 20, 1};
	velocity = {xVelocity, yVelocity, 0};
	gravity = 10;
	isActive = true;
}

void Projectile::update(float deltaTime)
{
	velocity.y -= deltaTime * gravity;

	position.x += deltaTime * velocity.x;
	position.y += deltaTime * velocity.y;
	position.z += deltaTime * velocity.z;

	// rotation = {rotation.x + 1, rotation.y + 1, rotation.z + 1};
}

void Projectile::hasHit()
{
	velocity.y = 5;
	velocity.z = 10;
	isActive = false;
}

bool Projectile::canBeDestroyed()
{
	return position.y < 0;
}
