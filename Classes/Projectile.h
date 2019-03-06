#pragma once
#include "cocos2d.h"
#include "BallistaController.h"
#include <string>
using namespace std;
using namespace cocos2d;

class Tower;
class Core;

class Projectile : public Node
{
protected:
	Sprite*		mSprite;
	string		mSpriteFilename;

	BallisticInfo	mBallisticInfo;
	PhysicsBody*	mPhysicsBody;

	CCParticleSystemQuad* mEmitter;

	//Used in update to call some core functions
	Core* mCoreReference;

	int mImpulse;
	const int mGravity;

	bool mHasGoneStatic;

	void disintegrate();

public:
	Projectile(const int impulse, const int gravity);
	void superInit();

	void	update(float dt);
	int		getDamage();
	Tower*	getTower();
	void	setImpulse(int impulse);

	virtual void handleCollision(Node* node);
};

class NormalProjectile : public Projectile
{
public:
	NormalProjectile(BallisticInfo info);
	static NormalProjectile* create(BallisticInfo info)
	{
		NormalProjectile *pRet = new(std::nothrow) NormalProjectile(info);
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

class ShieldProjectile : public Projectile
{
public:
	ShieldProjectile(BallisticInfo info);
	static ShieldProjectile* create(BallisticInfo info)
	{
		ShieldProjectile *pRet = new(std::nothrow) ShieldProjectile(info);
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
	void handleCollision(Node* node);
};

