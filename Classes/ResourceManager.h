#pragma once
#include "cocos2d.h"
#include "Types.h"
using namespace cocos2d;

class MonsterHealthUI : public Node
{
public:
	enum Overlay { White, Black };

	Sprite*		mMonsterHealth;
	DrawNode*	mOverlay;
	Vec2		mOverlayRectangle[2][4];

	float mX[2] = { 917.f, 917.f };
	const float mY[2] = { 95.f, 95.f };
	const float	mWidth[2] = { 248.f, 248.f * 3 };
	const float	mHeight[2] = { 95.f, 95.f };

	static MonsterHealthUI* create()
	{
		MonsterHealthUI *pRet = new(std::nothrow) MonsterHealthUI();
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

	bool init()
	{
		mMonsterHealth = Sprite::create(FilePaths::UI::MonsterHPBar);
		mMonsterHealth->setScale(0.4);
		mMonsterHealth->setAnchorPoint(Vec2(0.5, 1));
		addChild(mMonsterHealth);

		mOverlay = DrawNode::create();
		mMonsterHealth->addChild(mOverlay);

		refresh();

		return true;
	}

	MonsterHealthUI()
	{
		for (int i = 0; i < 2; i++)
		{
			mOverlayRectangle[i][0] = Vec2(mX[i], mY[i]);
			mOverlayRectangle[i][1] = Vec2(mX[i], mY[i]);
			mOverlayRectangle[i][2] = Vec2(mX[i], mY[i] + mHeight[i]);
			mOverlayRectangle[i][3] = Vec2(mX[i], mY[i] + mHeight[i]);
		}
	}

	void expand(int overlay, float ratio)
	{
		mOverlayRectangle[overlay][0].x = mX[overlay] - ((1 - ratio) * mWidth[overlay]);
		mOverlayRectangle[overlay][3].x = mX[overlay] - ((1 - ratio) * mWidth[overlay]);
		refresh();
	}

	void advance(int overlay)
	{
		mX[overlay] -= mWidth[overlay];
		for (auto& i : mOverlayRectangle[overlay])
		{
			i.x = mX[overlay];
		}
		refresh();
	}

	void reset(int overlay)
	{
		mOverlayRectangle[overlay][0].x = mOverlayRectangle[overlay][1].x;
		mOverlayRectangle[overlay][3].x = mOverlayRectangle[overlay][1].x;
		refresh();
	}

	void refresh()
	{
		mOverlay->clear();
		mOverlay->drawPolygon(mOverlayRectangle[0], 4, Color4F::WHITE, 0, Color4F(0, 0, 0, 1));
		mOverlay->drawPolygon(mOverlayRectangle[1], 4, Color4F::BLACK, 0, Color4F(0, 0, 0, 1));
	}
};

class PauseButtonUI : public Node
{

};

class GameUI : public Node
{
	int		mAmount;
	Sprite*	mSprite;
	Label*	mResourceMeter;
		
	MonsterHealthUI*	mMonsterHealth;
public:
	GameUI();
	static GameUI* create()
	{
		GameUI *pRet = new(std::nothrow) GameUI();
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

	int getAmount();
	void update(float time);
	void increase(int amount);
	bool decrease(int amount);

	void showTowerUI();

	void resetTemporaryDamage();
	void indicateTemporaryDamage(float currentHealthPercent);
	void indicatePermanentDamage(float currentHealthPercent);
};