#pragma once

#include "Component.h"
#include "../animation/Rig.h"

class AnimationComponent : public Component
{
	// The rig that performs the animations
	Rig rig;	

	// The current rotation, which the animation is based on
	float current_rotation;
	// Animations loop forward and backward, this tracks whether they are going forward or backward.
	bool ani_forward;

	//The maximum rotation for the attack animation
	const float ATTACK_MAX_ROTATION = 160.0f;


public:

	// The animations that can be chosen
	enum class Animation { RUN_LEFT, RUN_RIGHT, IDLE, ATTACK, CLIMB };

	AnimationComponent(Rig rig);
	AnimationComponent(const AnimationComponent &animation_handler);

	// Draws the joints from the rig
	void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures) override;

	/*
		Moves the joints through the selected animation

		@param deltaTime - The amount of time that has passed since the last frame
		@param cursor_pos_y - The y axis of the cursor, used for the attack animation
	*/
	void update(float elapsedTime) override;

	/*
		Sets the current animation
		Takes an value from the Animations enum above

		@param animation - The given animation
	*/
	void setAnimation(Animation animation);

	/*
		Returns the currently selected animation
	*/
	const Animation &getCurrentAnimation() { return current_animation; }

private:

	// The currently selected animation
	Animation current_animation;

	// The animations
	void run(float elapsedTime);
	void runLeft(float elapsedTime);
	void runRight(float elapsedTime);

	void idle(float elapsedTime);
	void attack(float elapsedTime);
	void climb(float elapsedTime);

	Math::vec3d convertCoordinates(Math::vec3d posCords, Math::vec3d parent);

	// For the attack animation, it matches the rotation of the character with the position of the cursor
	void positionToRotation(int y);
};
