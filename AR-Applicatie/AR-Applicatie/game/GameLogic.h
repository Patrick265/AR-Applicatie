#pragma once

#include "../objects/GameObject.h"
#include "../objects/Wildling.h"
#include "../objects/Projectile.h"
#include <vector>
#include "../objects/Player.h"
#include <map>
#include <chrono>

class GameLogic
{
	GameObject *wall;
	GameObject *wallTop;
	const int wallWidth = 26;
	GameObject *skyBox;
	Player* player;
	std::vector<Projectile *> projectiles;
	std::vector<Wildling *> wildlings;

	std::chrono::duration<float, std::milli> gameDuration;
	std::chrono::duration<float, std::milli> elapsedTime;

	void handleMouse();

public:
	bool isOver;

	GameLogic();
	~GameLogic();

	void update(float deltaTime);
	void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures);

	void throwProjectile(float xVelocity, float yVelocity);
	std::vector<GameObject *> getGameObjects();
};
