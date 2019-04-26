#include <GL/GLUT.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>

#include "util/ObjLoader.h"
#include "util/TextureHandler.h"
#include "objects/GameObject.h"

const float WIDTH = 1600;
const float HEIGHT = 800;

float startTime;
float currentTime;

float fTheta;

std::vector<GameObject> game_objects;

enum renderMode { ORTHO, PERSP };
int currentRenderMode;

void render(void);
void keyboard(unsigned char c, int x, int y);
void mouse(int button, int state, int x, int y);
void idle(void);

void standardRenderOperations();
void drawMesh(Graphics::mesh mesh, uint16_t texture_id);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Game Of Thrones: AR");

	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	Math::vec3d pos = { 0, 50, -30 };
	Math::vec3d rot = { 90.0f, 0.0f, 0.0f };
	Math::vec3d scale = { 1.0f, 1.0f, 1.0f };

	GameObject obj1(ObjLoader::LoadObj("Resources/Toot_Braustein/Toot_Braustein.obj"),
		    TextureHandler::addTexture("Resources/Toot_Braustein/Toot_Braustein.jpg"));
	obj1.setPosition(pos);
	obj1.setRotation(rot);
	obj1.setScale(scale);

	GameObject obj2(ObjLoader::LoadObj("Resources/Hammer/hammer.obj"),
			TextureHandler::addTexture("Resources/Hammer/hammer.png"));
	obj2.setPosition(Math::vector_add(pos, pos));
	obj2.setRotation(rot);
	obj2.setScale(scale);

	game_objects.push_back(obj1);
	game_objects.push_back(obj2);

	startTime = glutGet(GLUT_ELAPSED_TIME);

	glutMainLoop();
}

void keyboard(unsigned char c, int x, int y) 
{

	//Spacebar
	if (c == 32) {
		if (currentRenderMode == PERSP)
			currentRenderMode = ORTHO;
		else
			currentRenderMode = PERSP;
	}

	if (c == 27) {
		exit(0);
	}
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON) {
		exit(0);
	}
}

void idle(void) 
{
	//Deltatime
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = currentTime - startTime;
	startTime = currentTime;
	//-----

	fTheta += 0.05f * deltaTime;

	glutPostRedisplay();
}

void render(void) 
{
	standardRenderOperations();

	for (GameObject game_obj : game_objects) {

		//To rotate the object
		//Move to an update function for the gameobjects, which will be called in the idle function
		Math::vec3d rot = game_obj.getRotation();
		rot.y = fTheta * game_obj.getTextureId();
		game_obj.setRotation(rot);

		glPushMatrix();

		glTranslatef(game_obj.getPosition().x, game_obj.getPosition().y, game_obj.getPosition().z);

		glRotatef(game_obj.getRotation().x, 1,0,0);
		glRotatef(game_obj.getRotation().y, 0,1,0);
		glRotatef(game_obj.getRotation().z, 0,0,1);

		glScalef(game_obj.getScale().x, game_obj.getScale().y, game_obj.getScale().z);

		drawMesh(game_obj.getMesh(), game_obj.getTextureId());

		glPopMatrix();
	}

	glutSwapBuffers();
}

void standardRenderOperations()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4, 0.4, 0.4, 1.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	//Swapping between perspective and orthographic mode
	if (currentRenderMode == PERSP)
		gluPerspective(70.0f, WIDTH / HEIGHT, 1.0f, 10000.0f);
	else if (currentRenderMode == ORTHO)
		glOrtho(-20, 20, -20, 20, 1.0, 10000.0f);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, -20, 10,
		0, 0, 0,
		0, 1, 0);

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