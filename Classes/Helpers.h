#pragma once
#include "cocos2d.h"

namespace Helpers
{
	//Used Alot For Placing Items around the Core Accurately
	cocos2d::Vec2 placeAtAngle(float angleInDegrees, float radius);

	//Gets the angle made by a point with the core
	double getCoreAngle(cocos2d::Vec2 pos);
}
