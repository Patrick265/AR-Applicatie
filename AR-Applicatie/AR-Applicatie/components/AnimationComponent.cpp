#include "AnimationComponent.h"
#include "../vision/markerdetection.h"
#include <GL/freeglut.h>
#include "../objects/GameObject.h"
#include "../data/DataManager.h"
#include "../game/GameLogic.h"

AnimationComponent::AnimationComponent(const Rig rig)
{
	currentAnimation = Animation::IDLE;
	currentRotation = 0.0f;
	aniForward = true;
	this->rig = rig;
}

AnimationComponent::AnimationComponent(const AnimationComponent &ani)
	: rig(ani.rig), currentRotation(ani.currentRotation),
	aniForward(ani.aniForward), currentAnimation(ani.currentAnimation)
{
}

void AnimationComponent::update(const float elapsedTime)
{
	switch (currentAnimation)
	{
	case Animation::RUN_LEFT:
		runLeft(elapsedTime);
		break;
	case Animation::RUN_RIGHT:
		runRight(elapsedTime);
		break;
	case Animation::IDLE:
		idle(elapsedTime);
		break;
	case Animation::ATTACK_MOUSE:
		attackMouse(elapsedTime);
		break;
	case Animation::ATTACK_LEFT:
		attackLeft(elapsedTime);
		break;
	case Animation::ATTACK_RIGHT:
		attackRight(elapsedTime);
		break;
	case Animation::CLIMB:
		climb(elapsedTime);
		break;
	case Animation::FALL:
		fall(elapsedTime);
		break;
	case Animation::PULL_UP:
		pullUp(elapsedTime);
		break;
	case Animation::CHEER:
		cheer(elapsedTime);
		break;
	case Animation::PAUSE:
		pause(elapsedTime);
		break;
	}
}

void AnimationComponent::draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures)
{
	glPushMatrix();

	glTranslatef(gameObject->getPosition().x, gameObject->getPosition().y, gameObject->getPosition().z);
	glRotatef(gameObject->getRotation().x, 1, 0, 0);
	glRotatef(gameObject->getRotation().y, 0, 1, 0);
	glRotatef(gameObject->getRotation().z, 0, 0, 1);
	glScalef(gameObject->getScale().x, gameObject->getScale().y, gameObject->getScale().z);

	rig.drawRig(meshes, textures);

	glPopMatrix();
}

void AnimationComponent::setAnimation(const Animation &animation)
{
	//To prevent past animations from affecting the new one
	rig.setRotation({ 0,0,0 });

	//Idem
	rig.getNode("torso").setRotation({ 0,0,0 });

	//Starting the animation over
	currentRotation = 0.0f;

	//Making sure the animation starts off going forward
	aniForward = true;

	currentAnimation = animation;
}

void AnimationComponent::positionToRotation(const int y)
{
	const auto height = DataManager::getInstance().height;

	//Ensures the rotation is 160 at the top trigger area of the screen, and 0 at the bottom trigger area of the screen
	const auto rotationPerPixel = ATTACK_MAX_ROTATION / (height - yTriggerDistance * 2);

	//Inverse, because 0 == 160 degrees rotation
	// -y_trigger_distance, because the rotation has to end when entering the trigger area
	currentRotation = ATTACK_MAX_ROTATION - (y - yTriggerDistance) * rotationPerPixel;
}

void AnimationComponent::setTriggerDistance(int yTriggerDistance)
{
	this->yTriggerDistance = yTriggerDistance;
}

void AnimationComponent::run(const float elapsedTime)
{
	if (aniForward)
		currentRotation += 150.0f * elapsedTime;
	else
		currentRotation -= 150.0f * elapsedTime;

	if (currentRotation <= 0.0f)
		aniForward = true;
	else if (currentRotation >= 90.0f)
		aniForward = false;

	rig.getNode("la_u").setRotation({ -currentRotation + 45, 0, 0 });
	rig.getNode("la_l").setRotation({ -currentRotation,0, 0 });
	rig.getNode("ra_u").setRotation({ -90 + currentRotation + 45,0, 0 });
	rig.getNode("ra_l").setRotation({ -90 + currentRotation,0, 0 });

	rig.getNode("ll_u").setRotation({ currentRotation - 45, 0, 0 });
	rig.getNode("ll_l").setRotation({ currentRotation,0, 0 });
	rig.getNode("rl_u").setRotation({ 90 - currentRotation - 45,0, 0 });
	rig.getNode("rl_l").setRotation({ 90 - currentRotation,0, 0 });
}

void AnimationComponent::runLeft(const float elapsedTime)
{
	run(elapsedTime);
	rig.setRotation({ 0,-90,0 });
}

void AnimationComponent::runRight(const float elapsedTime)
{
	run(elapsedTime);
	rig.setRotation({ 0,90,0 });
}

void AnimationComponent::idle(const float elapsedTime)
{
	if (aniForward)
		currentRotation += 5.0f * elapsedTime;
	else
		currentRotation -= 5.0f * elapsedTime;

	if (currentRotation <= 0.0f)
		aniForward = true;
	else if (currentRotation >= 5.0f)
		aniForward = false;

	auto idleRotation = currentRotation * 0.003f;

	//rig.getNode("torso").setPosition({ 0,-idle_rotation, 0 });

	rig.getNode("la_u").setRotation({ -currentRotation * 0.5f, 0, 0 });
	rig.getNode("la_l").setRotation({ -currentRotation,0, 0 });
	rig.getNode("ra_u").setRotation({ -10 + currentRotation * 0.5f,0, 0 });
	rig.getNode("ra_l").setRotation({ -10 + currentRotation,0, 0 });

	rig.getNode("ll_u").setRotation({ -currentRotation, 0, 0 });
	rig.getNode("ll_l").setRotation({ currentRotation,0, 0 });
	rig.getNode("rl_u").setRotation({ -10 + currentRotation,0, 0 });
	rig.getNode("rl_l").setRotation({ 10 - currentRotation,0, 0 });
}

void AnimationComponent::attackMouse(const float elapsedTime)
{
	// Get mousePos and screen height
	const auto mousePos = DataManager::getInstance().mousePos;
	const auto height = DataManager::getInstance().height;

	positionToRotation(static_cast<int>(mousePos.y));

	if (currentRotation <= 0.0f)
	{
		rig.getNode("la_weapon").setMesh("none");
		rig.getNode("la_weapon").setTexture("none");
		currentRotation = 0.0f;
	}

	else if (currentRotation >= ATTACK_MAX_ROTATION)
	{
		currentRotation = ATTACK_MAX_ROTATION;
		rig.getNode("la_weapon").setMesh(DataManager::getInstance().currentWeapon);
		rig.getNode("la_weapon").setTexture(DataManager::getInstance().currentWeapon);
		const Math::vec3d armLeft = { 0.61229f , 0.068894f ,1.45316f };
		Math::vec3d armRight = { -0.605497f, 0.112396f, 1.48285f };

		rig.getNode("la_weapon").setPosition(convertCoordinates(DataManager::getInstance().weaponMap[DataManager::getInstance().currentWeapon], armLeft));
	}

	rig.getNode("la_u").setRotation({ -currentRotation,0, 0 });
	rig.getNode("la_l").setRotation({ -currentRotation,0, 0 });
	rig.getNode("ra_u").setRotation({ -currentRotation,0, 0 });
	rig.getNode("ra_l").setRotation({ -currentRotation,0, 0 });

	const auto legRotation = currentRotation / 30.0f;

	rig.getNode("torso").setPosition({ 0,legRotation*0.01f, 0 });

	rig.getNode("ll_u").setRotation({ -20,0, 0 });
	rig.getNode("ll_l").setRotation({ legRotation + 10,0, 0 });
	rig.getNode("rl_u").setRotation({ legRotation - 10,0, 0 });
	rig.getNode("rl_l").setRotation({ legRotation,0, 0 });
}

void AnimationComponent::climb(const float elapsedTime)
{
	rig.setRotation({ 0,180,0 });

	if (aniForward)
		currentRotation += 150.0f * elapsedTime;
	else
		currentRotation -= 150.0f * elapsedTime;

	if (currentRotation <= 0.0f)
		aniForward = true;
	else if (currentRotation >= 90.0f)
		aniForward = false;

	rig.getNode("torso").setRotation({ -30,0,2.5f - currentRotation / 20 });

	rig.getNode("la_u").setRotation({ -currentRotation - 30, 0, 0 });
	rig.getNode("la_l").setRotation({ currentRotation / 5 - 100 ,0, 0 });
	rig.getNode("ra_u").setRotation({ -90 + currentRotation ,0, 0 });
	rig.getNode("ra_l").setRotation({ currentRotation / 5 - 100  ,0, 0 });

	rig.getNode("ll_u").setRotation({ -currentRotation, 0, 0 });
	rig.getNode("ll_l").setRotation({ currentRotation, 0, 0 });
	rig.getNode("rl_u").setRotation({ -90 + currentRotation,0, 0 });
	rig.getNode("rl_l").setRotation({ 90 - currentRotation,0, 0 });
}

void AnimationComponent::attack(const float elapsedTime)
{
	//Given the goblin the elf model to throw down
	if (rig.getNode("la_weapon").getMesh() == "none")
	{
		currentRotation = 100;
		rig.getNode("la_weapon").setMesh("icicle");
		rig.getNode("la_weapon").setTexture("icicle");
	}

	if (currentRotation < 0.0f)
		aniForward = true;
	else if (currentRotation >= 100.0f)
		aniForward = false;

	if (aniForward)
		currentRotation += 100.0f * elapsedTime;
	else
		currentRotation -= 200.0f * elapsedTime;

	//After one rotation of the animation, switch to the cheer animation
	if (currentRotation < 0.0f)
	{
	}
	else if (currentRotation >= 100)
	{
		setAnimation(Animation::CHEER);
		currentRotation = ATTACK_MAX_ROTATION;
	}

	rig.getNode("la_u").setRotation({ -currentRotation,0, 0 });
	rig.getNode("la_l").setRotation({ -currentRotation,0, 0 });
	rig.getNode("ra_u").setRotation({ -currentRotation,0, 0 });
	rig.getNode("ra_l").setRotation({ -currentRotation,0, 0 });

	const auto legRotation = currentRotation / 30.0f;

	rig.getNode("torso").setPosition({ 0,5.5f + legRotation * 0.01f, 0 });

	rig.getNode("ll_u").setRotation({ -20,0, 0 });
	rig.getNode("ll_l").setRotation({ legRotation + 10,0, 0 });
	rig.getNode("rl_u").setRotation({ legRotation - 10,0, 0 });
	rig.getNode("rl_l").setRotation({ legRotation,0, 0 });
}

void AnimationComponent::attackLeft(const float elapsedTime)
{
	rig.setRotation({ 0,-90,0 });
	attack(elapsedTime);
}

void AnimationComponent::attackRight(const float elapsedTime)
{
	rig.setRotation({ 0,90,0 });
	attack(elapsedTime);
}

void AnimationComponent::pullUp(const float elapsedTime)
{
	//Making sure the goblin stops wielding icepicks after the climb is done
	if (rig.getNode("la_weapon").getMesh() != "none")
	{
		rig.getNode("la_weapon").setMesh("none");
		rig.getNode("la_weapon").setTexture("none");

		rig.getNode("ra_weapon").setMesh("none");
		rig.getNode("ra_weapon").setTexture("none");
	}

	rig.setRotation({ 0,180,0 });

	currentRotation += 80.0f * elapsedTime;

	//After one rotation of the animation, switch to the cheer animation
	if (currentRotation < 0.0f)
	{
		aniForward = true;
		currentRotation = 0.0f;
	}
	else if (currentRotation > 140 && currentRotation < 280)
	{
		aniForward = false;
	}
	else if (currentRotation > 280)
	{
		setAnimation(Animation::IDLE);
	}

	//First part of the pull-up
	if (aniForward)
	{
		rig.getNode("la_u").setRotation({ currentRotation - 90, 0, 0 });
		rig.getNode("la_l").setRotation({ currentRotation / 2 - 90, 0, 0 });
		rig.getNode("ra_u").setRotation({ currentRotation - 90 ,0, 0 });
		rig.getNode("ra_l").setRotation({ currentRotation / 2 - 90, 0, 0 });

		auto legRotation = currentRotation / 30.0f;
		rig.getNode("torso").setRotation({ -30 + currentRotation / 5, 0, 0 });

		rig.getNode("torso").setPosition({ 0, 5.5f + currentRotation / 19, currentRotation / 70 });

		rig.getNode("ll_u").setRotation({ -currentRotation, 0, 0 });
		rig.getNode("ll_l").setRotation({ currentRotation, 0, 0 });
		rig.getNode("rl_u").setRotation({ -90 + currentRotation,0, 0 });
		rig.getNode("rl_l").setRotation({ 90 - currentRotation,0, 0 });
	}
	//Second part of the pull-up
	else
	{
		rig.getNode("la_u").setRotation({ (140 - ((currentRotation - 140) / 2)) - 90, 0, 0 });
		rig.getNode("la_l").setRotation({ (140 - ((currentRotation - 140) / 2)) / 2 - 90, 0, 0 });
		rig.getNode("ra_u").setRotation({ (140 - ((currentRotation - 140) / 2)) - 90 ,0, 0 });
		rig.getNode("ra_l").setRotation({ (140 - ((currentRotation - 140) / 2)) / 2 - 90, 0, 0 });

		auto legRotation = currentRotation / 30.0f;

		rig.getNode("torso").setRotation({ -30 + ((140 / (currentRotation)) * 140) / 5, 0, 0 });
		rig.getNode("torso").setPosition({ 0, 5.5f + ((2 - (140 / currentRotation)) * 140) / 19, currentRotation / 70 });

		rig.getNode("ll_u").setRotation({ -140 + (((currentRotation / 140) - 1) * 140), 0, 0 });
		rig.getNode("ll_l").setRotation({ 140 - (((currentRotation / 140) - 1) * 140), 0, 0 });
		rig.getNode("rl_u").setRotation({ -90 + ((140 / (currentRotation)) * 140),0, 0 });
		rig.getNode("rl_l").setRotation({ 90 - ((140 / (currentRotation)) * 140),0, 0 });
	}
}

void AnimationComponent::cheer(const float elapsedTime)
{
	if (aniForward)
		currentRotation += 80.0f * elapsedTime;
	else
		currentRotation -= 80.0f * elapsedTime;

	if (currentRotation <= 0.0f)
		aniForward = true;
	else if (currentRotation >= 100.0f)
		aniForward = false;

	rig.getNode("torso").setRotation({ 0,0,2.5f - currentRotation / 20 });

	rig.getNode("la_u").setRotation({ -currentRotation - 30, 0, currentRotation / 2 });
	rig.getNode("la_l").setRotation({ currentRotation / 5 - 100 ,0, -currentRotation / 2 });
	rig.getNode("ra_u").setRotation({ -90 + currentRotation ,0, -currentRotation / 2 });
	rig.getNode("ra_l").setRotation({ currentRotation / 5 - 100  ,0, currentRotation / 2 });

	rig.getNode("ll_u").setRotation({ -currentRotation, 0, 0 });
	rig.getNode("ll_l").setRotation({ currentRotation, 0, 0 });
	rig.getNode("rl_u").setRotation({ -100 + currentRotation,0, 0 });
	rig.getNode("rl_l").setRotation({ 100 - currentRotation,0, 0 });
}

void AnimationComponent::fall(const float elapsedTime)
{
	if (aniForward)
		currentRotation += 80.0f * elapsedTime;
	else
		currentRotation -= 80.0f * elapsedTime;

	if (currentRotation <= 0.0f)
		aniForward = true;

	rig.getNode("torso").setRotation({ currentRotation,0,2.5f - currentRotation });

	rig.getNode("la_u").setRotation({ -currentRotation * 4 - 90, 0, currentRotation });
	rig.getNode("la_l").setRotation({ currentRotation * 4 / 3 - 100 ,0, -currentRotation });
	rig.getNode("ra_u").setRotation({ -currentRotation * 4 + 90,0, -currentRotation });
	rig.getNode("ra_l").setRotation({ currentRotation * 4 / 3 - 100  ,0, currentRotation });

	rig.getNode("ll_u").setRotation({ -currentRotation * 2 - 90, 0, 0 });
	rig.getNode("ll_l").setRotation({ currentRotation * 2 / 3 - 100 ,0, 0 });
	rig.getNode("rl_u").setRotation({ -currentRotation * 2 + 90,0, 0 });
	rig.getNode("rl_l").setRotation({ currentRotation * 2 / 3 - 100  ,0, 0 });
}

void AnimationComponent::pause(const float elapsedTime)
{
	rig.setRotation({ 0,180,0 });
	rig.getNode("torso").setRotation({ -30,0,0 });
}

Math::vec3d AnimationComponent::convertCoordinates(const Math::vec3d &posCords, const Math::vec3d &parent)
{
	Math::vec3d convertedCords;
	convertedCords.x = posCords.x - parent.x;
	convertedCords.y = posCords.z - parent.z;
	convertedCords.z = -posCords.y - (-parent.y);

	return convertedCords;
}
