#include "MonsterManager.h"
#include "GameScene.h"
#include "RNG.h"

MonsterManager::MonsterManager()
{
	scheduleUpdate();

	mMaxHealth = 3;
	mCurrentHealth = 3;

	mTargetQuadrant = RNG::getUniformInteger(0, 3);

	mSpawnRate = 4;

	mTentacle = nullptr;
	mHead = nullptr;
	mBody = nullptr;		
	
	this->schedule([this](float a) {this->initSwimPhase(); }, mSpawnRate, 0, 0, "swimPhase");
}

MonsterManager::~MonsterManager()
{
}

void MonsterManager::findRandomTargetQuadrant()
{
	vector<int> quadrant;
	GameScene* world = ((GameScene*)Director::getInstance()->getRunningScene());
	for (int i = 0; i < QuadrantCount; i++)
	{
		if (world->getTowers(i).size())
		{
			quadrant.push_back(i);
		}
	}

	if (quadrant.size() == 0)
		mTargetQuadrant = -1;
	else
		mTargetQuadrant = quadrant[RNG::getUniformInteger(0, quadrant.size() - 1)];
}

void MonsterManager::spawnTentacle()
{
	float distance = 1920;
	float x = ((mTargetQuadrant == 1 || mTargetQuadrant == 2) ? distance : -distance);
	float y = ((mTargetQuadrant == 0 || mTargetQuadrant == 1) ? distance / 1.7778 : -distance / 1.7778);
	
	int stage = mMaxHealth + 1 - mCurrentHealth;
	mTentacle = Tentacle::create(stage, mTargetQuadrant, Vec2(x, y));
	addChild(mTentacle);
}

void MonsterManager::spawnHead()
{
	//Get the tentacle quadrant.
	auto tQuad = mTentacle->getQuadrant();

	//Pick either one of the adjacent quadrants
	int quadrant = (tQuad + (-2 * RNG::getUniformInteger(0, 1) + 1)) % 4;
	quadrant = ((quadrant == -1) ? 3 : quadrant);
	
	float distance = 1920;
	float x = ((quadrant == 1 || quadrant == 2) ? distance : -distance);
	float y = ((quadrant == 0 || quadrant == 1) ? distance / 1.7778 : -distance / 1.7778);
	
	int stage = mMaxHealth + 1 - mCurrentHealth;
	mHead = Head::create(stage, quadrant, Vec2(x, y));
	addChild(mHead);
	mTargetQuadrant = quadrant;
}

void MonsterManager::spawnBody()
{
	float x = 1000;
	if (mTargetQuadrant == 0 || mTargetQuadrant == 3)
		x *= -1;

	float y = 450;
	if (mTargetQuadrant == 2 || mTargetQuadrant == 3)
		y *= -1;

	mBody = Body::create(mTargetQuadrant, Vec2(x * sqrt(2), y * sqrt(2)));
	addChild(mBody);
	this->schedule([this](float a) {this->initAttackPhase(); }, 10, 0, 0, "attackPhase");
}

void MonsterManager::initAttackPhase()
{
	findRandomTargetQuadrant();

	if (mTargetQuadrant == -1)
	{
		this->schedule([this](float a) {this->initAttackPhase(); }, 4, 0, 0, "attackPhase");
		return;
	}

	mCurrentPhase = AttackPhase;
	((Body*)mBody)->setTargetQuadrant(mTargetQuadrant);	
}

void MonsterManager::updateAttackPhase()
{
	if (mBody && mBody->canDestroy())
	{
		removeChild(mBody, true);
		mBody = nullptr;

		//Spawn the Tentacle
		spawnTentacle();

		//After Delay, Spawn the Head
		this->schedule([this](float a) {this->spawnHead(); }, mSpawnRate, 0, 0, "spawnHead");
	}
	else if (!mBody)
	{
		if (mHead && mHead->getHealthPercentage() == 0)
		{
			mCurrentHealth--;
			((GameScene*)Director::getInstance()->getRunningScene())->getGameUI()->indicatePermanentDamage(mCurrentHealth * 100 / mMaxHealth);
			if (mCurrentHealth <= 0)
				((GameScene*)Director::getInstance()->getRunningScene())->endGame(GameScene::EndGameOutcome::Victory);

			//Cancel swim phase scheduler, if any
			this->unschedule("swimPhase");

			//Immediately init Swim Phase
			initSwimPhase();
		}
		else if (mTentacle->getHealthPercentage() == 0 || (mHead && mHead->getHealthPercentage() <= 50))
		{
			//After Delay, init Swim Phase
			if (!mSwimPhaseSchedulerFlag)
			{
				mSwimPhaseSchedulerFlag = true;
				this->scheduleOnce([this](float a) {this->initSwimPhase(); }, mSpawnRate, "swimPhase");
			}			
		}

		if (mHead && mHead->getHealthPercentage() != 0)
			((GameScene*)Director::getInstance()->getRunningScene())->getGameUI()->indicateTemporaryDamage(mHead->getHealthPercentage());
	}
}

void MonsterManager::initSwimPhase()
{
	mCurrentPhase = SwimPhase;
	mSwimPhaseSchedulerFlag = false;

	//Cancel spawn head scheduler, if any
	this->unschedule("spawnHead");

	if (mHead)
		mHead->retreat();

	if (mTentacle)
		mTentacle->retreat();
}

void MonsterManager::updateSwimPhase()
{
	if (!mBody)
	{
		if (mTentacle && mTentacle->canDestroy())
		{
			removeChild(mTentacle, true);
			mTentacle = nullptr;
		}
		if (mHead && mHead->canDestroy())
		{
			if (mHead->getHealthPercentage() != 0)
				((GameScene*)Director::getInstance()->getRunningScene())->getGameUI()->resetTemporaryDamage();

			removeChild(mHead, true);
			mHead = nullptr;
		}
		if (!mHead && !mTentacle)
		{
			spawnBody();
		}
	}
}

void MonsterManager::update(float dt)
{
	if (mCurrentPhase == AttackPhase)
	{
		updateAttackPhase();
	}
	else if (mCurrentPhase == SwimPhase)
	{
		updateSwimPhase();
	}
}

float MonsterManager::getMonsterHealth()
{
	return mCurrentHealth * 100.f / mMaxHealth;
}

float MonsterManager::getMonsterHeadHealth()
{
	if (mHead)
		return mHead->getHealthPercentage();

	else
		return 100.f;
}
