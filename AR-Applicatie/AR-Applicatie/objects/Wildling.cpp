#include "Wildling.h"
#include "../util/ObjLoader.h"
#include "../components/AnimationComponent.h"

Wildling::Wildling(Player *player, std::vector<Wildling *> *wildlings, float x)
{
	this->player = player;
	this->wildlings = wildlings;

	position.x = x;
	position.y = 7; //--8
	position.z = 1;
	velocity.y = 1;
	currentAction = Action::CLIMB;
}

void Wildling::spawn()
{

}

void Wildling::update(float deltaTime)
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
		position.y += deltaTime * velocity.y;

		if (position.y >= 12.5) 
		{
			// Check for wildling above
			for (auto && other : *wildlings)
			{
				if (other != this && other->position.y > 14.5 && abs(other->position.x - position.x) < 2)
				{
					position.y -= deltaTime * velocity.y;
					return;
				}
			}

			currentAction = Action::PULL_UP;
			getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::PULL_UP);
					
		}
	}
	//Pulling up
	else if (currentAction == Action::PULL_UP) 
	{
		//Do not go further until the pull-up animation is finished
		if (getComponent<AnimationComponent>()->getCurrentAnimation() == AnimationComponent::Animation::PULL_UP)
			return;

		position.y = 17.5;

		if (position.x > player->getPosition().x)
		{
			currentAction = Action::RUNLEFT;
			getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::RUN_LEFT);
			velocity.x = -1.0f;
		}
		else
		{
			currentAction = Action::RUNRIGHT;
			getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::RUN_RIGHT);
			velocity.x = 1.0f;
		}
		//Putting the goblin on the wall
		position.z = -0.5f;
	}
	//Move left or right
	else if (currentAction == Action::RUNLEFT || currentAction == Action::RUNRIGHT)
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

				if (position.x > player->getPosition().x)
					getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::ATTACK_LEFT);
				else
					getComponent<AnimationComponent>()->setAnimation(AnimationComponent::Animation::ATTACK_RIGHT);

				player->setCurrentAction(Player::Action::FALLING);
				player->setPosition({ player->getPosition().x, player->getPosition().y, 1 });
			}
		}
	}

}


bool Wildling::isHit(float xProjectile, float yProjectile)
{
	if (abs(xProjectile - position.x) < 2 && abs(yProjectile - position.y - 6) < 2)
	{
		currentAction = Action::FALLING;
		return true;
	}
	return false;
}

bool Wildling::canBeDestroyed()
{
	return position.y < -10;
}
