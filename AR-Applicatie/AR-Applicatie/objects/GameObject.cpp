#include "GameObject.h"


GameObject::GameObject(Graphics::mesh mesh, uint16_t texture_id)
{
	this->mesh = mesh;
	this->texture_id = texture_id;

	this->position = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->scale = { 0.0f, 0.0f, 0.0f, 0.0f };
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
Math::vec3d GameObject::getScale()
{
	return scale;
}

Graphics::mesh GameObject::getMesh()
{
	return mesh;
}
uint16_t GameObject::getTextureId()
{
	return texture_id;
}
