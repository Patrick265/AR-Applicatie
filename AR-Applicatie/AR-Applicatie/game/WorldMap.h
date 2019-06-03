#pragma once
#include <vector>
#include "../objects/GameObject.h"
#include "../util/ObjLoader.h"
#include "../util/TextureHandler.h"
#include "../util/MousePicking.h"

/*
Class which holds and manipulates the objects in the menu screen (Map)
*/

class WorldMap
{
private:
	// variables
	int x;
	int y;
	float height;
	float deltatime;
	MousePicking* mousePicking;
	int margin;
	float increment;

	// Gameobjects
	GameObject* map;
	GameObject* icon;
	GameObject* wIcon;
	GameObject* skybox;
	std::vector<GameObject*> icons;

public:
	WorldMap();
	~WorldMap();

	// Method for updating the variables in the worldmap
	void update(float deltaTime, int x, int y, int height);
	// Method for drawing the whole map
	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures);

	// Method returns game objects for the map
	std::vector<GameObject*> getGameObjects() const;
	// Method returns the mousepicking object
	MousePicking* getMousePicking() const;
};

