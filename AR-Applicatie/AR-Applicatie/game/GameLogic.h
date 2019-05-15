#pragma once
#include "../objects/GameObject.h"
#include "../objects/Wildling.h"
#include "../objects/Projectile.h"
#include <vector>
#include "../objects/Player.h"

class GameLogic
{
private:
	GameObject *wall;
	Player* player;
	std::vector<Projectile *> projectiles;
	std::vector<Wildling *> wildlings;

	void handleMouse();

public:
	GameLogic();
	~GameLogic();

	void start();
	void update(float deltaTime);
	void throwProjectile();
	std::vector<GameObject *> getGameObjects();
};
