#pragma once

#include "Rig.h"

//The animations that can be chosen
enum Animations { RUN, IDLE, ATTACK, CLIMB };

class AnimationHandler {

	//The rig that performs the animations
	Rig rig;
	//The currently selected animation
	int current_animation;

	//The time since the last frame
	float deltaTime;

	//The current rotation, which the animation is based on
	float current_rotation;
	/*
		Animations loop forward and backward, this tracks whether they are going forward or backward.
	*/
	bool ani_forward;



public:

	AnimationHandler();
	AnimationHandler(const AnimationHandler &animation_handler);

	/*
		Sets the rig that will be used for the animations

		@param rig - The given rig
	*/
	void setRig(Rig rig);

	/*
		Moves the joints through the selected animation

		@param deltaTime - The amount of time that has passed since the last frame
		@param cursor_pos_y - The y axis of the cursor, used for the attack animation
	*/
	void animate(const float &deltaTime, const int &cursor_pos_y);

	/*
		Draws the joints from the rig
	*/
	void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures);

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
	void attack(const int &cursor_pos_y);
	void climb();

	//For the attack animation, it matches the rotation of the character with the position of the cursor
	void positionToRotation(int y);
};