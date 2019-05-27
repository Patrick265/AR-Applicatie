#include "../util/MousePicking.h"
#include "../data/DataManager.h"

DataManager *dataMP = &DataManager::getInstance();

MousePicking::MousePicking(GameObject *objects, int height, int x, int y)
{
	this->objectsToCheck = objects;
	this->windowHeight = height;
	this->lastX = x;
	this->lastY = y;
	this->cursorCounter = 0;
	this->isCounting = false;
	this->isSettingsScreen = false;
	this->timePassed = 0;
	this->cursorCounter = 0;
}

MousePicking::~MousePicking()
{
	
}

void MousePicking::checkSettings(int x, int y) {
	if (y >= 20 && y <= 30 && x>=30 && x <= 170) {
		this->isCounting = true;
		this->isSettingsScreen = true;
	}
}

void MousePicking::searchObject(int cursorX, int cursorY)
{
	float winZ;
	glReadPixels(cursorX, windowHeight - cursorY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	double modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	double projMatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	double x, y, z;
	gluUnProject
	(
		cursorX,
		windowHeight - cursorY,
		winZ,
		modelMatrix,
		projMatrix,
		viewport,
		&x,
		&y,
		&z
	);
	//std::cout << "xyz: " << x << ", " << y << ", " << z << std::endl;
	const double xIcon = objectsToCheck->getPosition().x;
	const double yIcon = objectsToCheck->getPosition().y;
	const double zIcon = objectsToCheck->getPosition().z;
	const double sizeIcon = 0.5f;
	
	if ((xIcon + sizeIcon) > x && (xIcon - sizeIcon) < x &&
		(yIcon + sizeIcon) > y && (yIcon - sizeIcon) < y &&
		(zIcon + sizeIcon) > z && (zIcon - sizeIcon) < z)
	{
		isCounting = true;
	}
	else
	{
		
	}
}

void MousePicking::update(int cursorX, int cursorY, int height, float time) 
{
	if (!isCounting) {
		this->windowHeight = height;
		if (abs(cursorX - lastX) <= 2 && abs(cursorY - lastY) <= 2) {
			cursorCounter += time;
			if (cursorCounter >= 1.5f && timePassed <= 1.6f) {
				searchObject(cursorX, cursorY);
				checkSettings(cursorX, cursorY);
				cursorCounter = 0;
			}
		}
		else {
			cursorCounter = 0;
		}
	}
	else {
		if (abs(cursorX - lastX) <= 5 && abs(cursorY - lastY) <= 5) {
			//std::cout << "entered" << time << std::endl;
			timePassed += time;
			
			if (timePassed >= 3.0f && timePassed <= 3.1f) {
				if (isSettingsScreen) {
					std::cout << "settings screen selected" << std::endl;
					isCounting = false;
					isSettingsScreen = false;
					timePassed = 0;
				}
				else {
					//std::cout << "selected the game!!" << std::endl;
					isCounting = false;
					timePassed = 0;
					dataMP->stateHandler.setState(StateHandler::States::GAME);
				}
			}
			
		}
		else {
			isCounting = false;
			this->timePassed = 0;
		}
		DataManager::getInstance().scaleLoading = timePassed * 10;
	}
	
	lastX = cursorX;
	lastY = cursorY;
}

float MousePicking::getTimePassed() {
	return timePassed;
}