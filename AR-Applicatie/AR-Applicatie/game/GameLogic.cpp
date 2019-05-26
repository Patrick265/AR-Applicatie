#include "GameLogic.h"
#include "../util/ObjLoader.h"
#include <queue>
#include "../components/StaticComponent.h"
#include "../components/AnimationComponent.h"
#include "../data/DataManager.h"

float counter = 0;

std::queue<Point2D> mouseHistory;
const auto height = DataManager::getInstance().height;
int y_trigger_distance = height / 4;
bool canThrow = false;

GameLogic::GameLogic()
{
	wall = new GameObject();
	wall->addComponent(new StaticComponent("wall", "wall"));
	wall->setPosition({ 0, -9, 0 });
	wall->setScale({ 0.5, 0.5, 0.5 });

	wallTop = new GameObject();
	wallTop->addComponent(new StaticComponent("wall_top", "wall_top"));
	wallTop->setPosition({ 0, -9, 0 });
	wallTop->setScale({ 0.5, 0.5, 0.5 });

	skybox = new GameObject();
	skybox->addComponent(new StaticComponent("skybox", "skybox"));
	skybox->setPosition({ 0,0,0 });
	skybox->setScale({ 1,1,1 });

	player = new Player();
	//player->addComponent(new StaticComponent("cube", "none"));
	player->addComponent(new AnimationComponent(Rig("elf", Math::vec3d{ 0,0,0 }, Math::vec3d{ 1.0,1.0,1.0 })));
	player->getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::ATTACK);
	player->setPosition(Math::vec3d{ 10,20.2,-2 });

}

GameLogic::~GameLogic()
{
	delete wall;
	delete player;
	for (auto &&p : projectiles)
		delete p;
	for (auto &&p : wildlings)
		delete p;
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
		Wildling *wildling = new Wildling(rand() % 20 - 10);
		wildling->addComponent(new AnimationComponent(Rig("goblin", Math::vec3d{ 0,0,0 }, Math::vec3d{ 0.5,0.5,0.5 })));//new StaticComponent("giant", "giant"));
		wildling->getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::CLIMB);

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

	// Update components
	for (auto o : getGameObjects())
		for (auto c : o->getComponents())
			c->update(deltaTime);
}

void GameLogic::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	for (auto o : getGameObjects())
		for (auto c : o->getComponents())
			c->draw(meshes, textures);
}

void GameLogic::throwProjectile(float xVelocity, float yVelocity)
{
	Projectile *p = new Projectile(player->getPosition().x, xVelocity, yVelocity);
	p->addComponent(new StaticComponent(DataManager::getInstance().currentWeapon, DataManager::getInstance().currentWeapon));
	p->setScale(Math::vec3d{ 1.0,1.0,1.0 });
	projectiles.push_back(p);
}

std::vector<GameObject *> GameLogic::getGameObjects()
{
	std::vector<GameObject *> gameObjects;
	gameObjects.clear();
	gameObjects.push_back(wall);
	gameObjects.push_back(wallTop);
	gameObjects.push_back(player);
	gameObjects.push_back(skybox);
	for (Wildling* wildling : wildlings)
		gameObjects.push_back(wildling);
	for (Projectile* projectile : projectiles)
		gameObjects.push_back(projectile);
	return gameObjects;
}

void GameLogic::handleMouse()
{
	// Get mousePos and screen width
	const auto mousePos = DataManager::getInstance().mousePos;
	const auto width = DataManager::getInstance().width;

	// Move player to mouse X position
	player->targetX = mousePos.x / float(width) * 20.0f - 10.0f;

	// Throw the projectiles
	mouseHistory.push(mousePos);
	if (mouseHistory.size() < 5)
		return;
	if (mouseHistory.size() > 5)
		mouseHistory.pop();

	const auto first = mouseHistory.front();
	const auto last = mouseHistory.back();


	//If the player is not current attacking, ignore the rest
	if (player->getCurrentAction() != Player::Action::ATTACK)
		return;

	//If a weapon is not ready
	if (!canThrow)
	{
		//If the mouse is within the trigger area at the top
		if (mousePos.y - y_trigger_distance <= 0.0f) {
			//Throw
			canThrow = true;
		}
	}
	//If a weapon has been drawn
	if (canThrow)
	{
		//If the mouse is within the trigger area at the bottom
		if (mousePos.y + y_trigger_distance >= height)
		{
			throwProjectile((last.x - first.x) * 0.1, (first.y - last.y) * 0.1);
			canThrow = false;
			DataManager::getInstance().determineNextWeapon();
		}
	}
}
