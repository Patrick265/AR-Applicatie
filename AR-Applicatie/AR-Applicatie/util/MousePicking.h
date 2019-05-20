#ifndef MOUSEPICKING
#define MOUSEPICKING

#include "../objects/GameObject.h"
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <chrono>

class MousePicking {
private:
	int windowHeight;
	//The object the MousePicker has to check
	GameObject* objectsToCheck;
	//Variables for checking what the last position of the cursor was
	int lastX, lastY;
	//Variables for checking how long the cursor is on the same position
	float cursorCounter;
	float timePassed;
	//Variable which determines if the mousepicker counts how long the mouse is on the object
	bool isCounting;
	

public:
	MousePicking(GameObject *objectsToCheck, int height, int x, int y);
	void searchObject(int cursorX, int cursorY);
	void update(int cursorX, int cursorY, int height, float time);
	float getTimePassed();
};

#endif // !MOUSEPICKING