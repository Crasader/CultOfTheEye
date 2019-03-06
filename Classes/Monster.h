#pragma once
#include "cocos2d.h"
#include "Tower.h"
#include <map>
using namespace cocos2d;

class Monster : public Node
{
public:
	enum ActionSequence 
	{ 
		AttackSequence, 
		WithdrawSequence, 
		RetreatSequence, 
		SpawnSequence 
	};

protected:	
	int		mEssenceDropRate;
	
	Sprite*			mSprite;
	PhysicsBody*	mPhysicsBody;

	Vec2	mBasePosition;
	Vec2	mOffScreenPosition;

	int		mCurrentHealth;
	uint8_t	mMaxHealth;

	float	mAttackSpeed;
	float	mIdleSpeed;
	float	mAttackRetreatSpeed;
	float	mAttackWithdrawSpeed;

	uint8_t	mQuadrant;

	bool	mCanDestroy;

	Tower*				mTarget;
	map<Tower*, int>	mAggroData;

	int		computeQuadrant();
	void	findOffScreenPosition();
	virtual void	createPhysicsBody();

	vector<Animate*> mAnimation;
	int mLastFrame;

	int		mCurrentAnimation;
	Action*	mCurrentAnimationAction;

	vector<vector<vector<PhysicsShapePolygon*>>>* mShapes;

public:
	Monster(int quadrant, Vec2 position);
	~Monster();

	bool acquireTarget();

	int getTowerSpecificAggro(int category);
	int getQuadrant();
	
	void changeAnimation(int i, Action* mAction = nullptr);

	virtual void attack();
	virtual void withdraw();
	virtual void retreat();
	virtual void spawn();

	void update(float dt);

	int	getHealthPercentage();
	bool canDestroy();

	void handleProjectileCollision(Projectile* projectile);
	void spawnEssence();
	void damage(int dmg);
};
