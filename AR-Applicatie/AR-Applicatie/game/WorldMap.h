#pragma once
#include <vector>
#include "../objects/GameObject.h"
#include "../util/ObjLoader.h"
#include "../util/TextureHandler.h"
#include "../util/MousePicking.h"

class WorldMap
{
private:
	std::vector<GameObject *> objects;
	int x;
	int y;
	float height;
	float deltatime;
	MousePicking* mousePicking;


public:
	WorldMap(float height, int x, int y);
	~WorldMap();

	
	void init();
	void update(float deltaTime, int x, int y, int height);
	void draw();
	void drawObj(Graphics::mesh mesh, uint16_t texture_id);
	void drawGameObject(GameObject game_obj);
	MousePicking* getMousePicking();

	
};

