#include "data/DataManager.h"
#include <GL/freeglut.h>

DataManager *data = &DataManager::getInstance();

float deltaTime;
float lastFrameTime;

void onIdle();
void onDisplay();
void runOpenCVThread();

int main(int argc, char** argv)
{
	data->initGlut(argc, argv, onIdle, onDisplay);
	data->initResources();

	data->stateHandler.setState(StateHandler::States::MENU);
	

	//std::thread openCV(runOpenCVThread);
	//openCV.join();
	// runMarkerDetection(MARKERDETECTION_WITH_OPENCV);

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

	data->stateHandler.update(deltaTime);
	data->updateCamera();

	// // Check for vision mouse updates
	// if (hasNewMousePosition())
	// 	onMouse();
	//
	// runMarkerDetection(MARKERDETECTION_WITH_MOUSE);

	glutPostRedisplay();
}

void onDisplay()
{
	data->standardRenderOperations();
	data->stateHandler.draw(data->meshes, data->textures);
	data->displayInfo();

	glutSwapBuffers();
}

void runOpenCVThread()
{
	//runMarkerDetection(MARKERDETECTION_WITH_OPENCV);
}
