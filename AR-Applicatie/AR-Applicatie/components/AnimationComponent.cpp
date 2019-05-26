#include "AnimationComponent.h"
#include "../vision/markerdetection.h"
#include <GL/freeglut.h>
#include "../objects/GameObject.h"
#include "../data/DataManager.h"
#include "../game/GameLogic.h"

extern int y_trigger_distance;

AnimationComponent::AnimationComponent(Rig rig)
{
	current_animation = Animation::IDLE;
	current_rotation = 0.0f;
	ani_forward = true;
	this->rig = rig;
}

AnimationComponent::AnimationComponent(const AnimationComponent &ani)
	:rig(ani.rig), current_animation(ani.current_animation),
	current_rotation(ani.current_rotation), ani_forward(ani.ani_forward)
{
}

void AnimationComponent::update(const float elapsedTime)
{
	if (current_animation == Animation::RUN_LEFT)
		runLeft(elapsedTime);
	else if (current_animation == Animation::RUN_RIGHT)
		runRight(elapsedTime);
	else if (current_animation == Animation::IDLE)
		idle(elapsedTime);
	else if (current_animation == Animation::ATTACK_MOUSE)
		attackMouse(elapsedTime);
	else if (current_animation == Animation::ATTACK_LEFT)
		attackLeft(elapsedTime);
	else if (current_animation == Animation::ATTACK_RIGHT)
		attackRight(elapsedTime);
	else if (current_animation == Animation::CLIMB)
		climb(elapsedTime);
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
	current_rotation = 0.0f;

	current_animation = animation;
}

void AnimationComponent::positionToRotation(const int y)
{
	const auto height = DataManager::getInstance().height;

		
	//int numer = Y_TRIGGER_DISTANCE;
	//Ensures the rotation is 160 at the top trigger area of the screen, and 0 at the bottom trigger area of the screen
	float rotation_per_pixel = ATTACK_MAX_ROTATION / (height - (y_trigger_distance * 2));

	//Inverse, because 0 == 160 degrees rotation
	// -y_trigger_distance, because the rotation has to end when entering the trigger area
	current_rotation = ATTACK_MAX_ROTATION - ((y-y_trigger_distance) * rotation_per_pixel);
}

void AnimationComponent::run(const float elapsedTime)
{
	if (ani_forward)
		current_rotation += 150.0f * elapsedTime;
	else
		current_rotation -= 150.0f * elapsedTime;

	if (current_rotation <= 0.0f)
		ani_forward = true;
	else if (current_rotation >= 90.0f)
		ani_forward = false;

	rig.getNode("la_u").setRotation({ -current_rotation + 45, 0, 0 });
	rig.getNode("la_l").setRotation({ -current_rotation,0, 0 });
	rig.getNode("ra_u").setRotation({ -90 + current_rotation + 45,0, 0 });
	rig.getNode("ra_l").setRotation({ -90 + current_rotation,0, 0 });

	rig.getNode("ll_u").setRotation({ current_rotation - 45, 0, 0 });
	rig.getNode("ll_l").setRotation({ current_rotation,0, 0 });
	rig.getNode("rl_u").setRotation({ 90 - current_rotation - 45,0, 0 });
	rig.getNode("rl_l").setRotation({ 90 - current_rotation,0, 0 });
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
	if (ani_forward)
		current_rotation += 5.0f * elapsedTime;
	else
		current_rotation -= 5.0f * elapsedTime;

	if (current_rotation <= 0.0f)
		ani_forward = true;
	else if (current_rotation >= 5.0f)
		ani_forward = false;

	float idle_rotation = current_rotation * 0.003f;

	rig.getNode("torso").setPosition({ 0,-idle_rotation, 0 });

	rig.getNode("la_u").setRotation({ -current_rotation * 0.5f, 0, 0 });
	rig.getNode("la_l").setRotation({ -current_rotation,0, 0 });
	rig.getNode("ra_u").setRotation({ -10 + current_rotation * 0.5f,0, 0 });
	rig.getNode("ra_l").setRotation({ -10 + current_rotation,0, 0 });

	rig.getNode("ll_u").setRotation({ -current_rotation, 0, 0 });
	rig.getNode("ll_l").setRotation({ current_rotation,0, 0 });
	rig.getNode("rl_u").setRotation({ -10 + current_rotation,0, 0 });
	rig.getNode("rl_l").setRotation({ 10 - current_rotation,0, 0 });
}

void AnimationComponent::attackMouse(const float elapsedTime)
{
	// Get mousePos and screen height
	const auto mousePos = DataManager::getInstance().mousePos;
	const auto height = DataManager::getInstance().height;

	positionToRotation(mousePos.y);

	if (current_rotation <= 0.0f) 
	{

		rig.getNode("la_weapon").setMesh("none");
		rig.getNode("la_weapon").setTexture("none");
		current_rotation = 0.0f;
	}
		
	else if (current_rotation >= ATTACK_MAX_ROTATION)
	{
		current_rotation = ATTACK_MAX_ROTATION;
		rig.getNode("la_weapon").setMesh(DataManager::getInstance().currentWeapon);
		rig.getNode("la_weapon").setTexture(DataManager::getInstance().currentWeapon);
		Math::vec3d arm_left= { 0.61229f , 0.068894f ,1.45316f };
		Math::vec3d arm_right = { -0.605497f, 0.112396f, 1.48285f };

		rig.getNode("la_weapon").setPosition(convertCoordinates(DataManager::getInstance().weaponMap[DataManager::getInstance().currentWeapon],arm_left));
	}
		

	rig.getNode("la_u").setRotation({ -current_rotation,0, 0 });
	rig.getNode("la_l").setRotation({ -current_rotation,0, 0 });
	rig.getNode("ra_u").setRotation({ -current_rotation,0, 0 });
	rig.getNode("ra_l").setRotation({ -current_rotation,0, 0 });

	float leg_rotation = current_rotation / 30.0f;

	rig.getNode("torso").setPosition({ 0,leg_rotation*0.01f, 0 });

	rig.getNode("ll_u").setRotation({ -20,0, 0 });
	rig.getNode("ll_l").setRotation({ leg_rotation + 10,0, 0 });
	rig.getNode("rl_u").setRotation({ leg_rotation - 10,0, 0 });
	rig.getNode("rl_l").setRotation({ leg_rotation,0, 0 });
}

void AnimationComponent::climb(const float elapsedTime)
{
	rig.setRotation({ 0,180,0 });

	if (ani_forward)
		current_rotation += 150.0f * elapsedTime;
	else
		current_rotation -= 150.0f * elapsedTime;

	if (current_rotation <= 0.0f)
		ani_forward = true;
	else if (current_rotation >= 90.0f)
		ani_forward = false;

//	rig.addRotation({ -30, 0, 0 });
	rig.getNode("torso").setRotation({ -30,0,0 });

	rig.getNode("la_u").setRotation({ -current_rotation - 30, 0, 0 });
	rig.getNode("la_l").setRotation({ current_rotation / 5 - 100 ,0, 0 });
	rig.getNode("ra_u").setRotation({ -90 + current_rotation ,0, 0 });
	rig.getNode("ra_l").setRotation({ current_rotation / 5 - 100  ,0, 0 });

	rig.getNode("ll_u").setRotation({ -current_rotation, 0, 0 });
	rig.getNode("ll_l").setRotation({ current_rotation, 0, 0 });
	rig.getNode("rl_u").setRotation({ -90 + current_rotation,0, 0 });
	rig.getNode("rl_l").setRotation({ 90 - current_rotation,0, 0 });
}


extern Player *player;
void AnimationComponent::attack(const float elapsedTime)
{
	if (ani_forward)
		current_rotation += 150.0f * elapsedTime;
	else
		current_rotation -= 150.0f * elapsedTime;

	if (current_rotation <= 0.0f)
		ani_forward = true;
	else if (current_rotation >= 160.0f)
		ani_forward = false;

	//After one rotation of the animation, switch to the cheer animation
	if (current_rotation <= 0.0f)
	{
		current_rotation = 0.0f;

		setAnimation(Animation::CHEER);
	}

	else if (current_rotation >= ATTACK_MAX_ROTATION)
	{
		current_rotation = ATTACK_MAX_ROTATION;
	}
	
	
	rig.getNode("la_u").setRotation({ -current_rotation,0, 0 });
	rig.getNode("la_l").setRotation({ -current_rotation,0, 0 });
	rig.getNode("ra_u").setRotation({ -current_rotation,0, 0 });
	rig.getNode("ra_l").setRotation({ -current_rotation,0, 0 });

	float leg_rotation = current_rotation / 30.0f;

	rig.getNode("torso").setPosition({ 0,5.5f+leg_rotation*0.01f, 0 });

	rig.getNode("ll_u").setRotation({ -20,0, 0 });
	rig.getNode("ll_l").setRotation({ leg_rotation + 10,0, 0 });
	rig.getNode("rl_u").setRotation({ leg_rotation - 10,0, 0 });
	rig.getNode("rl_l").setRotation({ leg_rotation,0, 0 });
	
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

void AnimationComponent::cheer(const float elapsedTime)
{
}

Math::vec3d AnimationComponent::convertCoordinates(const Math::vec3d &posCords, const Math::vec3d &parent)
{
	Math::vec3d convertedCords;
	convertedCords.x = posCords.x - parent.x;
	convertedCords.y = posCords.z - parent.z;
	convertedCords.z = -posCords.y - (-parent.y);

	return convertedCords;
}

