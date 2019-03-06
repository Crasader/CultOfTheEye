#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace ui;

class CreditsScene : public cocos2d::Scene
{
	Button* mButtonBack;
	Label* mTitle;

	Label* mPetra;
	Label* mSarah;
	Label* mKhadeidre;
	Label* mEmi;
	Label* mChris;
	Label* mIan;
	Label* mVarun;
	Label* mMiguel;
	Label* mXiaotong;

	Label* mProducer;
	Label* mEngineer1;
	Label* mEngineer2;
	Label* mEngineer3;
	Label* mArtist1;
	Label* mArtist2;
	Label* mArtist3;
	Label* mArtist4;
	Label* mArtist5;
	

public:

	bool init();

	static CreditsScene* create()
	{
		CreditsScene *pRet = new(std::nothrow) CreditsScene();
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}
};

