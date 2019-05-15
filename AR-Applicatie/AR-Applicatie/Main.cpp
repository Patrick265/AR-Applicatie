#define _USE_MATH_DEFINES
#include <cmath>

#include <GL/freeglut.h>
#include <opencv2/opencv.hpp>

#include "util/ObjLoader.h"
#include "util/TextureHandler.h"
#include "objects/GameObject.h"
#include "game/GameLogic.h"
#include "vision/markerdetection.h"
#include "animation/Rig.h"

float width = 1920;
float height = 1080;

float deltaTime;
float lastFrameTime;

//Rotation for the showcasing of the rigging
float current_rotation = 0.0f;
//Showcasing rigging through simple hardcoded animation
bool arm_up = false;

Rig* rig;


float fTheta;

//std::vector<GameObject> game_objects;

GameLogic gameLogic;
GameObject* map;
GameObject* castleBlackIcon;

struct Camera
{
	float
		posX = 13.63f,
		posY = 2.73f,
		posZ = 2,
		rotX = 42,
		rotY = 91;
} camera;

bool keys[255];
bool justMovedMouse = false;
bool mouseControl = true;

void onIdle();
void onDisplay();
void onReshape(int width, int height);
void onKey(unsigned char c, int x, int y);
void onKeyUp(unsigned char c, int x, int y);
void onMousePassiveMotion(int x, int y);
void moveCamera(float angle, float fac);

void standardRenderOperations();
void drawMesh(Graphics::mesh mesh, uint16_t texture_id);
void drawGameObject(GameObject game_obj);
void displayText();
void runOpencCVThread();
void initMap();

int cursorID;
int cursorX = 0;
int cursorY = 0;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);
	glutInitWindowSize(width, height);
	glutCreateWindow("Game Of Thrones: AR");

	glutIdleFunc(onIdle);
	glutDisplayFunc(onDisplay);
	glutKeyboardFunc(onKey);
	glutKeyboardUpFunc(onKeyUp);
	glutPassiveMotionFunc(onMousePassiveMotion);
	glutReshapeFunc(onReshape);

	current_rotation = 0.0f;

	Math::vec3d pos = { -5, 0, 0 };
	Math::vec3d rot = { 0.0f, 0.0f, 0.0f };
	Math::vec3d scale = { 1.0f, 1.0f, 1.0f };

	rig = new Rig(pos, rot, scale);
	rig->rigFemaleElf();

	//Cursor image
	cursorID = TextureHandler::addTexture("Resources/Cursor/16x16_cursor_icon.png");
	glutSetCursor(GLUT_CURSOR_NONE);

	lastFrameTime = glutGet(GLUT_ELAPSED_TIME);

	glutWarpPointer(width / 2, height / 2);
	initMap();

	//std::thread openCV(runOpencCVThread);
	//openCV.join();
	//runMarkerDetection(MARKERDETECTION_WITH_OPENCV);

	glutMainLoop();
}

void onIdle()
{
	//Calculate delta time
	float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	deltaTime = currentTime - lastFrameTime;
	lastFrameTime = currentTime;

	if (deltaTime < 0)
		return;

	fTheta += 30.0f * deltaTime;

	rig->setRotation({ 0,fTheta * 2,0 });

	//Hardcoded animation to showcase rig
	if (arm_up)
		current_rotation += 150.0f * deltaTime;
	else
		current_rotation -= 150.0f * deltaTime;

	if (current_rotation <= 0.0f)
		arm_up = true;
	else if (current_rotation >= 90.0f)
		arm_up = false;

	Node * la_u = rig->getNode("la_u");
	la_u->setRotation({ -current_rotation + 45, 0, 0 });
	Node * la_l = rig->getNode("la_l");
	la_l->setRotation({ -current_rotation,0, 0 });
	Node * ra_u = rig->getNode("ra_u");
	ra_u->setRotation({ -90 + current_rotation + 45,0, 0 });
	Node * ra_l = rig->getNode("ra_l");
	ra_l->setRotation({ -90 + current_rotation,0, 0 });

	Node * ll_u = rig->getNode("ll_u");
	ll_u->setRotation({ current_rotation - 45, 0, 0 });
	Node * ll_l = rig->getNode("ll_l");
	ll_l->setRotation({ current_rotation,0, 0 });
	Node * rl_u = rig->getNode("rl_u");
	rl_u->setRotation({ 90 - current_rotation - 45,0, 0 });
	Node * rl_l = rig->getNode("rl_l");
	rl_l->setRotation({ 90 - current_rotation,0, 0 });


	const float speed = 6;
	if (keys[int('a')]) moveCamera(0, deltaTime * speed);
	if (keys[int('d')]) moveCamera(180, deltaTime * speed);
	if (keys[int('w')]) moveCamera(90, deltaTime * speed);
	if (keys[int('s')]) moveCamera(270, deltaTime * speed);
	if (keys[int('e')]) camera.posZ += deltaTime * speed;
	if (keys[int('q')]) camera.posZ -= deltaTime * speed;

	gameLogic.update(deltaTime);
	
	//runMarkerDetection(MARKERDETECTION_WITH_OPENCV);
	
	glutPostRedisplay();
}

void runOpencCVThread() 
{
	runMarkerDetection(MARKERDETECTION_WITH_OPENCV);
}

void onDisplay()
{
	standardRenderOperations();


	glPushMatrix();
	rig->drawRig();
	glPopMatrix();

	/*// Test cube in the center of the world
	glPushMatrix();
	glTranslatef(-0.5, -0.5, -0.5);
	glutSolidCube(1);
	glPopMatrix();

	//Testing purposes for the npc
	for(int i = 0; i < game_objects.size(); i++)
	{
		GameObject game_obj = game_objects[i];
		//To rotate the object
		//Move to an update function for the gameobjects, which will be called in the idle function
		Math::vec3d rot = game_obj.getRotation();
		rot.x = fTheta * game_obj.getTextureId();
		game_obj.setRotation(rot);

		glPushMatrix();




		glScalef(game_obj.getScale().x, game_obj.getScale().y, game_obj.getScale().z);

		drawMesh(game_obj.getMesh(), game_obj.getTextureId());

		glPopMatrix();
	}*/

	// for (GameObject* gameObject : gameLogic.getGameObjects())
	// 	drawGameObject(*gameObject);

	drawGameObject(*map);
	drawGameObject(*castleBlackIcon);

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
	gluPerspective(70.0f, float(width) / float(height), 0.1f, 1000.0f);
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

void drawMesh2(Graphics::mesh mesh, uint16_t texture_id)
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

void drawGameObject(GameObject game_obj)
{
	glPushMatrix();

	glTranslatef(game_obj.getPosition().x, game_obj.getPosition().y, game_obj.getPosition().z);
	glRotatef(game_obj.getRotation().x, 1, 0, 0);
	glRotatef(game_obj.getRotation().y, 0, 1, 0);
	glRotatef(game_obj.getRotation().z, 0, 0, 1);
	glScalef(game_obj.getScale().x, game_obj.getScale().y, game_obj.getScale().z);

	drawMesh2(game_obj.getMesh(), game_obj.getTextureId());

	glPopMatrix();
}

void displayText()
{
	// Create a string that displays the fps, current camera location and rotation
	std::string text =
		"fps " + std::to_string(int(1 / deltaTime)) +
		"\nx " + std::to_string(camera.posX) +
		"\ny " + std::to_string(camera.posY) +
		"\nz " + std::to_string(camera.posZ) +
		"\nX " + std::to_string(camera.rotX) +
		"\nY " + std::to_string(camera.rotY);

	int xpos = 20;
	int ypos = 30;
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glClearDepth(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRasterPos2f(xpos, ypos);
	glColor3f(1, 1, 1);
	int len = text.length();
	for (int i = 0; i < len; i++)
	{
		if (text[i] == '\n')
		{
			ypos += 20;
			glRasterPos2f(xpos, ypos);
			continue;
		}
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, cursorID);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0, 0); glVertex2f(cursorX - 25, cursorY - 25);
	glTexCoord2f(0, 1); glVertex2f(cursorX - 25, cursorY + 25);
	glTexCoord2f(1, 1); glVertex2f(cursorX + 25, cursorY + 25);
	glTexCoord2f(1, 0); glVertex2f(cursorX + 25, cursorY - 25);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void onKey(unsigned char keyId, int x, int y)
{
	if (keyId == VK_ESCAPE)
		exit(1);
	if (keyId == 'c')
		mouseControl = !mouseControl;
	if (keyId == 't')
		gameLogic.throwProjectile();	keys[keyId] = true;
}

void onKeyUp(unsigned char keyId, int, int)
{
	keys[keyId] = false;
}

void onMousePassiveMotion(int x, int y)
{
	if (!mouseControl) {
		Point2D mousePos = getCoordinates();

		double xScreen = mousePos.x * 3;
		double yScreen = (double)mousePos.y * 2.25;

		if (xScreen >= 0 && xScreen <= width && yScreen >= 0 && yScreen <= height) {
			cursorX = xScreen;
			cursorY = yScreen;
			glutWarpPointer(xScreen, yScreen);

		}
		else {
			SetCursorPos(width / 2, height / 2);
		}
	}
	cursorX = x;
	cursorY = y;
	int dx = x - width / 2;
	int dy = y - height / 2;
	if ((dx != 0 || dy != 0) && abs(dx) < 400 && abs(dy) < 400 && !justMovedMouse)
	{
		camera.rotY += dx / 10.0f;
		camera.rotX += dy / 10.0f;
		if (camera.rotX < -90)
			camera.rotX = -90;
		if (camera.rotX > 90)
			camera.rotX = 90;
		if (camera.rotY > 360)
			camera.rotY -= 360;
		if (camera.rotY <= 0)
			camera.rotY += 360;
	}

	if (!justMovedMouse)
	{
		glutWarpPointer(width / 2, height / 2);
		justMovedMouse = true;
	}
	else
		justMovedMouse = false;

}

void onReshape(int w, int h)
{
	width = w;
	height = h;
}

void moveCamera(float angle, float fac)
{
	camera.posX -= (float)cos((camera.rotY + angle) / 180 * M_PI) * fac;
	camera.posY -= (float)sin((camera.rotY + angle) / 180 * M_PI) * fac;
}

void initMap()
{

	map = new GameObject(ObjLoader::loadObj("Resources/Map/Map.obj"), TextureHandler::addTexture("Resources/Map/Map.jpg"));
	map->setPosition(Math::vec3d{ 0, 0, 0 });
	map->setScale(Math::vec3d{ 1, 1, 1 });
	map->setRotation(Math::vec3d{ 0, 0, 0 });
	castleBlackIcon = new GameObject(ObjLoader::loadObj("Resources/Map/Castleblack icon.obj"), -1);
	castleBlackIcon->setPosition(Math::vec3d{ 0, 0, 0});
	castleBlackIcon->setScale(Math::vec3d{ 1, 1, 1 });
	castleBlackIcon->setRotation(Math::vec3d{ 0, 0, 0 });
	

}