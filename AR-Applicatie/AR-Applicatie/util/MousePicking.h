#ifndef MOUSEPICKING
#define MOUSEPICKING

#include "../objects/GameObject.h"
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <chrono>

class MousePicking {
private:
	//Variables for checking what the last position of the cursor was
	int lastX, lastY;
	//Variable for checking how long the cursor is on the same position
	float cursorCounter;
	GameObject* objectsToCheck;
	int windowHeight;
	bool isCounting;
	float timePassed;

public:
	MousePicking(GameObject *objectsToCheck, int height, int x, int y);
	void searchObject(int cursorX, int cursorY);
	void update(int cursorX, int cursorY, int height, float time);
};

#endif // !MOUSEPICKING