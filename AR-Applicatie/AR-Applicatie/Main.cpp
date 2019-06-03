#include "data/DataManager.h"
#include <GL/freeglut.h>
#include <thread>

markerdetection m;

void onIdle();
void onDisplay();
void runOpenCVThread();

int main(int argc, char** argv)
{
	auto data = &DataManager::getInstance();
	data->initGlut(argc, argv, onIdle, onDisplay);
	data->initResources();

	data->stateHandler.setState(StateHandler::States::MENU);

	std::thread openCV(runOpenCVThread);

	glutMainLoop();
}

void onIdle()
{
	//Calculate delta time
	static float lastTime;
	const auto currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	const auto deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	if (deltaTime < 0.0f || deltaTime > 1.0f)
		return;

	auto data = &DataManager::getInstance();
	data->stateHandler.update(deltaTime);
	data->updateCamera(deltaTime);

	if (m.hasNewMousePosition())
		data->onMotionData(static_cast<int>(data->mousePos.x), static_cast<int>(data->mousePos.y));

	glutPostRedisplay();
}

void onDisplay()
{
	auto data = &DataManager::getInstance();
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
