#pragma once
#include "cocos2d.h"
#include <string>
using namespace std;
using namespace cocos2d;

class Tower;
class Projectile;

struct BallisticInfo
{
	Vec2	mDirection;
	Vec2	mPosition;
	float	mForce;
	Tower*	mTower;
	int		mDamage;
};

class Ballista : public Node
{
	const float mMaxRange = 100;
	const float mMinRange = 25;

	Vec2 mReferenceVector;
	float mReferenceAngle;

	EventListenerTouchOneByOne* mTouchListener;

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	Tower*	mTower;

	function<Projectile* (BallisticInfo)> mSlingAction;

	Sprite* mSlingSprite;
	string	mSlingFilename;

	Sprite* mMarkerSprite;	
	string	mMarkerFilename;

	Sprite* mProjectileSprite;
	string mProjectileFilename;

	int		mCoolDown;
	bool	mCanActivate;
	int		mDamage;

	Ballista* mThis;

public:	
	Ballista(Tower* node);
	~Ballista();

	void	activate();
	void	deactivate();
	bool	canActivate();

	void	setCoolDown(int cooldown);
	int		getCoolDown();

	void	setDamage(int dmg);
	int		getDamage();

	void	setSpriteFilenames(string sling, string slingBall, string projectile);
	void	setSlingAction(function<Projectile* (BallisticInfo)> action);
	
	static Ballista* create(Tower* node)
	{
		Ballista *pRet = new(std::nothrow) Ballista(node);
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

