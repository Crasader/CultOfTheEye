#include "Monster.h"
#include "MonsterManager.h"
#include "GameScene.h"
#include "Projectile.h"
#include "Gem.h"
#include "Types.h"
#include "RNG.h"
#include "PhysicsShapeCache.h"
#include <math.h>
using namespace std;

void Monster::findOffScreenPosition()
{
	//TODO: CHANGE WITH SCREEN RESOLUTION
	float xAxis = ((mQuadrant == 1 || mQuadrant == 2) ? 1920.f : -1920.f);
	float yAxis = ((mQuadrant == 0 || mQuadrant == 1) ? 1080.f : -1080.f);

	float y = xAxis * mBasePosition.y / mBasePosition.x;
	float x = yAxis * mBasePosition.x / mBasePosition.y;
	x = max(min(x, 1920.f), -1920.f);
	y = max(min(y, 1080.f), -1080.f);
	mOffScreenPosition = Vec2(x, y);
}

int Monster::computeQuadrant()
{
	auto position = mSprite->getPosition();

	if (position.x >= 0 && position.y >= 0)
		return 1;
	else if(position.x >= 0 && position.y < 0)
		return 2;
	else if (position.x < 0 && position.y < 0)
		return 3;
	else if (position.x < 0 && position.y >= 0)
		return 0;
}

void Monster::createPhysicsBody()
{
	mPhysicsBody = PhysicsBody::createBox(mSprite->getBoundingBox().size, PhysicsMaterial(5.f, 1.0f, 0.0f));
	mPhysicsBody->setDynamic(true);
	mPhysicsBody->setContactTestBitmask(0xFFFFFFFF);
	mPhysicsBody->setRotationEnable(false);
	mSprite->addComponent(mPhysicsBody);
}

Monster::Monster(int quadrant, Vec2 position) : mBasePosition(position), mQuadrant(quadrant), mCanDestroy(false), mTarget(nullptr), mSprite(nullptr), mCurrentAnimationAction(nullptr)
{
	setName(ClassStrings::Monster);
	findOffScreenPosition();	
}

Monster::~Monster()
{
	if (mTarget)
	{
		mTarget->release();
		mTarget = nullptr;
	}	

	mPhysicsBody->removeAllShapes();

	for (auto anim : mAnimation)
	{
		anim->release();
	}
}

/**This function lets monster acquire a new Tower Target.
*This function is always called when a new monster is created and/or the previously acquired target is destroyed.
*This function can be called on other conditions depending on Monster Behaviour.*/
bool Monster::acquireTarget()
{
	auto towers = ((GameScene*)Director::getInstance()->getRunningScene())->getTowers(mQuadrant);

	if (towers.size() == 0)
	{
		retreat();
		return false;
	}

	//Check if any new towers have been added whose data doesn't exist in the AggroData map.
	for (auto itr = towers.begin(); itr != towers.end(); itr++)
	{
		mAggroData.insert(make_pair(*itr, 0));
	}

	//Iterate over map to find the maximum aggro and delete any towers which doesn't exist
	pair<Tower*, float> targetTurret = make_pair(nullptr, -1);
	for (auto itr = mAggroData.begin(); itr != mAggroData.end();)
	{
		if (find(towers.begin(), towers.end(), (*itr).first) == towers.end())
		{
			itr = mAggroData.erase(itr);
			continue;
		}
		else
		{
			//These two factors change depending on different Monster Behaviour.
			float factorA = 0.5f;
			float factorB = 0.5f;

			float aggro = (*itr).first->getBaseAggro();
			aggro += (*itr).first->getActiveAggro() * factorA;

			aggro += getTowerSpecificAggro((*itr).first->getCategory());
			aggro += (*itr).second * factorB;

			if (targetTurret.second < aggro)
			{
				targetTurret.first = (*itr).first;
				targetTurret.second = aggro;
			}

			itr++;
		}
	}

	mTarget = targetTurret.first;
	mTarget->retain();
	return true;
}

int Monster::getTowerSpecificAggro(int category)
{
	//These function return calculate and return different values depending on different Monster Behaviour
	int aggro = 0;

	if (category & TowerCategory::Offense)
		aggro += 2;
	if (category & TowerCategory::Defense)
		aggro += 1;

	return aggro;
}

int Monster::getQuadrant()
{
	return mQuadrant;
}

void Monster::handleProjectileCollision(Projectile* projectile)
{
	//Randomly spawn an essence
	if (RNG::getUniformInteger(0, 99) < mEssenceDropRate)
		spawnEssence();

	//If attacking, withdraw!
	if (mSprite->getNumberOfRunningActionsByTag(AttackSequence))
	{
		mSprite->stopAllActionsByTag(AttackSequence);
		withdraw();
	}
	
	//Update Aggro!
	auto itr = mAggroData.insert(make_pair(projectile->getTower(), 0));
	itr.first->second += projectile->getDamage();

	//Take damage.
	damage(projectile->getDamage());
}

void Monster::attack()
{
	if (mTarget && mTarget->getReferenceCount() == 1)
	{
		mTarget->release();
		mTarget = nullptr;
		acquireTarget();
	}
	if (mTarget != nullptr)
	{
		auto targetPosition = mTarget->findNearestPoint(mBasePosition);
		auto direction = (targetPosition - mBasePosition).getNormalized() * 250;

		auto attack = MoveTo::create(mAttackSpeed, direction + targetPosition);
		auto back = MoveTo::create(mAttackWithdrawSpeed, mBasePosition);
		auto nextAttack = CallFunc::create([this]() {this->schedule([this](float a) {this->attack(); }, mIdleSpeed, 0, 0, "Attack"); });

		auto attackSequence = Sequence::create(attack, back, nextAttack, nullptr);

		attackSequence->setTag(AttackSequence);
		mSprite->runAction(attackSequence);
	}
}

void Monster::withdraw()
{
	/*auto back = MoveTo::create(mAttackRetreatSpeed, mBasePosition);
	auto nextAttack = CallFunc::create([this]() {this->schedule([this](float a) {this->attack(); }, mIdleSpeed, 0, 0, "Attack");});

	auto withdrawSequence = Sequence::create(back, nextAttack, nullptr);

	withdrawSequence->setTag(WithdrawSequence);
	mSprite->runAction(withdrawSequence);*/
}

void Monster::retreat()
{
	unscheduleAllCallbacks();
	mBasePosition = mOffScreenPosition;
	
	auto escape = MoveTo::create(1, mBasePosition);
	auto dead = CallFunc::create([this]() {this->mCanDestroy = true;});
	auto retreatSequence = Sequence::create(escape, dead, nullptr);
	
	retreatSequence->setTag(RetreatSequence);
	mSprite->runAction(retreatSequence);
}

void Monster::spawn()
{
	auto spawnSequence = Sequence::create(MoveTo::create(3, mBasePosition), nullptr);

	spawnSequence->setTag(SpawnSequence);
	mSprite->runAction(spawnSequence);
}

int Monster::getHealthPercentage()
{
	return (mCurrentHealth * 100) / mMaxHealth;
}

bool Monster::canDestroy()
{
	return mCanDestroy;
}

void Monster::damage(int dmg)
{	
	if (mCurrentHealth > 0)
	{
		mCurrentHealth = max(0, mCurrentHealth - dmg);
		if (mCurrentHealth <= 0)
		{
			retreat();
		}
		else
		{
			withdraw();
		}
	}
}

void Monster::spawnEssence()
{
	((GameScene*)Director::getInstance()->getRunningScene())->spawnEssence(mSprite->getPosition());
}

void Monster::changeAnimation(int i, Action* mAction)
{
	mPhysicsBody->retain();
	mSprite->removeComponent(mPhysicsBody);

	mCurrentAnimation = i;

	if (mCurrentAnimationAction)
		mSprite->stopAction(mCurrentAnimationAction);

	if (mAction)
		mCurrentAnimationAction = mSprite->runAction(mAction);
	else
		mCurrentAnimationAction = mSprite->runAction(RepeatForever::create(mAnimation[mCurrentAnimation]));

	mSprite->setSpriteFrame(mAnimation[mCurrentAnimation]->getAnimation()->getFrames().front()->getSpriteFrame());

	mSprite->addComponent(mPhysicsBody);
	mPhysicsBody->addMass(999999);
	mPhysicsBody->setRotationOffset(0);
	mPhysicsBody->release();
}

void Monster::update(float dt)
{
	int mCurrentFrame = mAnimation[mCurrentAnimation]->getCurrentFrameIndex();
	if (mLastFrame != mCurrentFrame)
	{
		mLastFrame = mCurrentFrame;
		mPhysicsBody->removeAllShapes(false);
		for (auto s : (mShapes->operator[](mCurrentAnimation))[mCurrentFrame])
		{
			((MyPhysicsShapePolygon*)s)->setScale(mSprite->getScaleX(), mSprite->getScaleY());
			mPhysicsBody->addShape(s, false);
		}
	}
}
