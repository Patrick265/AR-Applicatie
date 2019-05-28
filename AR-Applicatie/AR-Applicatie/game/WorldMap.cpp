#include "WorldMap.h"
#include "../components/StaticComponent.h"
#include "../data/DataManager.h"

WorldMap::WorldMap()
{
	this->map = new GameObject();
	this->map->addComponent(new StaticComponent("map", "map"));
	this->map->setPosition({ -1.5, 18, 26 });
	this->map->setRotation({ 0, 90, 0});

	this->icon = new GameObject();
	this->icon->addComponent(new StaticComponent("icon", "none"));
	this->icon->setPosition({ 1.3563f, 18.50308f, 11});
	this->icon->setRotation({ 0, 90, 0 });

	this->wIcon = new GameObject();
	this->wIcon->addComponent(new StaticComponent("wicon", "none"));
	this->wIcon->setPosition({ -1.5, 18.25, 17.25});
	this->wIcon->setRotation({ 0, 90, 0 });
	this->margin = 100;
	this->increment = 0.1f;
	this->icons = std::vector<GameObject*>();
	this->icons.push_back(this->icon);
	this->icons.push_back(this->wIcon);
	this->mousePicking = new MousePicking(this->icons, 1080, 0,0);
}

WorldMap::~WorldMap()
{
	delete map;
	delete icon;
	delete wIcon;
}

void WorldMap::update(float deltaTime, int x, int y, int height)
{
	if(y <= margin && this->map->getPosition().z <= 24.5)
	{
		this->map->setPosition({ -1.5, 18, this->map->getPosition().z + increment });
		this->icon->setPosition({ 1.3563f, 18.50308f, this->icon->getPosition().z + increment });
		this->wIcon->setPosition({ -1.5, 18.25, this->wIcon->getPosition().z + increment });
	} else if(y >= height - margin && this->map->getPosition().z >= -7)
	{
		this->map->setPosition({ -1.5, 18, this->map->getPosition().z - increment });
		this->icon->setPosition({ 1.3563f, 18.50308f, this->icon->getPosition().z -increment });
		this->wIcon->setPosition({ -1.5, 18.25, this->wIcon->getPosition().z - increment });
	}
	this->x = x;
	this->y = y;
	this->height = static_cast<float>(height);
	this->deltatime = deltaTime;
	
}


void WorldMap::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures)
{
	for (auto o : getGameObjects())
	{
		for (auto c : o->getComponents())
		{
			c->draw(meshes, textures);
		}
	}
	

	this->mousePicking->update(x, y, static_cast<int>(this->height), this->deltatime);
}

std::vector<GameObject*> WorldMap::getGameObjects() const
{
	std::vector<GameObject*> gameObjects;
	gameObjects.clear();
	gameObjects.push_back(map);
	gameObjects.push_back(icon);
	gameObjects.push_back(wIcon);
	return gameObjects;
}
MousePicking* WorldMap::getMousePicking() const
{
	return this->mousePicking;
}