#pragma once
#include "cocos2d.h"
#include "ResourceManager.h"
#include <memory>

using namespace cocos2d;

class Gem : public Node 
{
	Sprite*					mSprite;
	PhysicsBody*			mPhysicsBody;
	GameUI*					mUI;

public:
	Gem(Vec2 position);

	static Gem* create(Vec2 position) 
	{
		Gem *pRet = new(std::nothrow) Gem(position);
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

	void update(float time);
	void handleTouch(Touch* touch);
	void collect();
	Rect getBoundingBox();

	void expire();
	bool markedForDeletion = false;

	~Gem();

};
