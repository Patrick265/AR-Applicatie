#include "StaticComponent.h"
#include "../opengl/DrawHandler.h"
#include <GL/freeglut.h>
#include "../objects/GameObject.h"

StaticComponent::StaticComponent(const std::string &mesh, const std::string &texture)
{
	this->mesh = mesh;
	this->texture = texture;
};

StaticComponent::~StaticComponent() = default;

void StaticComponent::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	glPushMatrix();

	glTranslatef(gameObject->getPosition().x, gameObject->getPosition().y, gameObject->getPosition().z);
	glRotatef(gameObject->getRotation().x, 1, 0, 0);
	glRotatef(gameObject->getRotation().y, 0, 1, 0);
	glRotatef(gameObject->getRotation().z, 0, 0, 1);
	glScalef(gameObject->getScale().x, gameObject->getScale().y, gameObject->getScale().z);

	DrawHandler::drawMesh_array(meshes[mesh], textures[texture]);

	glPopMatrix();
}
