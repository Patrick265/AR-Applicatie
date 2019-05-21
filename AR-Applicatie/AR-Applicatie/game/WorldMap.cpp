#include "WorldMap.h"

WorldMap::WorldMap(float height, int x, int y)
{
	this->objects = std::vector<GameObject*>();
	this->height = height;
	this->x = x;
	this->y = y;
	this->deltatime = 0;
	init();
}

WorldMap::~WorldMap()
{
}

void WorldMap::init()
{
	GameObject* map = new GameObject(ObjLoader::loadObj("Resources/Map/Map.obj"), TextureHandler::addTexture("Resources/Map/Map.jpg"));
	map->setPosition(Math::vec3d{ 0, 0, 0 });
	map->setScale(Math::vec3d{ 1, 1, 1 });
	map->setRotation(Math::vec3d{ 0, 0, 0 });

	GameObject* castleBlackIcon = new GameObject(ObjLoader::loadObj("Resources/Map/Castleblack icon.obj"), -1);
	castleBlackIcon->setPosition(Math::vec3d{ 15.277f, 0.50308f, 2.8563f });
	castleBlackIcon->setScale(Math::vec3d{ 1, 1, 1 });

	GameObject * skybox = new GameObject(ObjLoader::loadObj("Resources/Skybox/skybox.obj"), -1);
	skybox->setPosition(Math::vec3d{ 0, 0, 0 });
	skybox->setScale(Math::vec3d{ 1, 1, 1 });
	skybox->setRotation(Math::vec3d{ 0, 0, 0 });

	this->objects.push_back(map);
	this->objects.push_back(castleBlackIcon);
	this->objects.push_back(skybox);
	this->mousePicking = new MousePicking{ castleBlackIcon, static_cast<int>(this->height), this->x, this->y };
}

void WorldMap::update(float deltaTime, int x, int y, int height)
{
	this->x = x;
	this->y = y;
	this->height = height;
	this->deltatime = deltaTime;
	
}


void WorldMap::draw()
{
	for (GameObject* element : this->objects)
	{
		drawGameObject(*element);
	}
	//std::cout << deltatime << std::endl;
	this->mousePicking->update(x, y, this->height, this->deltatime);

}

MousePicking* WorldMap::getMousePicking()
{
	return this->mousePicking;
}




void WorldMap::drawObj(Graphics::mesh mesh, uint16_t texture_id)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);

	for (Graphics::triangle tri : mesh.tris) {

		glColor3f(1.0f, 1.0f, 1.0f);

		Math::vec3d normal = Graphics::triangle_getNormal(tri);
		glNormal3f(normal.x, normal.y, normal.z);
		for (int i = 0; i < 3; i++)
		{
			glTexCoord2f(tri.vt[i].x, tri.vt[i].y);
			glVertex3f(tri.p[i].x, tri.p[i].y, tri.p[i].z);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void WorldMap::drawGameObject(GameObject game_obj)
{
	glPushMatrix();

	glTranslatef(game_obj.getPosition().x, game_obj.getPosition().y, game_obj.getPosition().z);
	glRotatef(game_obj.getRotation().x, 1, 0, 0);
	glRotatef(game_obj.getRotation().y, 0, 1, 0);
	glRotatef(game_obj.getRotation().z, 0, 0, 1);
	glScalef(game_obj.getScale().x, game_obj.getScale().y, game_obj.getScale().z);

	drawObj(game_obj.getMesh(), game_obj.getTextureId());

	glPopMatrix();
}