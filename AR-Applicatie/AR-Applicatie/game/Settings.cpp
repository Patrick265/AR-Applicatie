#include "Settings.h"

Settings::Settings() {
	resX = 1280;
	resY = 720;
	currentDifficulty = Difficulty::Medium;
	currentInput = Input::Mouse;
	isChangeDif = false;
	isChangeInput = false;
	isSettingsScreen = false;
}

std::string Settings::getDifficultyString()const
{
	switch (this->currentDifficulty) {
	case Difficulty::Easy:
		return "Easy  ";
	case Difficulty::Medium:
		return "Medium";
	case Difficulty::Hard:
		return "Hard  ";
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

std::string Settings::getInputString()const
{
	if (this->currentInput == Input::Marker) {
		return "Marker";
	}
	else {
		return "Mouse ";
	}
}

void Settings::changeInput() {
	if (this->currentInput == Input::Marker) {
		this->currentInput = Input::Mouse;
	}
	else {
		this->currentInput = Input::Marker;
	}
}