#pragma once
#include <string>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace std;
using namespace cocos2d;

class Audio
{
	static Audio*		mInstance;
	SimpleAudioEngine*	mEngine;
	void	load();
public:
	Audio();	
	void	playEffect(string audioFile);
	void	playMusic(string audioFile);
	static Audio* getInstance();
};

