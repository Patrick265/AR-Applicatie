#include "GameLogic.h"
#include "../util/ObjLoader.h"
#include "../util/TextureHandler.h"
#include "../vision/markerdetection.h"
#include <queue>

extern markerdetection::Point2D mousePos;
extern float width;

float counter = 0;

std::queue<markerdetection::Point2D> mouseHistory;
const int Y_TRIGGER_DISTANCE = 40;
bool canThrow = true;

GameLogic::GameLogic()
{
	wall = new GameObject("cube", "none");
	wall->setPosition({0, 10, -1});
	wall->setScale({20, 20, 1});

	player = new Player("cube", "none");
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
	counter += deltaTime;
	if (counter > 0.5 && wildlings.size() < 5)
	{
		Wildling *wildling = new Wildling("giant",
			"giant", rand() % 20 - 10);
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

void GameLogic::throwProjectile(float xVelocity, float yVelocity)
{
	projectiles.push_back(new Projectile("packet", 
		"packet",
		player->getPosition().x, xVelocity, yVelocity));
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

void GameLogic::handleMouse()
{
	// Move player to mouse X position
	player->targetX = mousePos.x / float(width) * 20.0f - 10.0f;

	// Throw the projectiles
	mouseHistory.push(mousePos);
	if (mouseHistory.size() < 5)
		return;
	if (mouseHistory.size() > 5)
		mouseHistory.pop();

	markerdetection::Point2D first = mouseHistory.front();
	markerdetection::Point2D last = mouseHistory.back();

	// If downwards movement is large enough, throw a projectiles
	if (canThrow && first.y - last.y < -Y_TRIGGER_DISTANCE)
	{
		canThrow = false;
		throwProjectile((last.x - first.x) * 0.1, (first.y - last.y) * 0.1);
	}
	else if (first.y - last.y > Y_TRIGGER_DISTANCE)
	{
		canThrow = true;
	}
}
