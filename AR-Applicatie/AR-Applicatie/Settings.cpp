#include "game/Settings.h"

Settings::Settings() {
	resX = 1280;
	resY = 720;
	currentDifficulty = Difficulty::Medium;
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