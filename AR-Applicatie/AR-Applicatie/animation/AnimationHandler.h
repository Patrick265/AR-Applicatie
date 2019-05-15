#pragma once

#include "Rig.h"


enum Animations {RUN, IDLE, ATTACK, CLIMB};

class AnimationHandler {

	Rig* rig;
	int current_animation;

	int cursor_pos_y;
	float deltaTime;

	//The current rotation, which the animation is based on
	float current_rotation = 0.0f;
	/*
		Animations loop forward and backward, this tracks whether they are going forward or backward.
	*/
	bool ani_forward = false;

	

public:
	void setRig(Rig* rig);

	void setCursorPos(int y);
	
	void animate(float deltaTime);
	void draw();

	void setAnimation(int animation);
		

private:
	void run();
	void idle();
	void attack();
	void climb();

	//For the attack animation, it matches the rotation of the character with the position of the cursor
	void positionToRotation(int y);
};