#include "DeathState.h"
#include <GL/freeglut.h>

DeathState::DeathState() = default;

DeathState::~DeathState() = default;

void DeathState::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	// Test cube in the center of the world
	glPushMatrix();
	glTranslatef(-0.5, -0.5, -0.5);
	glutSolidCube(1);
	glPopMatrix();
}

void DeathState::update(float elapsedTime)
{
}
