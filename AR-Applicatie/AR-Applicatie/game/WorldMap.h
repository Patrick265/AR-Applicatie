#pragma once
#include <vector>
#include "../objects/GameObject.h"
#include "../util/ObjLoader.h"
#include "../util/TextureHandler.h"
#include "../util/MousePicking.h"

class WorldMap
{
private:
	GameObject* map;
	GameObject* icon;
	int x;
	int y;
	float height;
	float deltatime;
	MousePicking* mousePicking;


public:
	WorldMap();
	~WorldMap();

	void init();
	void update(float deltaTime, int x, int y, int height);
	void draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures);

	std::vector<GameObject*> getGameObjects() const;
	MousePicking* getMousePicking() const;

	
};

