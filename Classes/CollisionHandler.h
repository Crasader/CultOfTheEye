#pragma once
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class CollisionHandler
{
public:
	CollisionHandler(Node* node);
	
	bool onContactBegin(PhysicsContact& contact);
	bool matchCategory(pair<PhysicsShape*, PhysicsShape*>& p, string type1, string type2);
};

