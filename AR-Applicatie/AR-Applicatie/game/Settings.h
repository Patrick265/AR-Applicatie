#pragma once

#include <string>

/*
//	This class is used for storing and modification of settings
//	Author: Tim de Booij
*/

class Settings
{
public:
	// Booleans for mouse/marker checking in the settings screen
	bool isSettingsScreen;
	bool isChangeDif;
	bool isChangeInput;
	bool isGameObject;

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
	std::string getDifficultyString() const;
	std::string getInputString() const;

	// Change methods
	void changeDifficulty();
	void changeInput();

	//Constructor
	Settings();
};
