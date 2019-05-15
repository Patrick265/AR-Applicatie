#include "GameLogic.h"
#include "../util/ObjLoader.h"
#include <iostream>
#include "../util/TextureHandler.h"
#include "../vision/markerdetection.h"

extern Point2D mousePos;
extern float width;

int counter = 0;

GameLogic::GameLogic()
{
	wall = new GameObject(ObjLoader::loadObj("Resources/Cube/cube.obj"), -1);
	wall->setPosition({0, 10, -1});
	wall->setScale({20, 20, 1});

	player = new Player("Resources/Cube/cube.obj", -1);
}

GameLogic::~GameLogic()
{
	// TODO: Free stuff
}

void GameLogic::start()
{
	
}

void GameLogic::update(float deltaTime)
{
	// Mouse logic
	handleMouse();

	// Destroy all objects that need to be destroyed
	for (int i = 0; i < int(projectiles.size()); i++)
		if (projectiles[i]->canBeDestroyed())
			projectiles.erase(projectiles.begin() + i--);

	for (int i = 0; i < int(wildlings.size()); i++)
		if (wildlings[i]->canBeDestroyed())
			wildlings.erase(wildlings.begin() + i--);

	// Add wildling if there are too few
	if (counter++ > 100 && wildlings.size() < 5)
	{
		Wildling *wildling = new Wildling("Resources/Rune/giant.obj",
			TextureHandler::addTexture("Resources/Rune/giant.png"), rand() % 20 - 10);
		wildlings.push_back(wildling);
		counter = 0;
	}

	// Update player
	player->update(deltaTime);

	// Update all wildlings
	for (Wildling* wildling : wildlings)
		wildling->update(deltaTime);

	// Update all projectiles
	for (Projectile* projectile : projectiles)
		projectile->update(deltaTime);

	// Check collision
	for (Wildling* wildling : wildlings)
		for (Projectile* projectile : projectiles)
			if (projectile->isActive && wildling->isHit(projectile->getPosition().x, projectile->getPosition().y))
				projectile->hasHit();
}

void GameLogic::throwProjectile()
{
	projectiles.push_back(new Projectile("Resources/Pakketje/Pakketje.obj", 
		TextureHandler::addTexture("Resources/Pakketje/Pakketje.png"),
		player->getPosition().x, rand() % 10 - 5, -1));
}

std::vector<GameObject *> GameLogic::getGameObjects()
{
	std::vector<GameObject *> gameObjects;
	gameObjects.clear();
	gameObjects.push_back(wall);
	gameObjects.push_back(player);
	for (Wildling* wildling : wildlings)
		gameObjects.push_back(wildling);
	for (Projectile* projectile : projectiles)
		gameObjects.push_back(projectile);
	return gameObjects;
}

// Move player to mouse X position
void GameLogic::handleMouse()
{
	player->targetX = ((mousePos.x / float(width)) * 20.0f) - 10.0f;
}
