#include "AnimationComponent.h"
#include "../vision/markerdetection.h"
#include <GL/freeglut.h>
#include "../objects/GameObject.h"

extern Point2D mousePos;
extern float height;

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

void AnimationComponent::update(float elapsedTime)
{
	if (current_animation == Animation::RUN_LEFT)
		runLeft(elapsedTime);
	else if (current_animation == Animation::RUN_RIGHT)
		runRight(elapsedTime);
	else if (current_animation == Animation::IDLE)
		idle(elapsedTime);
	else if (current_animation == Animation::ATTACK)
		attack(elapsedTime);
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

void AnimationComponent::setAnimation(Animation animation)
{
	//To prevent past animations from affecting the new one
	rig.setRotation({ 0,0,0 });

	//Starting the animation over
	current_rotation = 0.0f;

	current_animation = animation;
}

void AnimationComponent::positionToRotation(int y)
{
	//With 100 pixels away from the centre it reaches maximum rotation, 160 degrees
	current_rotation = -y * 1.6f;
}

void AnimationComponent::run(float elapsedTime)
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

void AnimationComponent::runLeft(float elapsedTime)
{
	run(elapsedTime);
	rig.setRotation({ 0,-90,0 });
}

void AnimationComponent::runRight(float elapsedTime)
{
	run(elapsedTime);
	rig.setRotation({ 0,90,0 });
}

void AnimationComponent::idle(float elapsedTime)
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

void AnimationComponent::attack(float elapsedTime)
{
	positionToRotation(mousePos.y - height / 2);

	if (current_rotation <= 0.0f)
		current_rotation = 0.0f;
	else if (current_rotation >= 160.0f)
		current_rotation = 160.0f;

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

void AnimationComponent::climb(float elapsedTime)
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
