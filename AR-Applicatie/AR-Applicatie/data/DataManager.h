#pragma once

#include "../vision/markerdetection.h"
#include "../states/StateHandler.h"
#include "../game/Settings.h"
#include "../util/SoundManager.h"

class DataManager
{
	// Private singleton constructor
	DataManager();

	// Private helper methods
	void moveCamera(float angle, float fac);
	void initRigParts();
	void initGameLogicModels();
	void initWorldMapModels();

public:
	// State handler
	StateHandler stateHandler;

	// Settings
	Settings settings;

	// Screen dimensions
	int width;
	int height;

	// Mouse position
	markerdetection::Point2D mousePos;
	markerdetection m;
	float scaleLoading;

	// Key positions (true is pressed)
	std::vector<bool> keys;

	// Whether OpenCV control is enabled or not
	bool mouseControl;
	bool justMovedMouse = false;
	bool settingsActive = false;

	// Storage for all game meshes and textures
	std::map<std::string, Graphics::mesh> meshes;
	std::map<std::string, uint16_t> textures;

	std::vector<std::string> weaponList;
	std::map<std::string, Math::vec3d> weaponMap;
	std::string currentWeapon = "log";
	int currentScore = 0;
	bool newHighScore = false;
	void determineNextWeapon();

	// SoundManager
	SoundManager soundManager;

	int cursorId;
	int loadingId;
	int backgroundTextId;
	int backgroundImgId;
	int fontTextId;
	int settingsId;

	// Camera struct and instance (rendering position)
	struct Camera
	{
		float
			posX = 0,
			posY = 15,
			posZ = 22,
			rotX = 20,
			rotY = 0;
	} camera;

	// Returns the singleton of this data class
	static DataManager &getInstance()
	{
		static DataManager instance;
		return instance;
	}

	// Make sure there is no public constructor and copy
	DataManager(DataManager const &) = delete;
	void operator=(DataManager const &) = delete;

	// Glut callbacks
	void onReshapeData(int w, int h);
	void onKeyData(unsigned char keyId, int x, int y);
	void onKeyUpData(unsigned char keyId);
	void onMotionData(int x, int y);

	// Helper methods
	void standardRenderOperations() const;
	void displayInfo() const;
	void updateCamera(float deltaTime);
	void displaySettings() const;

	// Init methods
	void initGlut(int argc, char** argv, void(*onIdle)(), void(*onDisplay)()) const;
	void initResources();

	void setOrtho() const;
	void drawBackgroundScreen() const;
	void drawScreenText() const;

	static void drawDefaultText(int x, int y, std::string string, void * font, float width, float height);

	int retrieveHighscore();
	void writeHighscore(int score);
};
