#include "AnimationHandler.h"





void AnimationHandler::setRig(Rig* rig)
{
	this->rig = rig;
}

void AnimationHandler::setCursorPos(int y)
{
	this->cursor_pos_y = y;
}

void AnimationHandler::animate(float deltaTime)
{
	this->deltaTime = deltaTime;

	if (current_animation == RUN)
		run();
	else if (current_animation == IDLE)
		idle();
	else if (current_animation == ATTACK)
		attack();
	else if (current_animation == CLIMB)
		climb();
}

void AnimationHandler::draw()
{
	rig->drawRig();
}

void AnimationHandler::setAnimation(int animation)
{
	current_rotation = 0.0f;

	current_animation = animation;
}

void AnimationHandler::positionToRotation(int y)
{
	current_rotation = -y/2;
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

	rig->getNode("la_u")->setRotation({ -current_rotation + 45, 0, 0 });
	rig->getNode("la_l")->setRotation({ -current_rotation,0, 0 });
	rig->getNode("ra_u")->setRotation({ -90 + current_rotation + 45,0, 0 });
	rig->getNode("ra_l")->setRotation({ -90 + current_rotation,0, 0 });

	rig->getNode("ll_u")->setRotation({ current_rotation - 45, 0, 0 });
	rig->getNode("ll_l")->setRotation({ current_rotation,0, 0 });
	rig->getNode("rl_u")->setRotation({ 90 - current_rotation - 45,0, 0 });
	rig->getNode("rl_l")->setRotation({ 90 - current_rotation,0, 0 });
}

void AnimationHandler::idle()
{
	if (ani_forward)
		current_rotation += 150.0f * deltaTime;
	else
		current_rotation -= 150.0f * deltaTime;

	if (current_rotation <= 0.0f)
		ani_forward = true;
	else if (current_rotation >= 90.0f)
		ani_forward = false;

	rig->getNode("la_u")->setRotation({ -current_rotation + 45, 0, 0 });
	rig->getNode("la_l")->setRotation({ -current_rotation,0, 0 });
	rig->getNode("ra_u")->setRotation({ -90 + current_rotation + 45,0, 0 });
	rig->getNode("ra_l")->setRotation({ -90 + current_rotation,0, 0 });

	rig->getNode("ll_u")->setRotation({ current_rotation - 45, 0, 0 });
	rig->getNode("ll_l")->setRotation({ current_rotation,0, 0 });
	rig->getNode("rl_u")->setRotation({ 90 - current_rotation - 45,0, 0 });
	rig->getNode("rl_l")->setRotation({ 90 - current_rotation,0, 0 });
}

void AnimationHandler::attack()
{
	positionToRotation(cursor_pos_y);

	if (current_rotation <= 0.0f)
		current_rotation = 0.0f;
	else if (current_rotation >= 180.0f)
		current_rotation = 180.0f;

	rig->getNode("la_u")->setRotation({ -current_rotation, 0, 0 });
	rig->getNode("la_l")->setRotation({ -current_rotation,0, 0 });
	rig->getNode("ra_u")->setRotation({ -current_rotation,0, 0 });
	rig->getNode("ra_l")->setRotation({ -current_rotation,0, 0 });


	float leg_rotation = current_rotation / 30.0f;
	
	rig->getNode("torso")->setPosition({ 0,leg_rotation*0.01f, 0 });

	rig->getNode("ll_u")->setRotation({ -20,0, 0 });
	rig->getNode("ll_l")->setRotation({ leg_rotation+10,0, 0 });
	rig->getNode("rl_u")->setRotation({ leg_rotation-10,0, 0 });
	rig->getNode("rl_l")->setRotation({ leg_rotation,0, 0 });
	
}

void AnimationHandler::climb()
{

}
