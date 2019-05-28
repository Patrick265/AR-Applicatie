#include "SoundManager.h"

SoundManager::SoundManager()
: filePath("Resources/Music/"), type("wav"), engine(irrklang::createIrrKlangDevice())
{
	sounds[Sound::BACKGROUND] = "Background";
	sounds[Sound::CLIMBING] = "Climbing";
	sounds[Sound::DEATH] = "Death";
	sounds[Sound::HIT] = "Hit";
	sounds[Sound::LOSS] = "Loss";
	sounds[Sound::WIN] = "Win";
	sounds[Sound::THROW] = "Throw";

	for (auto && sound : sounds)
		sound.second = filePath + sound.second + "." + type;

	setVolume(0.2f);
	playSound(Sound::BACKGROUND, true);
}

SoundManager::~SoundManager()
{
	engine->drop();
};

void SoundManager::playSound(const Sound sound, const bool loop)
{
	engine->play2D(sounds[sound].c_str(), loop);
}

void SoundManager::stopSounds() const
{
	engine->stopAllSounds();
}

void SoundManager::setVolume(const float volume) const
{
	engine->setSoundVolume(volume);
}
