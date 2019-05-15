#pragma once

#include "Rig.h"

//The animations that can be chosen
enum Animations {RUN, IDLE, ATTACK, CLIMB};

class AnimationHandler {

	//The rig that performs the animations
	Rig* rig;
	//The currently selected animation
	int current_animation;

	//The y position of the cursor
	int cursor_pos_y;
	//The time since the last frame
	float deltaTime;

	//The current rotation, which the animation is based on
	float current_rotation = 0.0f;
	/*
		Animations loop forward and backward, this tracks whether they are going forward or backward.
	*/
	bool ani_forward = false;

	

public:
	/*
		Sets the rig that will be used for the animations

		@param rig - The given rig
	*/
	void setRig(Rig* rig);

	/*
		Sets the y position of the screen, for the attack animation

		@param y - The y position of the screen
	*/
	void setCursorPos(int y);
	
	/*
		Moves the joints through the selected animation

		@param deltaTime - The amount of time that has passed since the last frame
	*/
	void animate(float deltaTime);

	/*
		Draws the joints from the rig
	*/
	void draw();

	/*
		Sets the current animation
		Takes an value from the Animations enum above

		@param animation - The given animation
	*/
	void setAnimation(int animation);
		

private:
	//The animations
	void run();
	void idle();
	void attack();
	void climb();

	//For the attack animation, it matches the rotation of the character with the position of the cursor
	void positionToRotation(int y);
};