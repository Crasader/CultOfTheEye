#pragma once

#include "cocos2d.h"
#include "Types.h"
#include <array>

using namespace std;
using namespace cocos2d;

class Debris : public Node
{
public:
	Debris(Vec2 position, int type, int level);
	~Debris();
	
	//Geometry Based Functions
	array<Vec2, 4> getBoundingPolygon();
	bool	containsPoint(Vec2 point);
	Vec2	findNearestPoint(Vec2 point);

	static Debris* create(Vec2 position, int type, int level)
	{
		Debris *pRet = new(std::nothrow) Debris(position, type, level);
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

private:
	Sprite*			mSprite;
	PhysicsBody*	mPhysicsBody;
};
