#include "../util/MousePicking.h"
#include "../data/DataManager.h"

DataManager *dataMP = &DataManager::getInstance();

MousePicking::MousePicking(std::vector<GameObject *> objects, int height, int x, int y)
{
	this->objectsToCheck = objects;
	this->windowHeight = height;
	this->lastX = x;
	this->lastY = y;
	this->cursorCounter = 0;
	this->isCounting = false;
	this->timePassed = 0;
}

MousePicking::~MousePicking()
{
	
}

void MousePicking::checkSettings(int x, int y) 
{
	if ((y >= 20 && y <= 30 && x>=30 && x <= 190)||(y >= 250 && y <= 260 && x >= 580 && x <= 660)) {
		this->isCounting = true;
		dataMP->settings.isSettingsScreen = true;
	}
}

void MousePicking::checkChanges(int x, int y)
{
	if (y >= 350 && y <= 360 && x >= 580 && x <= 660) {
		this->isCounting = true;
		dataMP->settings.isChangeDif = true;
	}
	else if (y >= 370 && y <= 380 && x >= 580 && x <= 660) {
		this->isCounting = true;
		dataMP->settings.isChangeInput = true;
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
	for(int i =0; i < objectsToCheck.size(); i++)
	{
		const double xIcon = objectsToCheck.at(i)->getPosition().x;
		const double yIcon = objectsToCheck.at(i)->getPosition().y;
		const double zIcon = objectsToCheck.at(i)->getPosition().z;
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

}

void MousePicking::update(int cursorX, int cursorY, int height, float time) 
{
	//std::cout << "x,y: " << cursorX << " : " << cursorY << std::endl;
	if (!isCounting) {
		this->windowHeight = height;
		if (abs(cursorX - lastX) <= 2 && abs(cursorY - lastY) <= 2) {
			cursorCounter += time;
			if (cursorCounter >= 0.1f && timePassed <= 0.2f) {
				searchObject(cursorX, cursorY);
				checkSettings(cursorX, cursorY);
				if (dataMP->settingsActive == true) {
					checkChanges(cursorX, cursorY);
				}
				cursorCounter = 0;
			}
		}
		else {
			cursorCounter = 0;
		}
	}
	else {
		if (abs(cursorX - lastX) <= 5 && abs(cursorY - lastY) <= 5) {
			timePassed += time;
			if (timePassed >= 3.0f && timePassed <= 3.1f) {
				// Check for the setting screen to apear or disapear
				if (dataMP->settings.isSettingsScreen) {
					if (dataMP->settingsActive == true) {
						dataMP->settingsActive = false;
					}
					else {
						dataMP->settingsActive = true;
					}						
					dataMP->settings.isSettingsScreen = false;
					
				}
				// Check for difficulty change
				else if (dataMP->settings.isChangeDif) {
					dataMP->settings.changeDifficulty();
					dataMP->settings.isChangeDif = false;
					// Change difficulty via the datamanager when possible
				}
				// Check for input change
				else if (dataMP->settings.isChangeInput) {
					dataMP->settings.changeInput();
					dataMP->settings.isChangeInput = false;
					// Change the input via the datamanager when possible
					dataMP->mouseControl = !dataMP->mouseControl;
				}
				// If nothing else is selected then castle black is selected and the game state will start
				else {
					dataMP->settingsActive = false;
					dataMP->stateHandler.setState(StateHandler::States::GAME);
				}
				isCounting = false;
				timePassed = 0;
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