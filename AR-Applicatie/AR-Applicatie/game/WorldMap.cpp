#include "WorldMap.h"
#include "../components/StaticComponent.h"

WorldMap::WorldMap()
{
	this->map = new GameObject();
	this->map->addComponent(new StaticComponent("map", "map"));
	this->map->setPosition({ -1.5, 18, 26 });
	this->map->setRotation({ 0, 90, 0});

	this->icon = new GameObject();
	this->icon->addComponent(new StaticComponent("icon", "none"));
	this->icon->setPosition({ 1.3563f, 18.50308f, 11 });
	this->icon->setRotation({ 0, 90, 0 });
	this->mousePicking = new MousePicking(icon, 1080, 0,0);
}

WorldMap::~WorldMap()
{
	delete map;
	delete icon;
}

void WorldMap::update(float deltaTime, int x, int y, int height)
{
	this->x = x;
	this->y = y;
	this->height = height;
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
	

	this->mousePicking->update(x, y, this->height, this->deltatime);
}

std::vector<GameObject*> WorldMap::getGameObjects() const
{
	std::vector<GameObject*> gameObjects;
	gameObjects.clear();
	gameObjects.push_back(map);
	gameObjects.push_back(icon);
	return gameObjects;
}
MousePicking* WorldMap::getMousePicking() const
{
	return this->mousePicking;
}