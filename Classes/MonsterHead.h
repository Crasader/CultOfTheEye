#pragma once
#include "cocos2d.h"
#include "Tower.h"
#include "Monster.h"
#include <map>
using namespace cocos2d;

class Head : public Monster
{
	int mStage;
	CCParticleSystemQuad* mEmitter;
public:
	enum Animations
	{
		Idle, Spray, Count
	};
	static vector<vector<vector<PhysicsShapePolygon*>>> mShapes;
	static Head* create(int stage, int quadrant, Vec2 position);

public:
	Head(int stage, int quadrant, Vec2 position);

	void	createSprite();
	void	createPhysicsBody();

	void	attack();
	void	spawn();
};
