#pragma once
#include "State.h"

class WinState : public State
{
public:
	WinState();
	virtual ~WinState();

	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) override;
	void update(float elapsedTime) override;
};
