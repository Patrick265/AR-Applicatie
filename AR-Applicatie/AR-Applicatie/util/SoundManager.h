#pragma once
#include <irrKlang.h>
#include <map>
#include <string>

class SoundManager
{
	const std::string filePath;
	const std::string type;
	irrklang::ISoundEngine *engine;
public:
	enum class Sound
	{
		BACKGROUND,
		CLIMBING,
		DEATH,
		HIT,
		LOSS,
		WIN,
		THROW
	};

	SoundManager();
	~SoundManager();

	void playSound(Sound sound, bool loop);
	void stopSounds() const;
	void setVolume(float volume) const;
private:
	std::map<Sound, std::string> sounds;
};
