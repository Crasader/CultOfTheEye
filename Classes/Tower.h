#pragma once

#include "cocos2d.h"
#include "BallistaController.h"
#include "Types.h"
#include "Projectile.h"
#include <array>
#include <functional>

using namespace std;
using namespace cocos2d;

class Tower : public Node
{
protected:

	//Each tower has a base aggro which is based on the type of tower, it's upgrade level etc. 
	//It doesn't factor any action based aggro.
	int		mBaseAggro;
	
	//Active Aggro is aggro derived solely from the actions of a tower. 
	//Hitting the vulnerable points, dealing the most damage in a quadrant etc. contributes to this factor.
	int		mActiveAggro;

	TowerCategory	mCategory;
	eTowerState		mState;
	bool			mUpgradeModeActive;
	eTowerState		mPrevState; 
	int				mCurrentHealth;
	int				mMaximumHealth;
	int				mLevel;
	Vec2			mPosition;
	
	vector<int>	mUpgradeCost;	

	Sprite*			mSprite;
	Sprite*			mShield;
	Sprite*			mUpgradeCostImage;
	//CCParticleSystemQuad*	mIndicator;
	ProgressTimer*	mProgressTimer;
	float			mUpgradeTime;
	string			mSpriteFilename;
	string			mPhysicsFile;
	PhysicsBody*	mPhysicsBody;		
	Ballista*		mBallista;
	int				mQuadrant;
	bool			mSpawnStraight;

	//for rotation
	float			mCurrentAngle = 0;
	float			mRootAngle = 0;
	float			mCurrentRadius = 0;

	int				mActDeactCtr;
	
	bool mCanTakeDamage = true;
	bool mBallistaActive = false;
	bool mIsBeingUpgraded = false;

	EventListenerTouchOneByOne* mTouchListener;
		
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

public:	
	void	superInit();	

	//Geometry Based Functions
	array<Vec2, 4> getBoundingPolygon();
	bool	containsPoint(Vec2 point);
	Vec2	findNearestPoint(Vec2 point);
	
	void update(float dt);
	pair<float, float> getSweep();
	
	bool			canUpgrade();
	virtual void	upgrade() = 0;

	void	enterCooldown();
	void	exitCooldown();
	void	shield();
	
	void	damage(float dmg);
	
	void rotate();
	void straighten();
	void moveTo(const Vec2& pos);

	void indicateUpgradeMode();
	void exitUpgradeMode();

	eCommonColors mCurrentColor;

	//Getters for Basic Attributes
	int	getBaseAggro()
	{
		return mBaseAggro;
	}
	int	getCategory()
	{
		return mCategory;
	}
	int	getActiveAggro()
	{
		return mActiveAggro;
	}
	Vec2 getPosition()
	{
		return mPosition;
	}
	int getLevel()
	{
		return mLevel;
	}
	PhysicsBody* getPhysicsBody()
	{
		return mPhysicsBody;
	}
};

class NormalTower : public Tower
{
	static int sBuildCost;
public:
	NormalTower(Vec2 position, int quadrant, bool straight);
	void		upgrade();
	static int	getBuildCost();

	static NormalTower* create(Vec2 position, int quadrant, bool straight)
	{
		NormalTower *pRet = new(std::nothrow) NormalTower(position, quadrant, straight);
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

class ShieldTower : public Tower
{
	static int sBuildCost;
public:
	ShieldTower(Vec2 position, int quadrant, bool straight);
	void		upgrade();
	static int	getBuildCost();

	static ShieldTower* create(Vec2 position, int quadrant, bool straight)
	{
		ShieldTower *pRet = new(std::nothrow) ShieldTower(position, quadrant, straight);
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
