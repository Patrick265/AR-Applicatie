#include "GameLogic.h"
#include "../util/ObjLoader.h"
#include <queue>
#include "../components/StaticComponent.h"
#include "../components/AnimationComponent.h"
#include "../data/DataManager.h"
#include <GL/freeglut_std.h>

int sHeight;
int yTriggerDistance;

std::queue<markerdetection::Point2D> mouseHistory;
bool canThrow = false;

float counter = 0;
float spawnRate = 0;
int gameScore = 0;
int highScore;

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

	skyBox = new GameObject();
	skyBox->addComponent(new StaticComponent("skybox", "skybox"));
	skyBox->setPosition({ 0,0,0 });
	skyBox->setScale({ 1,1,1 });
	skyBox->setPosition({ -160,0,0 });

	player = new Player();
	player->addComponent(new AnimationComponent(Rig("elf", Math::vec3d{ 0,0,0 }, Math::vec3d{ 1.0,1.0,1.0 })));
	player->getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::ATTACK_MOUSE);
	player->setPosition(Math::vec3d{ wallWidth / 2.0f,20.2f,-2.0f });

	gameDuration = std::chrono::duration<float, std::milli>(1 * 60 * 1000);
	elapsedTime = std::chrono::duration<float, std::milli>(0);

	isOver = false;

	highScore = DataManager::getInstance().retrieveHighscore();
	gameScore = 0;
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

void GameLogic::update(const float deltaTime)
{
	// Add elapsedTime
	elapsedTime += std::chrono::milliseconds(static_cast<int>(deltaTime * 1000.0f));
	spawnRate = std::min(elapsedTime.count() / 5000.0f, 10.0f);

	// Check if game has been won
	if (!isOver && elapsedTime >= gameDuration)
	{
		isOver = true;
		for (auto && wildling : wildlings)
			wildling->die();

		if (gameScore > highScore)
		{
			DataManager::getInstance().writeHighscore(gameScore);
			highScore = gameScore;
			DataManager::getInstance().newHighScore = true;
		}

		DataManager::getInstance().soundManager.stopSounds();
		DataManager::getInstance().soundManager.setVolume(0.5);
		DataManager::getInstance().soundManager.playSound(SoundManager::Sound::WIN, false);
		DataManager::getInstance().stateHandler.setState(StateHandler::States::WIN);
	}

	// Mouse logic
	handleMouse();

	// Destroy all objects that need to be destroyed
	for (auto i = 0; i < int(projectiles.size()); i++)
		if (projectiles[i]->canBeDestroyed())
			projectiles.erase(projectiles.begin() + i--);

	for (auto i = 0; i < int(wildlings.size()); i++)
		if (wildlings[i]->canBeDestroyed())
			wildlings.erase(wildlings.begin() + i--);

	// Add wildling if there are too few
	counter += deltaTime;
	if (!isOver && counter > (13.0f - spawnRate) * 0.5f)
	{
		auto xPos = 0;
		auto exit = false;
		for (auto i = 0; i < 10; ++i)
		{
			xPos = static_cast<int>(rand() % wallWidth - wallWidth / 2.0f);
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

		auto wildling = new Wildling(player, &wildlings, static_cast<float>(xPos));
		wildling->addComponent(new AnimationComponent(Rig("goblin",
			Math::vec3d{ 0,0,0 }, Math::vec3d{ 0.5,0.5,0.5 })));
		wildling->getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::CLIMB);

		wildlings.push_back(wildling);
		counter = 0;
	}

	// Update player
	player->update(deltaTime);

	// Update all wildlings
	for (auto wildling : wildlings)
		wildling->update(deltaTime);

	// Update all projectiles
	for (auto projectile : projectiles)
		projectile->update(deltaTime);

	// Check collision
	for (auto wildling : wildlings)
		for (auto projectile : projectiles)
			if (projectile->isActive && wildling->isHit(projectile->getPosition().x, projectile->getPosition().y))
			{
				projectile->hasHit();
				DataManager::getInstance().soundManager.playSound(SoundManager::Sound::DEATH, false);
				gameScore += 10;
			}

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

	if (isOver)
		return;

	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(gameDuration - elapsedTime);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(gameDuration - elapsedTime - minutes);
	DataManager::getInstance().setOrtho();
	DataManager::getInstance().drawDefaultText(DataManager::getInstance().width - 300, 50,
		"Time left : " + std::to_string(minutes.count()) + ":" + std::to_string(seconds.count()), GLUT_STROKE_ROMAN, 0.25, 0.25);
	DataManager::getInstance().drawDefaultText(20, 50,
		"Score   : " + std::to_string(gameScore), GLUT_STROKE_ROMAN, 0.25, 0.25);
	DataManager::getInstance().drawDefaultText(20, 85,
		"Highscore : " + std::to_string(highScore), GLUT_STROKE_ROMAN, 0.25, 0.25);
}

void GameLogic::throwProjectile(const float xVelocity, const float yVelocity)
{
	auto p = new Projectile(player->getPosition().x, xVelocity, yVelocity);
	p->addComponent(new StaticComponent(DataManager::getInstance().currentWeapon, DataManager::getInstance().currentWeapon));

	p->setScale(Math::vec3d{ 1.0,1.0,1.0 });
	projectiles.push_back(p);
	DataManager::getInstance().soundManager.playSound(SoundManager::Sound::THROW, false);
}

std::vector<GameObject *> GameLogic::getGameObjects()
{
	std::vector<GameObject *> gameObjects;
	gameObjects.clear();
	gameObjects.push_back(wall);
	gameObjects.push_back(wallTop);
	gameObjects.push_back(player);
	gameObjects.push_back(skyBox);
	for (auto wildling : wildlings)
		gameObjects.push_back(wildling);
	for (auto projectile : projectiles)
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

	sHeight = DataManager::getInstance().height;
	yTriggerDistance = static_cast<int>(sHeight / 3.0f);

	//If a weapon is not ready
	if (!canThrow)
	{
		//If the mouse is within the trigger area at the top
		if (mousePos.y - yTriggerDistance <= 0.0f) {
			//Throw
			canThrow = true;
		}
	}
	//If a weapon has been drawn
	if (canThrow)
	{
		//If the mouse is within the trigger area at the bottom
		if (mousePos.y + yTriggerDistance >= sHeight)
		{
			throwProjectile((last.x - first.x) * 0.1f, (first.y - last.y) * 0.1f);
			canThrow = false;
			DataManager::getInstance().determineNextWeapon();
		}
	}
}
