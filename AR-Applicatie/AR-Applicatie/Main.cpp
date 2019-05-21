#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#include <GL/freeglut.h>
#include <opencv2/opencv.hpp>

#include "util/ObjLoader.h"
#include "util/TextureHandler.h"
#include "objects/GameObject.h"
#include "game/GameLogic.h"
#include "vision/markerdetection.h"

float width = 1280;
float height = 720;

float deltaTime;
float lastFrameTime;

Point2D mousePos;

GameLogic gameLogic;

struct Camera
{
	float
		posX = 0,
		posY = 15,
		posZ = 20,
		rotX = 20,
		rotY = 0;
} camera;

std::map<std::string, Graphics::mesh> meshes;
std::map<std::string, uint16_t> textures;

bool keys[255];
bool justMovedMouse = false;
bool mouseControl = true;

void initResources();

void onIdle();
void onDisplay();
void onReshape(int w, int h);
void onKey(unsigned char keyId, int x, int y);
void onKeyUp(unsigned char keyId, int x, int y);
void onMouse();
void onMotion(int x, int y);
void onMousePassiveMotion(int x, int y);
void moveCamera(float angle, float fac);

void standardRenderOperations();
void displayText();
void runOpenCVThread();

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
	glutMotionFunc(onMotion);
	glutReshapeFunc(onReshape);

	Math::vec3d pos = { -5, 0, 0 };
	Math::vec3d rot = { 0.0f, 0.0f, 0.0f };
	Math::vec3d scale = { 1.0f, 1.0f, 1.0f };

	initResources();

	//Cursor image
	cursorID = TextureHandler::addTexture("Resources/Cursor/16x16_cursor_icon.png", textures.size());
	glutSetCursor(GLUT_CURSOR_NONE);

	lastFrameTime = glutGet(GLUT_ELAPSED_TIME);

	//std::thread openCV(runOpenCVThread);
	//openCV.join();
	runMarkerDetection(MARKERDETECTION_WITH_OPENCV);

	glutMainLoop();
}

void onIdle()
{
	//Calculate delta time
	const auto currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	deltaTime = currentTime - lastFrameTime;
	lastFrameTime = currentTime;

	if (deltaTime < 0 || deltaTime > 1)
		return;

	// Check for vision mouse updates
	if (hasNewMousePosition())
		onMouse();

	const float speed = 6;
	if (keys[int('a')]) moveCamera(0, deltaTime * speed);
	if (keys[int('d')]) moveCamera(180, deltaTime * speed);
	if (keys[int('w')]) moveCamera(90, deltaTime * speed);
	if (keys[int('s')]) moveCamera(270, deltaTime * speed);
	if (keys[int('e')]) camera.posZ += deltaTime * speed;
	if (keys[int('q')]) camera.posZ -= deltaTime * speed;

	gameLogic.update(deltaTime);

	//runMarkerDetection(MARKERDETECTION_WITH_MOUSE);

	glutPostRedisplay();
}

void runOpenCVThread()
{
	//runMarkerDetection(MARKERDETECTION_WITH_OPENCV);
}

void onDisplay()
{
	standardRenderOperations();
	gameLogic.draw(meshes, textures);
	displayText();

	glutSwapBuffers();
}

void standardRenderOperations()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4, 0.4, 0.4, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, float(width) / float(height), 0.1f, 1000.0f);

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

void displayText()
{
	// Create a string that displays the fps, current camera location and rotation
	auto text =
		"fps " + std::to_string(int(1 / deltaTime)) +
		"\nx " + std::to_string(camera.posX) +
		"\ny " + std::to_string(camera.posY) +
		"\nz " + std::to_string(camera.posZ) +
		"\nX " + std::to_string(camera.rotX) +
		"\nY " + std::to_string(camera.rotY);

	const auto xPos = 20;
	auto yPos = 30;
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glClearDepth(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRasterPos2f(xPos, yPos);
	glColor3f(1, 1, 1);
	const int len = text.length();
	for (auto i = 0; i < len; i++)
	{
		if (text[i] == '\n')
		{
			yPos += 20;
			glRasterPos2f(xPos, yPos);
			continue;
		}
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, cursorID);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); glVertex2f(mousePos.x - 25, mousePos.y - 25);
	glTexCoord2f(0, 1); glVertex2f(mousePos.x - 25, mousePos.y + 25);
	glTexCoord2f(1, 1); glVertex2f(mousePos.x + 25, mousePos.y + 25);
	glTexCoord2f(1, 0); glVertex2f(mousePos.x + 25, mousePos.y - 25);
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
		gameLogic.throwProjectile(rand() % 10 - 5, -1);	keys[keyId] = true;
}

void onKeyUp(unsigned char keyId, int, int)
{
	keys[keyId] = false;
}

void onMouse()
{
	// If controls with OpenCV
	if (!mouseControl)
	{
		Point2D normalized = getCoordinates();
		mousePos = { normalized.x * width, normalized.y * height };
		/*
				double xScreen = mousePos.x * width;
				double yScreen = mousePos.y * height;

				if (xScreen >= 0 && xScreen <= width && yScreen >= 0 && yScreen <= height)
				{
					cursorX = xScreen;
					cursorY = yScreen;
					glutWarpPointer(xScreen, yScreen);
				}
				else
				{
					SetCursorPos(width / 2, height / 2);
				}*/
	}
}

void onMotion(int x, int y)
{
	onMousePassiveMotion(x, y);
}

void onMousePassiveMotion(int x, int y)
{
	if (mouseControl)
		mousePos = { float(x), float(y) };
	/*
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
		//glutWarpPointer(width / 2, height / 2);
		justMovedMouse = true;
	}
	else
		justMovedMouse = false;
	*/
}

void onReshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}

void moveCamera(float angle, float fac)
{
	camera.posX -= (float)cos((camera.rotY + angle) / 180 * M_PI) * fac;
	camera.posY -= (float)sin((camera.rotY + angle) / 180 * M_PI) * fac;
}

void initRigParts()
{
	/*
		FEMALE ELF
	*/
	textures["elf_head"] = TextureHandler::addTexture("Resources/Rune/npc_head.png", textures.size());
	meshes["elf_head"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_head.obj");
	meshes["elf_neck"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_head_neck.obj");

	textures["elf_torso"] = TextureHandler::addTexture("Resources/Rune/npc_torso.png", textures.size());
	meshes["elf_torso"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_torso.obj");

	textures["elf_legs"] = TextureHandler::addTexture("Resources/Rune/npc_legs.png", textures.size());
	meshes["elf_lowerbod"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_skirt.obj");
	meshes["elf_ll_u"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_top_left.obj");
	meshes["elf_ll_l"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_bottom_left.obj");
	meshes["elf_rl_u"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_top_right.obj");
	meshes["elf_rl_l"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_bottom_right.obj");

	textures["elf_arms"] = TextureHandler::addTexture("Resources/Rune/npc_arms.png", textures.size());
	meshes["elf_la_u"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_left_top.obj");
	meshes["elf_la_l"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_left_bottom.obj");
	meshes["elf_ra_u"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_right_top.obj");
	meshes["elf_ra_l"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_right_bottom.obj");

	textures["elf_sack"] = TextureHandler::addTexture("Resources/Rigid_NPC/NPC_big_sack.png", textures.size());
	meshes["elf_sack"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_big_sack.obj");

	/*
	GOBLIN
	*/
	textures["goblin_torso"] = TextureHandler::addTexture("Resources/Enemy/goblin_torso.png", textures.size());
	meshes["goblin_torso"] = ObjLoader::loadObj("Resources/Enemy/Goblin_torso.obj");

	textures["goblin_la_u"] = TextureHandler::addTexture("Resources/Enemy/goblin_arm_left_top.png", textures.size());
	meshes["goblin_la_u"] = ObjLoader::loadObj("Resources/Enemy/Goblin_arm_left_top.obj");

	textures["goblin_ra_u"] = TextureHandler::addTexture("Resources/Enemy/goblin_arm_top_right.png", textures.size());
	meshes["goblin_ra_u"] = ObjLoader::loadObj("Resources/Enemy/Goblin_arm_right_top.obj");

	textures["goblin_ll_u"] = TextureHandler::addTexture("Resources/Enemy/goblin_leg_left_top.png", textures.size());
	meshes["goblin_ll_u"] = ObjLoader::loadObj("Resources/Enemy/Goblin_leg_left_top.obj");
	meshes["goblin_rl_u"] = ObjLoader::loadObj("Resources/Enemy/Goblin_leg_right_top.obj");

	textures["goblin_la_l"] = TextureHandler::addTexture("Resources/Enemy/goblin_arm_left_bottom.png", textures.size());
	meshes["goblin_la_l"] = ObjLoader::loadObj("Resources/Enemy/Goblin_arm_left_bottom.obj");

	textures["goblin_ra_l"] = TextureHandler::addTexture("Resources/Enemy/goblin_arm_right_bottom.png", textures.size());
	meshes["goblin_ra_l"] = ObjLoader::loadObj("Resources/Enemy/Goblin_arm_right_bottom.obj");

	textures["goblin_ll_l"] = TextureHandler::addTexture("Resources/Enemy/goblin_leg_left_bottom.png", textures.size());
	meshes["goblin_ll_l"] = ObjLoader::loadObj("Resources/Enemy/Goblin_leg_left_bottom.obj");

	textures["goblin_rl_l"] = TextureHandler::addTexture("Resources/Enemy/goblin_leg_right_bottom.png", textures.size());
	meshes["goblin_rl_l"] = ObjLoader::loadObj("Resources/Enemy/Goblin_leg_right_bottom.obj");
}

void initGameLogicModels()
{
	textures["wall"] = TextureHandler::addTexture("Resources/Wall/Snow.jpg",textures.size());
	meshes["cube"] = ObjLoader::loadObj("Resources/Cube/cube.obj");
	
	textures["giant"] = TextureHandler::addTexture("Resources/Rune/giant.png", textures.size());
	meshes["giant"] = ObjLoader::loadObj("Resources/Rune/giant.obj");

	textures["packet"] = TextureHandler::addTexture("Resources/Pakketje/Pakketje.png", textures.size());
	meshes["packet"] = ObjLoader::loadObj("Resources/Pakketje/Pakketje.obj");

	textures["brick"] = TextureHandler::addTexture("Resources/Weapons/Brick.png", textures.size());
}

void initResources()
{
	//To prevent the map from throwing an exception
	meshes["none"] = {};
	textures["none"] = -1;

	initRigParts();
	initGameLogicModels();
}
