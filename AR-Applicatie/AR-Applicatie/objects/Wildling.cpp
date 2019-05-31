#include "Wildling.h"
#include "../util/ObjLoader.h"
#include "../components/AnimationComponent.h"
#include "../data/DataManager.h"
#include "../states/GameState.h"

Wildling::Wildling(Player *player, std::vector<Wildling *> *wildlings, const float x)
{
	this->player = player;
	this->wildlings = wildlings;

	position.x = x;
	position.y = -8;
	position.z = 1;
	velocity.y = 1;
	currentAction = Action::CLIMB;
}

void Wildling::update(const float deltaTime)
{
	GameObject::update(deltaTime);

	//Falling
	if (currentAction == Action::FALLING)
	{
		velocity.y -= deltaTime * GRAVITY;
		position.y += deltaTime * velocity.y;
	}
	//Climbing
	else if (currentAction == Action::CLIMB)
	{
		climb(deltaTime);
	}
	//Pulling up
	else if (currentAction == Action::PULL_UP)
	{
		pullUp(deltaTime);
	}
	//Move left or right
	else if (currentAction == Action::RUNLEFT || currentAction == Action::RUNRIGHT)
	{
		run(deltaTime);
	}
	//Attacking
	else if (currentAction == Action::ATTACK)
	{
		attack(deltaTime);
	}
}

bool Wildling::isHit(const float xProjectile, const float yProjectile)
{
	if (abs(xProjectile - position.x) < 2 && abs(yProjectile - position.y - 6) < 2)
	{
		die();
		return true;
	}
	return false;
}

void Wildling::die()
{
	currentAction = Action::FALLING;
	getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::FALL);
}

bool Wildling::canBeDestroyed() const
{
	return position.y < -10;
}

void Wildling::climb(const float deltaTime)
{
	position.y += deltaTime * velocity.y;

	if (position.y >= 12.5)
	{
		// Check for wildling above
		for (auto && other : *wildlings)
		{
			if (other != this && other->position.y > 14.5 && abs(other->position.x - position.x) < 2)
			{
				position.y -= deltaTime * velocity.y;

				//Make them use no animation until the way is clear
				getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::PAUSE);

				return;
			}
		}

		currentAction = Action::PULL_UP;
		getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::PULL_UP);
	}
}

void Wildling::pullUp(const float deltaTime)
{
	//Do not go further until the pull-up animation is finished
	if (getComponent<AnimationComponent>()->getCurrentAnimation() == AnimationComponent::Animation::PULL_UP)
		return;

	position.y = 17.5;

	if (position.x > player->getPosition().x)
	{
		currentAction = Action::RUNLEFT;
		getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::RUN_LEFT);
		velocity.x = -2.0f;
	}
	else
	{
		currentAction = Action::RUNRIGHT;
		getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::RUN_RIGHT);
		velocity.x = 2.0f;
	}
	//Putting the goblin on the wall
	position.z = -0.5f;
}

void Wildling::run(const float deltaTime)
{
	//If the player is falling, all goblins start to cheer when they are on the wall
	if (player->getCurrentAction() == Player::Action::FALLING)
	{
		currentAction = Action::CHEER;
		getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::CHEER);
	}
	else
	{
		position.x += deltaTime * velocity.x;

		//When near the player, attack
		if (abs(position.x - player->getPosition().x) < 3.0f)
		{
			currentAction = Action::ATTACK;
			DataManager::getInstance().soundManager.playSound(SoundManager::Sound::DEATH, false);

			if (position.x > player->getPosition().x)
				getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::ATTACK_LEFT);
			else
				getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::ATTACK_RIGHT);
		}
	}
}

void Wildling::attack(const float deltaTime)
{
	//If the second half of the attack animation is initiated, kill the player
	if (getComponent<AnimationComponent>()->getAniDirection())
	{
		player->setCurrentAction(Player::Action::FALLING);
		player->getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::FALL);
		//Makes sure the player is in front of the wall, so the falling looks natural
		player->setPosition({ player->getPosition().x, player->getPosition().y, 1 });
	}

	//If the attack animation has finished, make the player fall down
	if (getComponent<AnimationComponent>()->getCurrentAnimation() == AnimationComponent::Animation::CHEER)
	{
		currentAction = Action::CHEER;
		DataManager::getInstance().soundManager.stopSounds();
		DataManager::getInstance().soundManager.setVolume(1.0);
		DataManager::getInstance().soundManager.playSound(SoundManager::Sound::LOSS, false);
		GameState *state = dynamic_cast<GameState*>(&DataManager::getInstance().stateHandler.getState());
		state->gameLogic.isOver = true;
		DataManager::getInstance().stateHandler.setState(StateHandler::States::DEATH);
	}
}
