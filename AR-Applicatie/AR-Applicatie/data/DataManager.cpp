#include "DataManager.h"
#include <string>
#include <GL/freeglut.h>
#include "../util/TextureHandler.h"
#include "../util/ObjLoader.h"
#include "../states/WorldMapState.h"
#include <corecrt_math_defines.h>
#include <ctime>
#include <fstream>

extern float deltaTime;
extern float lastFrameTime;

DataManager::DataManager()
	: width(1280), height(720), mousePos(), scaleLoading(), mouseControl(true), cursorId(), loadingId(0)
{
	keys.resize(255);
}

/*
 * GLUT CALLBACKS
 */

 // OnKey
void DataManager::onKeyData(unsigned char keyId, int x, int y)
{
	if (keyId == VK_ESCAPE)
		exit(1);
	if (keyId == 'c') {
		mouseControl = !mouseControl;
		settings.changeInput();
	}
	keys[keyId] = true;
}
static void onKey(unsigned char keyId, int x, int y)
{
	DataManager::getInstance().onKeyData(keyId, x, y);
}

// OnKeyUp
void DataManager::onKeyUpData(unsigned char keyId)
{
	keys[keyId] = false;
}
static void onKeyUp(unsigned char keyId, int, int)
{
	DataManager::getInstance().onKeyUpData(keyId);
}

// OnMotion
static void onMousePassiveMotion(int x, int y)
{
	DataManager::getInstance().onMotionData(x, y);
}
static void onMotion(int x, int y)
{
	onMousePassiveMotion(x, y);
}
void DataManager::onMotionData(int x, int y)
{
	if (mouseControl)
		mousePos = { float(x), float(y) };
	else if (!mouseControl)
	{
		const auto normalized = m.getCoordinates();
		mousePos = { normalized.x * width, normalized.y * height };
	}
}

// OnReshape
void DataManager::onReshapeData(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
}
static void onReshape(int w, int h)
{
	DataManager::getInstance().onReshapeData(w, h);
}

/*
 * HELPER FUNCTIONS
 */

void DataManager::standardRenderOperations() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Change to camera position
	glRotatef(camera.rotX, 1, 0, 0);
	glRotatef(camera.rotY, 0, 1, 0);
	glTranslatef(-camera.posX, -camera.posZ, -camera.posY);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
}

void DataManager::displayInfo() const
{
	if (dynamic_cast<WorldMapState*>(&stateHandler.getState()))
	{
		std::string text = "->SELECT SETTINGS<-";

		const auto xPos = 20;
		auto yPos = 30;
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glClearDepth(GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, width, height, 0.0, 0.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRasterPos2f(static_cast<GLfloat>(xPos), static_cast<GLfloat>(yPos));
		glColor3f(1, 1, 1);
		const auto len = text.length();
		for (auto i = 0; i < len; i++)
		{
			if (text[i] == '\n')
			{
				yPos += 20;
				glRasterPos2f(static_cast<GLfloat>(xPos), static_cast<GLfloat>(yPos));
				continue;
			}
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
		}
	}
	if (settingsActive)
		displaySettings();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, this->loadingId);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); glVertex2f(mousePos.x - scaleLoading, mousePos.y - scaleLoading);
	glTexCoord2f(0, 1); glVertex2f(mousePos.x - scaleLoading, mousePos.y + scaleLoading);
	glTexCoord2f(1, 1); glVertex2f(mousePos.x + scaleLoading, mousePos.y + scaleLoading);
	glTexCoord2f(1, 0); glVertex2f(mousePos.x + scaleLoading, mousePos.y - scaleLoading);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, cursorId);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); glVertex2f(mousePos.x - 25, mousePos.y - 25);
	glTexCoord2f(0, 1); glVertex2f(mousePos.x - 25, mousePos.y + 25);
	glTexCoord2f(1, 1); glVertex2f(mousePos.x + 25, mousePos.y + 25);
	glTexCoord2f(1, 0); glVertex2f(mousePos.x + 25, mousePos.y - 25);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void DataManager::updateCamera()
{
	const float speed = 6;
	if (keys[int('a')]) moveCamera(0, deltaTime * speed);
	if (keys[int('d')]) moveCamera(180, deltaTime * speed);
	if (keys[int('w')]) moveCamera(90, deltaTime * speed);
	if (keys[int('s')]) moveCamera(270, deltaTime * speed);
	if (keys[int('e')]) camera.posZ += deltaTime * speed;
	if (keys[int('q')]) camera.posZ -= deltaTime * speed;
}

void DataManager::moveCamera(const float angle, const float fac)
{
	camera.posX -= static_cast<float>(cos((camera.rotY + angle) / 180 * M_PI)) * fac;
	camera.posY -= static_cast<float>(sin((camera.rotY + angle) / 180 * M_PI)) * fac;
}

/*
 * INITIALIZATION
 */

void DataManager::initGlut(int argc, char** argv, void(*onIdle)(), void(*onDisplay)()) const
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);
	glutInitWindowSize(width, height);
	glutCreateWindow("Game Of Thrones: AR");

	glutIdleFunc(onIdle);
	glutDisplayFunc(onDisplay);
	glutKeyboardFunc(onKey);
	glutKeyboardUpFunc(onKeyUp);
	glutPassiveMotionFunc(onMousePassiveMotion);
	glutMotionFunc(onMotion);
	glutReshapeFunc(onReshape);

	lastFrameTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
}

void DataManager::initResources()
{
	//To prevent the map from throwing an exception
	meshes["none"] = {};
	textures["none"] = -1;

	initRigParts();
	initGameLogicModels();
	initWorldMapModels();

	//Cursor image
	cursorId = TextureHandler::addTexture("Resources/Cursor/16x16_cursor_icon.png", textures.size());
	//Loading ID
	this->loadingId = TextureHandler::addTexture("Resources/Cursor/16x16_cursor_icon_loading.png", textures.size());
	this->fontTextId = TextureHandler::addTexture("Resources/Font/ExportedTest.png", textures.size());
	this->backgroundTextId = TextureHandler::addTexture("Resources/Ending_Screen/ScreenAssetText.png", 1);
	this->backgroundImgId = TextureHandler::addTexture("Resources/Ending_Screen/ScreenAssetBackground.png", 0);
	this->settingsId = TextureHandler::addTexture("Resources/Settings/SettingsBackground.png", textures.size());

	glutSetCursor(GLUT_CURSOR_NONE);

	// Init random seed
	srand(static_cast<unsigned int>(time(nullptr)));
}

void DataManager::initRigParts()
{
	/*
		FEMALE ELF
	*/
	textures["elf_head"] = TextureHandler::addTexture("Resources/Rune/npc_head.png", textures.size());
	meshes["elf_head"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_head.obj");
	meshes["elf_neck"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_head_neck.obj");

	textures["elf_torso"] = TextureHandler::addTexture("Resources/Rune/npc_torso.png", textures.size());
	meshes["elf_torso"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_torso.obj");

	textures["elf_legs"] = TextureHandler::addTexture("Resources/Rune/npc_legs.png", textures.size());
	meshes["elf_lowerbod"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_skirt.obj");
	meshes["elf_ll_u"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_top_left.obj");
	meshes["elf_ll_l"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_bottom_left.obj");
	meshes["elf_rl_u"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_top_right.obj");
	meshes["elf_rl_l"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_bottom_right.obj");

	textures["elf_arms"] = TextureHandler::addTexture("Resources/Rune/npc_arms.png", textures.size());
	meshes["elf_la_u"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_left_top.obj");
	meshes["elf_la_l"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_left_bottom.obj");
	meshes["elf_ra_u"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_right_top.obj");
	meshes["elf_ra_l"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_right_bottom.obj");

	textures["elf_sack"] = TextureHandler::addTexture("Resources/Rigid_NPC/NPC_big_sack.png", textures.size());
	meshes["elf_sack"] = ObjLoader::loadObj("Resources/Rigid_NPC/NPC_big_sack.obj");

	textures["log"] = TextureHandler::addTexture("Resources/Weapons/weapon_log.png", textures.size());
	meshes["log"] = ObjLoader::loadObj("Resources/Weapons/weapon_log.obj");
	weaponMap["log"] = { 0.667523f,-0.030498f, 0.71092f };

	textures["weights"] = TextureHandler::addTexture("Resources/Weapons/weights.png", textures.size());
	meshes["weights"] = ObjLoader::loadObj("Resources/Weapons/weights.obj");
	weaponMap["weights"] = { 0.0f,0.0f,0.820919f };

	textures["godsword"] = TextureHandler::addTexture("Resources/Weapons/weapon_godsword.png", textures.size());
	meshes["godsword"] = ObjLoader::loadObj("Resources/Weapons/weapon_godsword.obj");
	weaponMap["godsword"] = { 0.0f,0.119117f,0.81344f };

	textures["cattleprod"] = TextureHandler::addTexture("Resources/Weapons/weapon_cattleprod.png", textures.size());
	meshes["cattleprod"] = ObjLoader::loadObj("Resources/Weapons/weapon_cattleprod.obj");
	weaponMap["cattleprod"] = { 0.661686f,0.014448f,0.698785f };

	textures["fish"] = TextureHandler::addTexture("Resources/Weapons/weapon_fish.png", textures.size());
	meshes["fish"] = ObjLoader::loadObj("Resources/Weapons/weapon_fish.obj");
	weaponMap["fish"] = { 0.811473f,0.14979f,-0.108852f };

	textures["weapon_elf"] = TextureHandler::addTexture("Resources/Weapons/weapon_elf.png", textures.size());
	meshes["weapon_elf"] = ObjLoader::loadObj("Resources/Weapons/weapon_elf.obj");

	/*
	GOBLIN
	*/
	textures["goblin_torso"] = TextureHandler::addTexture("Resources/Enemy/goblin_torso.png", textures.size());
	meshes["goblin_torso"] = ObjLoader::loadObj("Resources/Enemy/Goblin_torso.obj");

	textures["goblin_la_u"] = TextureHandler::addTexture("Resources/Enemy/goblin_arm_left_top.png", textures.size());
	meshes["goblin_la_u"] = ObjLoader::loadObj("Resources/Enemy/Goblin_arm_left_top.obj");

	textures["goblin_ra_u"] = TextureHandler::addTexture("Resources/Enemy/goblin_arm_top_right.png", textures.size());
	meshes["goblin_ra_u"] = ObjLoader::loadObj("Resources/Enemy/Goblin_arm_right_top.obj");

	textures["goblin_ll_u"] = TextureHandler::addTexture("Resources/Enemy/goblin_leg_left_top.png", textures.size());
	meshes["goblin_ll_u"] = ObjLoader::loadObj("Resources/Enemy/Goblin_leg_left_top.obj");
	meshes["goblin_rl_u"] = ObjLoader::loadObj("Resources/Enemy/Goblin_leg_right_top.obj");

	textures["goblin_la_l"] = TextureHandler::addTexture("Resources/Enemy/goblin_arm_left_bottom.png", textures.size());
	meshes["goblin_la_l"] = ObjLoader::loadObj("Resources/Enemy/Goblin_arm_left_bottom.obj");

	textures["goblin_ra_l"] = TextureHandler::addTexture("Resources/Enemy/goblin_arm_right_bottom.png", textures.size());
	meshes["goblin_ra_l"] = ObjLoader::loadObj("Resources/Enemy/Goblin_arm_right_bottom.obj");

	textures["goblin_ll_l"] = TextureHandler::addTexture("Resources/Enemy/goblin_leg_left_bottom.png", textures.size());
	meshes["goblin_ll_l"] = ObjLoader::loadObj("Resources/Enemy/Goblin_leg_left_bottom.obj");

	textures["goblin_rl_l"] = TextureHandler::addTexture("Resources/Enemy/goblin_leg_right_bottom.png", textures.size());
	meshes["goblin_rl_l"] = ObjLoader::loadObj("Resources/Enemy/Goblin_leg_right_bottom.obj");

	textures["icicle"] = TextureHandler::addTexture("Resources/Enemy/icicle.png", textures.size());
	meshes["icicle"] = ObjLoader::loadObj("Resources/Enemy/icicle.obj");
}

void DataManager::initGameLogicModels()
{
	textures["wall"] = TextureHandler::addTexture("Resources/Wall/Glacier.jpg", textures.size());
	meshes["wall"] = ObjLoader::loadObj("Resources/Wall/Glacier_bottom.obj");

	textures["wall_top"] = TextureHandler::addTexture("Resources/Wall/wood.jpg", textures.size());
	meshes["wall_top"] = ObjLoader::loadObj("Resources/Wall/Glacier_top.obj");

	textures["giant"] = TextureHandler::addTexture("Resources/Rune/giant.png", textures.size());
	meshes["giant"] = ObjLoader::loadObj("Resources/Rune/giant.obj");

	textures["skybox"] = TextureHandler::addTexture("Resources/Skybox/stars.jpg", textures.size());
	meshes["skybox"] = ObjLoader::loadObj("Resources/Skybox/skybox.obj");
	Graphics::inverseNormals(meshes["skybox"]);

	textures["packet"] = TextureHandler::addTexture("Resources/Pakketje/Pakketje.png", textures.size());
	meshes["packet"] = ObjLoader::loadObj("Resources/Pakketje/Pakketje.obj");

	textures["brick"] = TextureHandler::addTexture("Resources/Weapons/Brick.png", textures.size());
}

void DataManager::determineNextWeapon()
{
	const int number = rand() % weaponMap.size();
	auto pos = 0;
	for (auto const& x : weaponMap)
	{
		if (number == pos)
		{
			currentWeapon = x.first;
			break;
		}
		pos += 1;
	}
}

void DataManager::initWorldMapModels()
{
	meshes["map"] = ObjLoader::loadObj("Resources/Map/Map.obj");
	textures["map"] = TextureHandler::addTexture("Resources/Map/map.jpg", textures.size());
	textures["skybox_forest"] = TextureHandler::addTexture("Resources/Skybox/skybox2.jpg", textures.size());

	meshes["icon"] = ObjLoader::loadObj("Resources/Map/Castleblack icon.obj");
	meshes["wicon"] = ObjLoader::loadObj("Resources/Map/Winterfell.obj");
}

void DataManager::drawBackgroundScreen() const
{
	const auto centerX = static_cast<GLfloat>(this->width) / 2.0f;
	const auto centerY = static_cast<GLfloat>(this->height) / 2.0f;

	setOrtho();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, this->backgroundImgId);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); glVertex2f(centerX - centerX, centerY - centerY);
	glTexCoord2f(0, 1); glVertex2f(centerX - centerX, centerY + centerY);
	glTexCoord2f(1, 1); glVertex2f(centerX + centerX, centerY + centerY);
	glTexCoord2f(1, 0); glVertex2f(centerX + centerX, centerY - centerY);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, this->backgroundTextId);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); glVertex2f(centerX - centerX, centerY - centerY);
	glTexCoord2f(0, 1); glVertex2f(centerX - centerX, centerY + centerY);
	glTexCoord2f(1, 1); glVertex2f(centerX + centerX, centerY + centerY);
	glTexCoord2f(1, 0); glVertex2f(centerX + centerX, centerY - centerY);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void DataManager::setOrtho() const
{
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glClearDepth(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, this->width, this->height, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void DataManager::drawScreenText() const
{
	const auto characterList = glGenLists(256);
	glTranslated(0, 20, 0);
	for (auto i = 0; i < 256; i++)
	{
		const auto characterX = static_cast<float>(i % 32) / 32.0f;
		const auto characterY = static_cast<float>(i / 32) / 32.0f;
		const auto size = 1 / 32.0f;

		glEnable(GL_BLEND);

		glNewList(characterList + i, GL_COMPILE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, this->fontTextId);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glVertex2d(0, 0); glTexCoord2f(characterX, characterY);
		glVertex2i(0, 1);  glTexCoord2f(characterX, characterY + size);
		glVertex2i(1, 1); glTexCoord2f(characterX + size, characterY + size);
		glVertex2i(1, 0); glTexCoord2f(characterX + size, characterY);
		glEnd();
		glTranslated(2, 0, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glEndList();
	}

	auto size = 6;
	glScalef(100, 100, 100);
	glListBase(characterList);
	glCallLists(6, GL_UNSIGNED_BYTE, "Lekker");
}

void DataManager::drawDefaultText(int x, int y, std::string string, void *font, float width, float height)
{
	const auto cStr = string.c_str();
	auto length = static_cast<int>(strlen(cStr));

	glEnable(GL_LINE_SMOOTH);

	glPushMatrix();
	glTranslatef(static_cast<GLfloat>(x), static_cast<GLfloat>(y), 0);
	glScalef(width, -height, 0);
	glColor3f(1, 0, 0);
	for (auto str = cStr; *str; str++)
	{
		glutStrokeCharacter(font, *str);
		glutStrokeWidth(font, *str);
	}
	glColor3f(1, 1, 1);
	glPopMatrix();
	glDisable(GL_LINE_SMOOTH);
	glEnd();
}

int DataManager::retrieveHighscore()
{
	std::ifstream myFile;
	myFile.open("Resources\\Data\\Highscores.txt", std::ios::in);
	if (myFile.fail())
	{
		return 0;
	}
	std::string textLine;
	auto score = 0;
	if (getline(myFile, textLine))
	{
		score = std::stoi(textLine);
	}
	myFile.close();
	this->currentScore = score;
	return score;
}

void DataManager::writeHighscore(const int score)
{
	std::ofstream myFile;
	myFile.open("Resources\\Data\\Highscores.txt", std::ios::trunc);
	if (myFile.fail())
	{
		myFile.close();
		return;
	}
	this->currentScore = score;
	myFile << score;
	myFile.close();
}

void DataManager::displaySettings() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, settingsId);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); glVertex2f(1100, 150);
	glTexCoord2f(0, 1); glVertex2f(1100, 500);
	glTexCoord2f(1, 1); glVertex2f(-100, 500);
	glTexCoord2f(1, 0); glVertex2f(-100, 150);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	const auto dif = settings.getDifficultyString();
	const auto inp = settings.getInputString();
	auto settingsInfo =
		"                         ->RETURN<-"
		"\n\n\nWidth:        " + std::to_string(settings.resX) +
		"\nHeight:       " + std::to_string(settings.resY) +
		"\nDifficulty:   " + dif +
		"\nInput:        " + inp + "     ->CHANGE<-";

	glRasterPos2f(350, 260);
	const int len = settingsInfo.length();
	auto yPosSettings = 260;
	for (auto i = 0; i < len; i++)
	{
		if (settingsInfo[i] == '\n')
		{
			yPosSettings += 20;
			glRasterPos2f(350, yPosSettings);
			continue;
		}
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, settingsInfo[i]);
	}
}
