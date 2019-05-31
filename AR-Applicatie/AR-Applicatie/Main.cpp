#include "data/DataManager.h"
#include <GL/freeglut.h>
#include <thread>

DataManager *data = &DataManager::getInstance();
markerdetection m;
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

	std::thread openCV(runOpenCVThread);

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

	if (m.hasNewMousePosition())
		data->onMotionData(static_cast<int>(data->mousePos.x), static_cast<int>(data->mousePos.y));

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
	while (true)
		m.runMarkerDetection(markerdetection::DetectionMode::opencv);
}
