#pragma once
#include "State.h"

class WinState : public State
{
private:
	float timePassed;
public:
	WinState();
	virtual ~WinState();
	
	
	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
	void hovering(float elapsedTime);
};
