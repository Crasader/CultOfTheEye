#pragma once
#include "cocos2d.h"
#include "Tower.h"
#include "Monster.h"
#include <map>

using namespace cocos2d;

class Tentacle : public Monster
{
	int mStage;
public:
	enum Animations
	{
		Idle, Attack, Damaged, Retreat, Spawn, Count
	};
	static vector<vector<vector<PhysicsShapePolygon*>>> mShapes;
	static Tentacle* create(int stage, int quadrant, Vec2 position);

	Tentacle(int stage, int quadrant, Vec2 position);

	void	createPhysicsBody();
	void	createSprite();

	void	attack();
	void	spawn();
	void	retreat();
	void	withdraw();
	void	idle();
};
