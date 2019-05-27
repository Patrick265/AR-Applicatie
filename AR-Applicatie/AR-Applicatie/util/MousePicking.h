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
	bool isSettingsScreen;
	

public:
	/*
	ObjectsToCheck: The object the MousePicker has to check for
	height: The height of the window
	x: The x position of the mouse for initialization purpose
	y: The y position of the mouse for initialization purpose
	*/
	MousePicking(GameObject *objectsToCheck, int height, int x, int y);
	~MousePicking();
	/*
	The method which checks if the cursor position corresponds to the position of the object
	*/
	void searchObject(int cursorX, int cursorY);

	void checkSettings(int x, int y);

	/*
	The method is called from main to check if the position of the cursor has changed and updates the time which has passed since the last change
	height: The height of the window
	time: The time that has passed since the last time update was called
	*/
	void update(int cursorX, int cursorY, int height, float time);
	float getTimePassed();
};

#endif // !MOUSEPICKING