#pragma once
#include "State.h"

class DeathState : public State
{
private:

	std::vector<std::string> text;
public:
	DeathState();
	virtual ~DeathState();

	float timePassed;
	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
};
