#define _USE_MATH_DEFINES
#include <cmath>

#include <GL/freeglut.h>
#include <opencv2/opencv.hpp>

#include "util/ObjLoader.h"
#include "util/TextureHandler.h"
#include "objects/GameObject.h"

const float WIDTH = 1600;
const float HEIGHT = 800;

float lastFrameTime;

float fTheta;

std::vector<GameObject> game_objects;

struct Camera
{
	float
	posX = 2,
	posY = 2,
	posZ = 2,
	rotX = 45,
	rotY = -45;
} camera;

bool keys[255];
bool justMovedMouse = false;
bool mouseControl = true;

void onIdle();
void onDisplay();
void onKey(unsigned char c, int x, int y);
void onKeyUp(unsigned char c, int x, int y);
void onMousePassiveMotion(int x, int y);
void moveCamera(float angle, float fac);

void standardRenderOperations();
void drawMesh(Graphics::mesh mesh, uint16_t texture_id);
void displayText();

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) / 2);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Game Of Thrones: AR");

	glutIdleFunc(onIdle);
	glutDisplayFunc(onDisplay);
	glutKeyboardFunc(onKey);
	glutKeyboardUpFunc(onKeyUp);
	glutPassiveMotionFunc(onMousePassiveMotion);

	Math::vec3d pos = { -5, 0, 0 };
	Math::vec3d rot = { 0.0f, 0.0f, 0.0f };
	Math::vec3d scale = { 1.0f, 1.0f, 1.0f };

	 GameObject obj1(ObjLoader::loadObj("Resources/Rune/NPC_arms.obj"),
	 	    TextureHandler::addTexture("Resources/Rune/npc_arms.png"));
	 obj1.setPosition(pos);
	 obj1.setRotation(rot);
	 obj1.setScale(scale);
 
	 GameObject obj2(ObjLoader::loadObj("Resources/Rune/NPC_head.obj"),
	 	    TextureHandler::addTexture("Resources/Rune/npc_head.png"));
	 obj2.setPosition(pos);
	 obj2.setRotation(rot);
	 obj2.setScale(scale);
 
	 GameObject obj3(ObjLoader::loadObj("Resources/Rune/NPC_legs.obj"),
	 		TextureHandler::addTexture("Resources/Rune/npc_legs.png"));
	 obj3.setPosition(pos);
	 obj3.setRotation(rot);
	 obj3.setScale(scale);
 
	 GameObject obj4(ObjLoader::loadObj("Resources/Rune/NPC_torso.obj"),
	 	    TextureHandler::addTexture("Resources/Rune/npc_torso.png"));
	 obj4.setPosition(pos);
	 obj4.setRotation(rot);
	 obj4.setScale(scale);


	// pos = {5, 0, 0 };
 //
	// GameObject obj2(ObjLoader::loadObj("Resources/Hammer/hammer.obj"),
	// 		TextureHandler::addTexture("Resources/Hammer/hammer.png"));
	// obj2.setPosition(Math::vector_add(pos, pos));
	// obj2.setRotation(rot);
	// obj2.setScale(scale);

	game_objects.push_back(obj1);
	game_objects.push_back(obj2);
	game_objects.push_back(obj3);
	game_objects.push_back(obj4);


	lastFrameTime = glutGet(GLUT_ELAPSED_TIME);
	
	glutWarpPointer(WIDTH / 2, HEIGHT / 2);

	glutMainLoop();
}

void onIdle() 
{
	//Calculate delta time
	float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float deltaTime = currentTime - lastFrameTime;
	lastFrameTime = currentTime;

	fTheta += 30.0f * deltaTime;

	const float speed = 6;
	if (keys[int('a')]) moveCamera(0, deltaTime * speed);
	if (keys[int('d')]) moveCamera(180, deltaTime * speed);
	if (keys[int('w')]) moveCamera(90, deltaTime * speed);
	if (keys[int('s')]) moveCamera(270, deltaTime * speed);
	if (keys[int('e')]) camera.posZ += deltaTime * speed;
	if (keys[int('q')]) camera.posZ -= deltaTime * speed;

	glutPostRedisplay();
}

void onDisplay() 
{
	standardRenderOperations();

	// Test cube in the center of the world
	glPushMatrix();
	glTranslatef(-0.5, -0.5, -0.5);
	glutSolidCube(1);
	glPopMatrix();

	//Testing purposes for the npc
	for(int i = 0; i < game_objects.size(); i++)
	{
		auto game_obj = game_objects[i];
		//To rotate the object
		//Move to an update function for the gameobjects, which will be called in the idle function
		Math::vec3d rot = game_obj.getRotation();
		rot.x = fTheta * game_obj.getTextureId();
		game_obj.setRotation(rot);

		glPushMatrix();

		int armsId = 0;
		if(i == armsId)
		{
			glTranslatef(game_obj.getPosition().x, game_obj.getPosition().y + 4.1, game_obj.getPosition().z - 0.1);
		}
		else
		{
			glTranslatef(game_obj.getPosition().x, game_obj.getPosition().y, game_obj.getPosition().z);
		}

		if(i == armsId)
		{
			glRotatef(game_obj.getRotation().x, 1,0,0);
			glRotatef(game_obj.getRotation().y, 0,1,0);
			glRotatef(game_obj.getRotation().z, 0,0,1);
		}


		glScalef(game_obj.getScale().x, game_obj.getScale().y, game_obj.getScale().z);

		drawMesh(game_obj.getMesh(), game_obj.getTextureId());

		glPopMatrix();
	}

	/// old rendering code, obv a bit more clean, but for testing purposes we use the NPC.

	// for (GameObject game_obj : game_objects) {
 //
	// 	//To rotate the object
	// 	//Move to an update function for the gameobjects, which will be called in the idle function
	// 	Math::vec3d rot = game_obj.getRotation();
	// 	rot.y = fTheta * game_obj.getTextureId();
	// 	game_obj.setRotation(rot);
 //
	// 	glPushMatrix();
 //
	// 	glTranslatef(game_obj.getPosition().x, game_obj.getPosition().y, game_obj.getPosition().z);
 //
	// 	glRotatef(game_obj.getRotation().x, 1,0,0);
	// 	glRotatef(game_obj.getRotation().y, 0,1,0);
	// 	glRotatef(game_obj.getRotation().z, 0,0,1);
 //
	// 	glScalef(game_obj.getScale().x, game_obj.getScale().y, game_obj.getScale().z);
 //
	// 	drawMesh(game_obj.getMesh(), game_obj.getTextureId());
 //
	// 	glPopMatrix();
	// }

	displayText();

	glutSwapBuffers();
}

void standardRenderOperations()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4, 0.4, 0.4, 1.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	//Swapping between perspective and orthographic mode (disabled)
	// if (currentRenderMode == PERSP)
	gluPerspective(70.0f, WIDTH / HEIGHT, 0.1f, 1000.0f);
	// else if (currentRenderMode == ORTHO)
	// 	glOrtho(-20, 20, -20, 20, 1.0, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Change to camera position
	glRotatef(camera.rotX, 1, 0, 0);
	glRotatef(camera.rotY, 0, 1, 0);
	glTranslatef(-camera.posX, -camera.posZ, -camera.posY);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
}

void drawMesh(Graphics::mesh mesh, uint16_t texture_id)
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

void displayText()
{
	// Create a string that displays the current camera location and rotation
	std::string text = "x " + std::to_string(camera.posX) + "\ny " + std::to_string(camera.posY) + "\nz " +
		std::to_string(camera.posZ) + "\nX " + std::to_string(camera.rotX) + "\nY " + std::to_string(camera.rotY);

	int xpos = -15;
	int ypos = 20;
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 100.0, 0.0, 0.0, 1.0);
	glRasterPos2f(xpos, ypos);
	glColor3f(1, 1, 1);
	int len = text.length();
	for (int i = 0; i < len; i++)
	{
		if (text[i] == '\n')
		{
			ypos += 2;
			glRasterPos2f(xpos, ypos);
			continue;
		}
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}
	glColor3f(1, 1, 1);
}

void onKey(unsigned char keyId, int x, int y)
{
	if (keyId == VK_ESCAPE)
		exit(1);
	if (keyId == 'c')
		mouseControl = !mouseControl;	keys[keyId] = true;
}

void onKeyUp(unsigned char keyId, int, int)
{
	keys[keyId] = false;
}

void onMousePassiveMotion(int x, int y)
{
	if (!mouseControl)
		return;

	int dx = x - WIDTH / 2;
	int dy = y - HEIGHT / 2;
	if ((dx != 0 || dy != 0) && abs(dx) < 400 && abs(dy) < 400 && !justMovedMouse)
	{
		camera.rotY += dx / 10.0f;
		camera.rotX += dy / 10.0f;
		if (camera.rotX < -90)
			camera.rotX = -90;
		if (camera.rotX > 90)
			camera.rotX = 90;
	}
	if (!justMovedMouse)
	{
		glutWarpPointer(WIDTH / 2, HEIGHT / 2);
		justMovedMouse = true;
	}
	else
		justMovedMouse = false;
}

void moveCamera(float angle, float fac)
{
	camera.posX -= (float)cos((camera.rotY + angle) / 180 * M_PI) * fac;
	camera.posY -= (float)sin((camera.rotY + angle) / 180 * M_PI) * fac;
}
