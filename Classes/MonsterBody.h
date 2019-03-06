#pragma once
#include "cocos2d.h"
#include "Monster.h"
#include <map>
using namespace cocos2d;

class Body : public Monster
{
	int mTargetQuadrant;
	int mDirection;
public:
	static Body* create(int quadrant, Vec2 position);

	enum Animations
	{
		Idle, Count
	};
	static vector<vector<vector<PhysicsShapePolygon*>>> mShapes;

			Body(int quadrant, Vec2 position);
	void	update(float dt);
	void	setTargetQuadrant(int target);
	
	void	createSprite();
	void	createPhysicsBody();
	void	spawn();
};