#include "MonsterBody.h"
#include "MonsterManager.h"
#include "GameScene.h"
#include "Projectile.h"
#include "Gem.h"
#include "Types.h"
#include "RNG.h"
#include "PhysicsShapeCache.h"
#include <math.h>
using namespace std;

vector<vector<vector<PhysicsShapePolygon*>>> Body::mShapes;

Body* Body::create(int quadrant, Vec2 position)
{
	Body *pRet = new(std::nothrow) Body(quadrant, position);
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

Body::Body(int quadrant, Vec2 position) : Monster(quadrant, position)
{
	Monster::mShapes = &mShapes;

	mEssenceDropRate = 25;
	mCurrentHealth = mMaxHealth = 5000;
	
	mAttackSpeed = 0;
	mIdleSpeed = 4;
	mAttackWithdrawSpeed = 2.5;
	mAttackRetreatSpeed = 1.5;

	mTargetQuadrant = -2;
	mDirection = RNG::getUniformInteger(0, 1);

	createSprite();
	createPhysicsBody();
	
	changeAnimation(Idle);
	spawn();
	
	scheduleUpdate();
}

void Body::createSprite()
{
	auto spritecache = SpriteFrameCache::getInstance();

	vector<pair<string, int>> animString =
	{
		make_pair(::Animations::Monster::Swim::fIdle, 68),
	};

	mAnimation.resize(Animations::Count);
	for (int i = 0; i < Animations::Count; i++)
	{
		Vector<SpriteFrame*> animFrames;
		for (int j = 1; j <= animString[i].second; j++)
		{
			char str[100];
			sprintf(str, animString[i].first.c_str(), 1, j);
			animFrames.pushBack(spritecache->getSpriteFrameByName(str));

			if (j == 1)
				mSprite = Sprite::createWithSpriteFrame(animFrames.front());
		}

		Animation *anim = Animation::createWithSpriteFrames(animFrames, 1 / 30.f);
		mAnimation[i] = Animate::create(anim);
		mAnimation[i]->retain();
	}

	mSprite->setAnchorPoint(Vec2(0.75, 0.5));
	mSprite->setPosition(mOffScreenPosition);

	mSprite->setScale(2);
	//0 direction = positive scale
	if (mQuadrant == 0 || mQuadrant == 3)
		mSprite->setScaleX(2);
	else
		mSprite->setScaleX(-2);

	addChild(mSprite);

	scheduleUpdate();
}

void Body::createPhysicsBody()
{
	mPhysicsBody = PhysicsBody::create();
	PhysicsShapeCache::getInstance()->createDynamicBodyWithName(FilePaths::Physics::Monster::Swim::Strings[0].first, *mPhysicsBody);
	mSprite->addComponent(mPhysicsBody);
}

void Body::update(float dt)
{
	int mCurrentFrame = mAnimation[mCurrentAnimation]->getCurrentFrameIndex();
	if (mLastFrame != mCurrentFrame)
	{
		mLastFrame = mCurrentFrame;
		mPhysicsBody->removeAllShapes(false);
		for (auto s : (Monster::mShapes->operator[](mCurrentAnimation))[mCurrentFrame])
		{
			((MyPhysicsShapePolygon*)s)->setScale(mSprite->getScaleX(), mSprite->getScaleY());
			mPhysicsBody->addShape(s, false);
		}
	}

	if (computeQuadrant() == (mTargetQuadrant) % 4)
	{
		mSprite->setRotation(90 - CC_RADIANS_TO_DEGREES(mSprite->getPosition().getAngle()));
		mSprite->setAnchorPoint(mSprite->getAnchorPoint() * (1.f + dt));
		/*if (mPhysicsBody)
		{
			mSprite->removeComponent(mPhysicsBody);
			mPhysicsBody = nullptr;
		}*/		
		retreat();
	}
	else if (!mSprite->getNumberOfRunningActionsByTag(SpawnSequence))
	{
		mSprite->setRotation(90 - CC_RADIANS_TO_DEGREES(mSprite->getPosition().getAngle()));

		//For Uniform Elliptical Motion
		auto angle = mSprite->getPosition().getAngle();
		auto position = Vec2(450 * cos(angle), 1000 * sin(angle));
		auto direction = -position.getNormalized();
		auto winding = ((mDirection == 0) ? CC_DEGREES_TO_RADIANS(90) : CC_DEGREES_TO_RADIANS(-90));
		auto perpendicular = direction.rotateByAngle(Vec2(0, 0), winding);

		mPhysicsBody->setVelocity(perpendicular * 600);
	}	
}

void Body::setTargetQuadrant(int target)
{
	mTargetQuadrant = target;

	if (target == 0)
		mOffScreenPosition = Vec2(-1920, 1100);
	else if (target == 1)
		mOffScreenPosition = Vec2(1950, 1080);
	else if (target == 2)
		mOffScreenPosition = Vec2(1920, -1100);
	else if (target == 3)
		mOffScreenPosition = Vec2(-1950, -1080);
}

void Body::spawn()
{
	auto adjust = CallFunc::create([this]() 
	{
		mPhysicsBody->removeAllShapes();
		mSprite->removeComponent(mPhysicsBody);
		
		mSprite->setAnchorPoint(Vec2(0.5, 0.5));
		mSprite->setRotation(90 - CC_RADIANS_TO_DEGREES(mSprite->getPosition().getAngle()));
		
		if (mDirection == 0)
			mSprite->setScaleX(2);
		else
			mSprite->setScaleX(-2);

		mPhysicsBody = PhysicsBody::create();
		PhysicsShapeCache::getInstance()->createDynamicBodyWithName(FilePaths::Physics::Monster::Swim::Strings[0].first, *mPhysicsBody);
		mSprite->addComponent(mPhysicsBody);
		mPhysicsBody->addMass(999999);
		mPhysicsBody->setRotationOffset(0);
	});
	auto spawnSequence = Sequence::create(MoveTo::create(3, mBasePosition), adjust, nullptr);
	spawnSequence->setTag(SpawnSequence);
	mSprite->runAction(spawnSequence);
}

