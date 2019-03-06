#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace ui;

class TitleScene : public cocos2d::Scene
{
	Label*	mTitle;
	Button* mButtonPlay;
	Button* mButtonControls;
	Button* mButtonCredits;

public:

	bool init();
	
	static TitleScene* create()
	{
		TitleScene *pRet = new(std::nothrow) TitleScene();
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

