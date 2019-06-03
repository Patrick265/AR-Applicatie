#include "Settings.h"

/*
//	This class is used for storing and modification of settings
//	Author: Tim de Booij
*/

Settings::Settings() {
	this->resX = 1280;
	this->resY = 720;
	this->currentDifficulty = Difficulty::Medium;
	this->currentInput = Input::Mouse;
	this->isChangeDif = false;
	this->isChangeInput = false;
	this->isSettingsScreen = false;
	this->isGameObject = false;
}

// Returns the string of the current difficulty that is selected
std::string Settings::getDifficultyString()const
{
	switch (this->currentDifficulty) {
	case Difficulty::Easy:
		return "Easy  ";
	case Difficulty::Medium:
		return "Medium";
	case Difficulty::Hard:
		return "Hard  ";
	default: 
		return "Easy  ";
	}
}

// Checks the current difficulty and changes it to the next
void Settings::changeDifficulty()
{
	switch (this->currentDifficulty) {
	case Difficulty::Easy:
		this->currentDifficulty = Difficulty::Medium;
		break;
	case Difficulty::Medium:
		this->currentDifficulty = Difficulty::Hard;
		break;
	case Difficulty::Hard:
		this->currentDifficulty = Difficulty::Easy;
		break;
	}
}

// Returns the string of the current input that is selected
std::string Settings::getInputString()const
{
	if (this->currentInput == Input::Marker) {
		return "Marker";
	}
	else {
		return "Mouse ";
	}
}

// Checks the current input and changes it to the next
void Settings::changeInput() {
	if (this->currentInput == Input::Marker) {
		this->currentInput = Input::Mouse;
	}
	else {
		this->currentInput = Input::Marker;
	}
}