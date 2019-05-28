#pragma once
#include <string>
#include <iostream>

class Settings
{
public:
	// Booleans for mouse/marker checking in the settings screen
	bool isSettingsScreen;
	bool isChangeDif;
	bool isChangeInput;

	// Enums
	enum class Difficulty { Hard, Medium, Easy };
	enum class Input { Mouse, Marker };

	// Standard settings
	int resX;
	int resY;

	// Changable settings
	Difficulty currentDifficulty;
	Input currentInput;

	// Getter methods
	std::string getDifficultyString()const;
	std::string getInputString()const;

	// Change methods
	void changeDifficulty();
	void changeInput();

	//Constructor
	Settings();
};