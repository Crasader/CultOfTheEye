#include "MonsterHead.h"
#include "MonsterManager.h"
#include "GameScene.h"
#include "Projectile.h"
#include "Gem.h"
#include "Types.h"
#include "RNG.h"
#include "PhysicsShapeCache.h"
#include <math.h>
using namespace std;

vector<vector<vector<PhysicsShapePolygon*>>> Head::mShapes;

Head* Head::create(int stage, int quadrant, Vec2 position)
{
	Head *pRet = new(std::nothrow) Head(stage, quadrant, position);
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

Head::Head(int stage, int quadrant, Vec2 position) : Monster(quadrant, position)
{
	Monster::mShapes = &mShapes;

	mEssenceDropRate = 100;
	mCurrentHealth = mMaxHealth = 3;

	mAttackSpeed = 0;
	mIdleSpeed = 4;
	mAttackWithdrawSpeed = 2.5;
	mAttackRetreatSpeed = 1.5;

	mStage = stage;

	createSprite();
	createPhysicsBody();

	if (mQuadrant == 1 || mQuadrant == 3)
		mSprite->setRotation(60.64);
	else
		mSprite->setRotation(-60.64);

	changeAnimation(Idle);

	spawn();
	acquireTarget();
}

void Head::createSprite()
{	
	auto spritecache = SpriteFrameCache::getInstance();

	vector<pair<string, int>> animString =
	{
		make_pair(::Animations::Monster::Head::fIdle, 50),
		make_pair(::Animations::Monster::Head::fSpray, 50)
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

		Animation *anim = Animation::createWithSpriteFrames(animFrames, 1 / 24.f);
		mAnimation[i] = Animate::create(anim);
		mAnimation[i]->retain();
	}

	mSprite->setAnchorPoint(Vec2(0.5, 0));
	mSprite->setPosition(mOffScreenPosition);
	(mQuadrant < 2) ? mSprite->setScale(-1.5) : mSprite->setScale(1.5);

	addChild(mSprite);
	
	scheduleUpdate();
}

void Head::createPhysicsBody()
{
	mPhysicsBody = PhysicsBody::create();
	PhysicsShapeCache::getInstance()->createDynamicBodyWithName(FilePaths::Physics::Monster::Head::Strings[0].first, *mPhysicsBody);
	mSprite->addComponent(mPhysicsBody);
}


void Head::spawn()
{
	auto changeToIdle = CallFunc::create([this]()
	{
		//changeAnimation(Idle);
		this->schedule([this](float a) {this->attack(); }, mIdleSpeed, 0, 0, "Attack");
	});
	auto spawnSequence = Sequence::create(MoveTo::create(3, mBasePosition), changeToIdle, nullptr);

	spawnSequence->setTag(SpawnSequence);
	mSprite->runAction(spawnSequence);
	//changeAnimation(Idle, spawnSequence);	
}

void Head::attack()
{
	if (mTarget && mTarget->getReferenceCount() == 1)
	{
		mTarget->release();
		mTarget = nullptr;
		if (!acquireTarget())
			return;
	}
	if (mTarget != nullptr)
	{
		auto targetPosition = mTarget->findNearestPoint(mBasePosition);
		auto direction = (targetPosition - mBasePosition).getNormalized() * 250;

		mEmitter = CCParticleSystemQuad::create("Images/particle_spray.plist");
		mEmitter->setDuration(ParticleSystem::DURATION_INFINITY);
		mEmitter->setPosition(mSprite->getPosition());
		mEmitter->setRotation(mSprite->getRotation());
		addChild(mEmitter, 10);

		auto changeToIdle = CallFunc::create([this]()
		{
			changeAnimation(Idle);
			removeChild(mEmitter);
			this->schedule([this](float a) {this->attack(); }, mIdleSpeed, 0, 0, "Attack");
		});

		auto attackSequence = Sequence::create(mAnimation[Spray], changeToIdle, nullptr);

		changeAnimation(Spray, attackSequence);
	}
}