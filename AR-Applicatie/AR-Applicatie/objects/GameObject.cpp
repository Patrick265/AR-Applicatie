#include "GameObject.h"

GameObject::GameObject()
{
	this->position = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->scale = { 1.0f, 1.0f, 1.0f, 0.0f };
}

void GameObject::addComponent(Component *component)
{
	component->setGameObject(this);
	components.push_back(component);
}

std::list<Component *> GameObject::getComponents() const
{
	return components;
}

void GameObject::draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures)
{
	for (auto &c : components)
		c->draw(meshes, textures);
}

void GameObject::update(float elapsedTime)
{
	for (auto &c : components)
		c->update(elapsedTime);
}

void GameObject::setPosition(const Math::vec3d pos)
{
	this->position = pos;
}

void GameObject::setRotation(const Math::vec3d rot)
{
	this->rotation = rot;
}

void GameObject::setScale(const Math::vec3d scale)
{
	this->scale = scale;
}

Math::vec3d GameObject::getPosition() const
{
	return position;
}

Math::vec3d GameObject::getRotation() const
{
	return rotation;
}

Math::vec3d GameObject::getScale() const
{
	return scale;
}
