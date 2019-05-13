#pragma once

#include <vector>
#include <string>


class Frame {
	
	float transition_time;
	std::string node_name;
	float target_rotation;
public:
	Frame(float transition_time, std::string node_name, float target_rotation);

};

struct Frame2 {
	float transition_time;
	std::string node_name;
	float target_rotation;
};

class AnimationHandler {


public:

	

private:


	std::vector<std::vector<Frame>> animations;

		// elke unieke animatie
		// voor elke actie
		// 3 floats -> {tijd van transitie, joint, doel rotatie } 
		//

public:
	AnimationHandler();
	void AddAnimation();

};