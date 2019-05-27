#include "GameLogic.h"
#include "../util/ObjLoader.h"
#include <queue>
#include "../components/StaticComponent.h"
#include "../components/AnimationComponent.h"
#include "../data/DataManager.h"

std::queue<Point2D> mouseHistory;
const auto height = DataManager::getInstance().height;
int y_trigger_distance = height / 4;
bool canThrow = false;

float counter = 0;
float spawnRate = 0;

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
	skybox->setPosition({-160,0,0});

	player = new Player();
	player->addComponent(new AnimationComponent(Rig("elf", Math::vec3d{ 0,0,0 }, Math::vec3d{ 1.0,1.0,1.0 })));
	player->getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::ATTACK_MOUSE);
	player->setPosition(Math::vec3d{ wallWidth / 2.0f,20.2,-2 });

	gameDuration = std::chrono::duration<float, std::milli>(1 * 60 * 1000);
	elapsedTime = std::chrono::duration<float, std::milli>(0);
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
	// Add elapsedTime
	elapsedTime += std::chrono::milliseconds(static_cast<int>(deltaTime * 1000.0f));
	spawnRate = std::min(elapsedTime.count() / 1000.0f, 10.0f);

	// Check if game has been won
	if (elapsedTime >= gameDuration)
	{
		// Game has been won!
		// TODO: Win logic
		// exit(1);
	}

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
	if (counter > (15.0f - spawnRate) * 0.1f)
	{
		auto xPos = 0;
		auto exit = false;
		for (auto i = 0; i < 10; ++i)
		{
			xPos = rand() % wallWidth - wallWidth / 2.0f;
			exit = false;
			for (auto && wildling : wildlings)
			{
				if (wildling->getPosition().y < 10 && abs(wildling->getPosition().x - xPos) < 2)
				{
					exit = true;
					break;
				}
			}
			if (!exit)
				break;
		}

		auto wildling = new Wildling(player, &wildlings, xPos);
		wildling->addComponent(new AnimationComponent(Rig("goblin",
			Math::vec3d{ 0,0,0 }, Math::vec3d{ 0.5,0.5,0.5 })));
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
	auto xPos = mousePos.x / DataManager::getInstance().width;
	const auto padding = 0.2f;

	// Move player to mouse X position
	if (xPos < padding)
		xPos = padding;
	if (xPos > 1.0f - padding)
		xPos = 1.0f - padding;

	xPos = (xPos - padding) * (1.0f / (1.0f - padding * 2.0f));
	player->targetX = xPos * wallWidth - wallWidth / 2.0f;

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
