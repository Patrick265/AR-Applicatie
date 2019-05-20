#include "GameObject.h"


GameObject::GameObject(const std::string &mesh, const std::string &texture)
{
	this->mesh = mesh;
	this->texture = texture;

	this->position = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->scale = { 1.0f, 1.0f, 1.0f, 0.0f };
}

void GameObject::setPosition(Math::vec3d pos)
{
	this->position = pos;
}
void GameObject::setRotation(Math::vec3d rot)
{
	this->rotation = rot;
}
void GameObject::setScale(Math::vec3d scale)
{
	this->scale = scale;
}

Math::vec3d GameObject::getPosition()
{
	return position;
}
Math::vec3d GameObject::getRotation()
{
	return rotation;
}
Math::vec3d GameObject::getScale() const
{
	return scale;
}

std::string GameObject::getMesh() const
{
	return mesh;
}
std::string GameObject::getTexture() const
{
	return texture;
}
