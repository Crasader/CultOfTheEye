#include "Tentacle.h"
#include "MonsterManager.h"
#include "GameScene.h"
#include "Projectile.h"
#include "Gem.h"
#include "Types.h"
#include "RNG.h"
#include "PhysicsShapeCache.h"
#include <math.h>
using namespace std;

vector<vector<vector<PhysicsShapePolygon*>>> Tentacle::mShapes;

Tentacle* Tentacle::create(int stage, int quadrant, Vec2 position)
{
	Tentacle *pRet = new(std::nothrow) Tentacle(stage, quadrant, position);
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

Tentacle::Tentacle(int stage, int quadrant, Vec2 position) : Monster(quadrant, position)
{
	Monster::mShapes = &mShapes;

	mEssenceDropRate = 50;
	mCurrentHealth = mMaxHealth = 4;

	mAttackSpeed = 1.5;
	mIdleSpeed = 2;
	mAttackWithdrawSpeed = 2.5;
	mAttackRetreatSpeed = 1.5;

	mStage = stage;

	createSprite();
	createPhysicsBody();

	

	spawn();

	acquireTarget();
}

void Tentacle::createSprite()
{
	auto spritecache = SpriteFrameCache::getInstance();

	vector<pair<string, int>> animString = 
	{
		make_pair(::Animations::Monster::Tentacle::fIdle, 40),
		make_pair(::Animations::Monster::Tentacle::fAttack, 40),
		make_pair(::Animations::Monster::Tentacle::fDamaged, 40),
		make_pair(::Animations::Monster::Tentacle::fRetreat, 33),
		make_pair(::Animations::Monster::Tentacle::fRetreat, 33)
	};

	mAnimation.resize(Animations::Count);
	for (int i = 0; i < Animations::Count; i++)
	{
		Vector<SpriteFrame*> animFrames;
		for (int j = 1; j <= animString[i].second; j++)
		{
			char str[100];
			sprintf(str, animString[i].first.c_str(), mStage, j);
			animFrames.pushBack(spritecache->getSpriteFrameByName(str));

			if (j == 1)
				mSprite = Sprite::createWithSpriteFrame(animFrames.front());
		}
		if (i == Spawn)
			animFrames.reverse();
		Animation *anim = Animation::createWithSpriteFrames(animFrames, 1 / 24.f);
		mAnimation[i] = Animate::create(anim);
		mAnimation[i]->retain();
	}

	mSprite->setAnchorPoint(Vec2(0.5, 0));
	mSprite->setPosition(mBasePosition);

	if (mQuadrant == 0 || mQuadrant == 1)
		mSprite->setScale(-1.75);
	else
		mSprite->setScale(1.75);
	if (mQuadrant == 1 || mQuadrant == 3)
		mSprite->setRotation(60.64);
	else
		mSprite->setRotation(-60.64);
			
	addChild(mSprite);

	scheduleUpdate();
}

void Tentacle::createPhysicsBody()
{
	mPhysicsBody = PhysicsBody::create();
	PhysicsShapeCache::getInstance()->createDynamicBodyWithName(FilePaths::Physics::Monster::Tentacle::Strings[0].first, *mPhysicsBody);
	mSprite->addComponent(mPhysicsBody);
}

void Tentacle::retreat()
{
	if (mCanDestroy)
		return;

	unscheduleAllCallbacks();
	scheduleUpdate();

	auto makeHimDie = CallFunc::create([this]() {mCanDestroy = true; });
	
	auto retreatSequence = Sequence::create(mAnimation[Retreat], makeHimDie, nullptr);

	changeAnimation(Retreat, retreatSequence);
}

void Tentacle::spawn()
{
	auto changeToIdle = CallFunc::create([this]()
	{
		idle();
	});

	auto spawnSequence = Sequence::create(mAnimation[Spawn], changeToIdle, nullptr);

	changeAnimation(Spawn, spawnSequence);
}

void Tentacle::withdraw()
{
	auto changeToIdle = CallFunc::create([this]()
	{
		idle();
	});

	auto damageSequence = Sequence::create(mAnimation[Damaged], changeToIdle, nullptr);

	changeAnimation(Damaged, damageSequence);
}

void Tentacle::idle()
{
	auto idle = Repeat::create(mAnimation[Idle], 2);

	auto reAttack = CallFunc::create([this]() 
	{
		attack(); 
	});

	changeAnimation(Idle, Sequence::create(idle, reAttack, nullptr));
}

void Tentacle::attack()
{
	if (mTarget && mTarget->getReferenceCount() == 1)
	{
		mTarget->release();
		mTarget = nullptr;
		if (!acquireTarget())
		{
			damage(500);
		}
	}
	if (mTarget != nullptr)
	{
		auto v1 = mTarget->getPosition(); //findNearestPoint(mBasePosition);
		auto v2 = Vec2(0, 1080);

		if (mQuadrant < 2)
			v2.y = -1080;

		if (mQuadrant == 0)			v1 -= Vec2(-1920, 1080);
		else if (mQuadrant == 1)	v1 -= Vec2(1920, 1080);
		else if (mQuadrant == 2)	v1 -= Vec2(1920, -1080);
		else if (mQuadrant == 3)	v1 -= Vec2(-1920, -1080);

		float defaultAngle = mSprite->getRotation();
		float targetAngle = CC_RADIANS_TO_DEGREES(atan2f(v1.x * v2.y - v1.y * v2.x, v1.x * v2.x + v1.y * v2.y));
		mSprite->setRotation(targetAngle);
		
		auto changeToIdle = CallFunc::create([this, defaultAngle]()
		{
			mSprite->setRotation(defaultAngle);
			idle();
		});

		auto attackSequence = Sequence::create(mAnimation[Attack], changeToIdle, nullptr);

		changeAnimation(Attack, attackSequence);
	}
}

