#pragma once
#include "cocos2d.h"
#include "Monster.h"
#include "Tentacle.h"
#include "MonsterHead.h"
#include "MonsterBody.h"
#include "Types.h"
#include <vector>
#include <array>

using namespace cocos2d;
using namespace std;

class MonsterManager : public Node
{
public: 
	enum Phase
	{
		AttackPhase = 1 << 0,
		SwimPhase = 1 << 1,
		RetreatPhase = 1 << 2
	};

private:
	Monster*	mTentacle;
	Monster*	mHead;
	Monster*	mBody;

	int mSpawnRate;
	int mTargetQuadrant;
	int mCurrentPhase;

	int mCurrentHealth;
	int mMaxHealth;

	bool mSwimPhaseSchedulerFlag;

public:
	static MonsterManager* create()
	{
		MonsterManager *pRet = new(std::nothrow) MonsterManager();
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

	MonsterManager();
	~MonsterManager();
	
	void findRandomTargetQuadrant();

	void spawnTentacle();
	void spawnHead();
	void spawnBody();
	void update(float dt);

	float getMonsterHealth();
	float getMonsterHeadHealth();

	void initAttackPhase();
	void updateAttackPhase();

	void initSwimPhase();
	void updateSwimPhase();
};

