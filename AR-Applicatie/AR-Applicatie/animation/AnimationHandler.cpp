#include "AnimationHandler.h"

#include <string>

AnimationHandler::AnimationHandler()
{

}

void AnimationHandler::AddAnimation()
{
	Frame* frame = new Frame(0.5f,"la_u",90.0f);

	Frame2 frame2;
	frame2.node_name = "la_u";
	frame2.target_rotation = 90.0;
	frame2.transition_time = 90.0;


}

Frame::Frame(float transition_time, std::string node_name, float target_rotation)
{
	this->transition_time = transition_time;
	this->node_name = node_name;
	this->target_rotation = target_rotation;
}
