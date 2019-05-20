#include "AnimationHandler.h"

AnimationHandler::AnimationHandler()
{
	deltaTime = 0;
	current_animation = IDLE;
	current_rotation = 0.0f;
	ani_forward = true;
}

AnimationHandler::AnimationHandler(const AnimationHandler & ani)
	:rig(ani.rig), current_animation(ani.current_animation),
	deltaTime(ani.deltaTime),
	current_rotation(ani.current_rotation), ani_forward(ani.ani_forward)
{
}

void AnimationHandler::setRig(Rig rig)
{
	this->rig = rig;
}


void AnimationHandler::animate(const float &deltaTime, const int &cursor_pos_y)
{
	this->deltaTime = deltaTime;

	if (current_animation == RUN)
		run();
	else if (current_animation == IDLE)
		idle();
	else if (current_animation == ATTACK)
		attack(cursor_pos_y);
	else if (current_animation == CLIMB)
		climb();
}

void AnimationHandler::draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures)
{
	rig.drawRig(meshes, textures);
}

void AnimationHandler::setAnimation(int animation)
{
	current_rotation = 0.0f;

	current_animation = animation;
}

void AnimationHandler::positionToRotation(int y)
{
	//With 100 pixels away from the centre it reaches maximum rotation, 160 degrees
	current_rotation = -y * 1.6f;
}

void AnimationHandler::run()
{
	if (ani_forward)
		current_rotation += 150.0f * deltaTime;
	else
		current_rotation -= 150.0f * deltaTime;

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

void AnimationHandler::idle()
{
	if (ani_forward)
		current_rotation += 5.0f * deltaTime;
	else
		current_rotation -= 5.0f * deltaTime;

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

void AnimationHandler::attack(const int &cursor_pos_y)
{
	positionToRotation(cursor_pos_y);

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

void AnimationHandler::climb()
{
	if (ani_forward)
		current_rotation += 150.0f * deltaTime;
	else
		current_rotation -= 150.0f * deltaTime;

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
