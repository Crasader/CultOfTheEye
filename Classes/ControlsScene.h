#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace ui;

class ControlsScene : public cocos2d::Scene
{

		Button* mButtonBack;
		Button* mButtonNext;
		Button* mButtonMenu;
		int mCurrentPage = 1;

public:

	bool init();

	void   changeToPage(int pageNumber);

	static ControlsScene* create()
	{
		ControlsScene *pRet = new(std::nothrow) ControlsScene();
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
