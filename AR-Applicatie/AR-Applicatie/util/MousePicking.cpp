#include "../util/MousePicking.h"
#include "../data/DataManager.h"

/*
//	This class is used for mouse picking
//	Author: Tim de Booij, Patrick de Jonge
*/

DataManager *dataMP = &DataManager::getInstance();

// Initialise values
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

// Checks the current cursor position for the area of the return button
void MousePicking::checkReturnButton(int x, int y) {
	if (dataMP->settingsActive == true) {
		if ((y >= 250 && y <= 260 && x >= 580 && x <= 660)) {
			this->isCounting = true;
			dataMP->settings.isSettingsScreen = true;
			dataMP->settings.isChangeInput = false;
		}
	}
}

// Checks the current cursor position for the area of the select settings button
void MousePicking::checkSettings(int x, int y) 
{
		if ((y >= 20 && y <= 30 && x >= 30 && x <= 190)) {
			this->isCounting = true;
			dataMP->settings.isSettingsScreen = true;
			dataMP->settings.isChangeInput = false;
		}
}

// Checks the current cursor position for the area of the change input value button
void MousePicking::checkChanges(int x, int y)
{
	if (y >= 370 && y <= 380 && x >= 580 && x <= 660 && dataMP->settingsActive) {
		this->isCounting = true;
		dataMP->settings.isChangeInput = true;
	}
}

// Checks the current cursor position for the area of the icons (Castle black for example)
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
	// Loop through all objects that need to be checked
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
			dataMP->settings.isSettingsScreen = false;
			if (i == 0) {
				dataMP->settings.isGameObject = true;
				dataMP->settings.isSettingsScreen = false;
			}
		}
	}

}

// The method is called from main to check if the position of the cursor has changed and updates the time which has passed since the last change
void MousePicking::update(int cursorX, int cursorY, int height, float time) 
{
	// This part checks if the cursor is a longer time on the same position
	if (!isCounting) {
		this->windowHeight = height;
		// Checks if the cursor position is close enough to the last cursor position
		if (abs(cursorX - lastX) <= 2 && abs(cursorY - lastY) <= 2) {
			cursorCounter += time;
			// Searching for objects
			if (cursorCounter >= 0.1f && timePassed <= 0.2f) {
				searchObject(cursorX, cursorY);
				checkSettings(cursorX, cursorY);
				checkReturnButton(cursorX, cursorY);
				// Only check the changes if the settings screen is active
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
	// In this part the program has found an object to start the selection counter for
	else {
		// Check if the position hasn't changed too much (if it has the selection will be aborted)
		if (abs(cursorX - lastX) <= 5 && abs(cursorY - lastY) <= 5) {
			timePassed += time;
			// Check if the maximum time for the selection has passed (after this the selection is made)
			if (timePassed >= 3.0f && timePassed <= 3.1f) {
				// Check for the setting screen to apear or disapear
				if (dataMP->settings.isSettingsScreen == true && (dataMP->settings.isChangeInput == false) && (dataMP->settings.isGameObject == false)) {
					if (dataMP->settingsActive == true) {
						dataMP->settingsActive = false;
						dataMP->settings.isSettingsScreen = false;
					}
					else {
						dataMP->settingsActive = true;
						dataMP->settings.isSettingsScreen = false;
					}						
					dataMP->settings.isSettingsScreen = false;
				}
				// Check for difficulty change
				else if (dataMP->settings.isChangeDif == true) {
					dataMP->settings.changeDifficulty();
					dataMP->settings.isChangeDif = false;
					// Change difficulty via the datamanager when possible
				}
				// Check for input change
				else if (dataMP->settings.isChangeInput == true) {
					dataMP->settings.changeInput();
					dataMP->settings.isChangeInput = false;
					// Change the input via the datamanager when possible
					dataMP->mouseControl = !dataMP->mouseControl;
				}
				// If nothing else is selected then castle black is selected and the game state will start
				else if(dataMP->settings.isGameObject == true) {
					dataMP->settingsActive = false;
					dataMP->settings.isGameObject = false;
					dataMP->stateHandler.setState(StateHandler::States::GAME);
				}
				// Reset variables
				isCounting = false;
				dataMP->settings.isGameObject = false;
				dataMP->settings.isChangeInput = false;
				dataMP->settings.isSettingsScreen = false;
				timePassed = 0;
			}
			
		}
		// The selection has been aborted and variables will be reset
		else {
			isCounting = false;
			this->timePassed = 0;
		}
		DataManager::getInstance().scaleLoading = timePassed * 10;
	}
	// Save the new cursor positions to the last ones
	lastX = cursorX;
	lastY = cursorY;
}

// The method returns the float which determines how far the selection progress is
float MousePicking::getTimePassed() {
	return timePassed;
}