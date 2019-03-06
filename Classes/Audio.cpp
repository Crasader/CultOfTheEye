#include "Types.h"
#include "Audio.h"

Audio* Audio::mInstance = nullptr;

Audio::Audio()
{
	if (!mInstance)
	{
		mInstance = this;
		mEngine = SimpleAudioEngine::getInstance();
		load();
	}
}

Audio* Audio::getInstance()
{
	return mInstance;
}

void Audio::playEffect(string audioFile)
{
	mEngine->playEffect(audioFile.c_str(), false);
}

void Audio::playMusic(string audioFile)
{
	mEngine->playBackgroundMusic(audioFile.c_str());
}

void Audio::load()
{	
	mEngine->preloadBackgroundMusic(FilePaths::Audio::Music::AndroidBackground.c_str());
	mEngine->preloadBackgroundMusic(FilePaths::Audio::Music::WinBackground.c_str());

	mEngine->preloadEffect(FilePaths::Audio::SFX::Monster::Hurt.c_str());
	mEngine->preloadEffect(FilePaths::Audio::SFX::Monster::Idle1.c_str());
	mEngine->preloadEffect(FilePaths::Audio::SFX::Monster::Idle2.c_str());
	mEngine->preloadEffect(FilePaths::Audio::SFX::Monster::Idle3.c_str());
	mEngine->preloadEffect(FilePaths::Audio::SFX::Monster::Idle4.c_str());

	mEngine->preloadEffect(FilePaths::Audio::SFX::Tower::Damaged.c_str());
	mEngine->preloadEffect(FilePaths::Audio::SFX::Tower::Destroyed.c_str());
	mEngine->preloadEffect(FilePaths::Audio::SFX::Tower::Shoot.c_str());
}
