#pragma once

#include "cocos2d.h"
#include "Types.h"
#include <array>
#include <queue>

using namespace std;
using namespace cocos2d;

class UITower;

class UIButton : public Node
{
public:
	UIButton(eTowerType iTowerType, Vec2 iPosition);
	~UIButton();

	void deactivate();
	void activate();

	void flashRed();
	void flashBlue();

	//Geometry Based Functions
	array<Vec2, 4>	getBoundingPolygon();
	bool			containsPoint(Vec2 point);
	Vec2			findNearestPoint(Vec2 point);

	static UIButton* create(eTowerType iTowerType, Vec2 iPosition)
	{
		UIButton *pRet = new(std::nothrow) UIButton(iTowerType, iPosition);
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
private:
	Sprite*			mSprite;
	eTowerType		mCategory;
	Vec2			mRootPosition;
	UITower*		mUITower;

	bool		mIsCreatingTower;

	EventListenerTouchOneByOne* mTouchListener;

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
};

