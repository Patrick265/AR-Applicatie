#pragma once
#include "../objects/GameObject.h"
#include "../objects/Wildling.h"
#include "../objects/Projectile.h"
#include <vector>
#include "../objects/Player.h"
#include <map>

class GameLogic
{
	GameObject *wall;
	Player* player;
	std::vector<Projectile *> projectiles;
	std::vector<Wildling *> wildlings;

	GameObject *animation;

	void handleMouse();

public:
	GameLogic();
	~GameLogic();

	void start();

	void update(float deltaTime);
	void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures);

	void throwProjectile(float xVelocity, float yVelocity);
	std::vector<GameObject *> getGameObjects();
};
