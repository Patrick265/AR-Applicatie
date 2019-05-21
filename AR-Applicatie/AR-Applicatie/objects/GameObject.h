#pragma once

#include "../calculations/Graphics.h"
#include "../components/Component.h"
#include <list>

class GameObject
{
	std::list<Component *> components;

protected:

	Math::vec3d position;
	Math::vec3d rotation;
	Math::vec3d scale;

public:

	GameObject();

	void addComponent(Component *component);
	std::list<Component *> getComponents() const;

	void update(float elapsedTime);
	void draw(std::map<std::string, Graphics::mesh> &meshes, std::map<std::string, uint16_t> &textures);

	void setPosition(Math::vec3d pos);
	void setRotation(Math::vec3d rot);

	void setScale(Math::vec3d scale);
	Math::vec3d getPosition() const;
	Math::vec3d getRotation() const;
	Math::vec3d getScale() const;
	std::string getMesh() const;
	std::string getTexture() const;

	template<class T>
	T* getComponent()
	{
		for (auto c : components)
		{
			T* t = dynamic_cast<T*>(c);
			if (t)
				return t;
		}
		return nullptr;
	}

	template<class T>
	void removeComponent()
	{
		components.remove_if([](Component* c)
		{
			T* t = dynamic_cast<T*>(c);
			return t != nullptr;
		});
	}
};
