#pragma once
#include <string>
#include <iostream>

class Settings
{
public:
	int resX;
	int resY;
	enum class Difficulty { Hard, Medium, Easy};
	Difficulty currentDifficulty;
	std::string getDifficultyString()const;
	void changeDifficulty();
	Settings();
};