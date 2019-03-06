#pragma once

#include "cocos2d.h"
#include "Types.h"
#include <array>
#include <queue>

using namespace std;
using namespace cocos2d;

class Core;
class TowerManager;
class GameScene;

class UITower : public Node
{
public:
	UITower(eTowerType iTowerType);
	~UITower();
	
	void deactivate();
	void deactivate(Vec2 position);
	void activate(Vec2 position);
	void reset();

	void setVelocity(Vec2 velocity);
	Vec2 getVelocity() const;

	void setPosition(Vec2 pos);
	void setPositionGracefully(Vec2 pos);

	void update(float dt) override;

	//Geometry Based Functions
	array<Vec2, 4>	getBoundingPolygon();
	bool			containsPoint(Vec2 point);
	Vec2			findNearestPoint(Vec2 point);

	static UITower* create(eTowerType iTowerType)
	{
		UITower *pRet = new(std::nothrow) UITower(iTowerType);
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
	eTowerType		mCategory;
	Vec2			mCurrentPosition;
	Vec2			mVelocity;
	Sprite*			mSprite;
	PhysicsBody*	mPhysicsBody;
	Core*			mCore;
	TowerManager*	mTowerManager;
	GameScene*		mActiveScene;
	Quadrants		mRootQuadrant;
	Vec2			mRootPosition;

	bool			mCanDrag;
	bool			mReadyToUpdate;

	queue<Vec2>		mPositionOverTime;
	uint8_t			mQueueSize;
	void clearPositionQueue();

	EventListenerTouchOneByOne* mTouchListener;

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
};

