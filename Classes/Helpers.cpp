#pragma once
#include "Helpers.h"
#include "GameScene.h"
#include "Core.h"

using namespace cocos2d;
using namespace std;

namespace Helpers
{
	Vec2 placeAtAngle(float angleInDegrees, float radius)
	{
		int towerX = 0;
		int towerY = 0;
		float a1, a2, a3, x1, x2, x3, xPos, yPos;

		a1 = angleInDegrees / 2;
		a2 = 90 - angleInDegrees;
		a3 = 90 - (a1 + a2);
		a1 = CC_DEGREES_TO_RADIANS(a1);
		a2 = CC_DEGREES_TO_RADIANS(a2);
		a3 = CC_DEGREES_TO_RADIANS(a3);
		x1 = sinf(a1) * radius * 2;
		x2 = sinf(a3) * x1;
		x3 = cosf(a3) * x1;
		xPos = radius - x2;
		yPos = x3;

		return Vec2(xPos, yPos);
	}

	double getCoreAngle(Vec2 pos)
	{
		float ringRadius = ScaleConstants::RING_RADIUS;
		//We need to correct the radius slightly to account for alpha in the sprite
		ringRadius -= 50;
		float intersectionRadius = pos.getDistance(Vec2(0, 0));

		Vec2 reference = Vec2(ringRadius, 0);

		double angle = acos(reference.dot(pos) / (intersectionRadius*ringRadius));
		angle = CC_RADIANS_TO_DEGREES(angle);

		//Let's compute the final angle
		if (pos.y < 0)
		{
			angle = 360 - angle;
		}

		return angle;
	}
}

