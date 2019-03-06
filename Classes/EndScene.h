#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"

using namespace cocos2d;
using namespace ui;

class EndScene : public cocos2d::Scene
{
	Button*		mButtonPlay;

	GameScene::EndGameOutcome mOutcome;
public:
	EndScene(GameScene::EndGameOutcome outcome);
	bool init();
	
	static EndScene* create(GameScene::EndGameOutcome outcome)
	{
		EndScene *pRet = new(std::nothrow) EndScene(outcome);
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

