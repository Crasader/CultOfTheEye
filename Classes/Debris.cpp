#include "Debris.h"
#include "RNG.h"
#include "PhysicsShapeCache.h"

Debris::Debris(Vec2 position, int type, int level)
{
	setName(ClassStrings::Debris);

	switch (type)
	{
	case eTowerType::NORMAL:
		if(level == 0)
			mSprite = Sprite::createWithSpriteFrameName("_attackTower0103.png");
		else if (level == 1)
			mSprite = Sprite::createWithSpriteFrameName("_attackTower0203.png");
		//mPhysicsBody= PhysicsShapeCache::getInstance()->createBodyWithName("_attackTowerDebris");
		break;
	
	case eTowerType::SHIELD:
		mSprite = Sprite::createWithSpriteFrameName("_shieldTower0103.png");
		
		//mPhysicsBody = PhysicsShapeCache::getInstance()->createBodyWithName("_shieldTowerDebris");
		break;
	
	default: break;
	}

	addChild(mSprite);

	mPhysicsBody = PhysicsBody::createBox(mSprite->getBoundingBox().size, PhysicsMaterial(0.001f, 1.0f, 0.0f));
	mPhysicsBody->setDynamic(true);
	mPhysicsBody->setLinearDamping(0.15);
	//mPhysicsBody->setAngularDamping(0.1);
	mPhysicsBody->setCollisionBitmask(0x00000000);
	mPhysicsBody->setRotationEnable(true);

	mSprite->addComponent(mPhysicsBody);
	//mSprite->setPhysicsBody(mPhysicsBody);

	mSprite->setAnchorPoint(Vec2(0.5, 0.5));
	mSprite->setScale(1);
	mSprite->setRotation(90 - CC_RADIANS_TO_DEGREES(position.getAngle()));
	mSprite->setPosition(position);

	Vec2 impulse = Vec2(RNG::getUniformInteger(2500, 7500), RNG::getUniformInteger(2500, 7500));
	//float torque = (float)RNG::getUniformInteger(-10000, 10000);

	mPhysicsBody->applyImpulse(impulse);
	mPhysicsBody->setAngularVelocity((float)(RNG::getUniformInteger(-2, 2)));

	schedule([this](float a)
	{
		removeFromParentAndCleanup(true);
	}, 0, 0, 15, "Disintegrate");
}

Debris::~Debris()
{}

bool Debris::containsPoint(Vec2 point)
{
	auto mPoints = getBoundingPolygon();

	bool collision = false;

	//Check if a polygon contains a point	
	for (int i = 0; i < 4; i++)
	{
		//Get the next vertex index
		int j = (i + 1) % 4;

		//Get the two vertexes making a polygon Line
		auto A = mPoints[i];
		auto B = mPoints[j];

		bool doesExistUnderYPlane = ((A.y > point.y && B.y < point.y) || (A.y < point.y && B.y > point.y));
		if (doesExistUnderYPlane && (point.x < (B.x - A.x) * (point.y - A.y) / (B.y - A.y) + A.x))
		{
			collision = !collision;
		}
	}
	return collision;
}

array<Vec2, 4> Debris::getBoundingPolygon()
{
	auto a = mSprite->getBoundingBox();
	auto angle = CC_DEGREES_TO_RADIANS(mSprite->getRotation());
	auto size = mSprite->getContentSize() * mSprite->getScale();

	auto anchorPoint = mSprite->getAnchorPointInPoints() * mSprite->getScale();
	auto position = mSprite->getPosition();

	array<Vec2, 4> mPoints;
	mPoints[0] = Vec2(0, 0);
	mPoints[1] = Vec2(size.width, 0);
	mPoints[2] = Vec2(size.width, size.height);
	mPoints[3] = Vec2(0, size.height);

	//Apply Translation Matrix
	for (int i = 0; i < 4; i++)
	{
		mPoints[i] -= anchorPoint;
	}

	//Apply Rotation Matrix
	for (int i = 0; i < 4; i++)
	{
		float x = mPoints[i].x * cos(angle) + mPoints[i].y * sin(angle);
		float y = -mPoints[i].x * sin(angle) + mPoints[i].y * cos(angle);
		mPoints[i].x = x;
		mPoints[i].y = y;
	}

	//Apply Translation Matrix
	for (int i = 0; i < 4; i++)
	{
		mPoints[i] += position;
	}

	return mPoints;
}

Vec2 Debris::findNearestPoint(Vec2 point)
{
	auto mPoints = getBoundingPolygon();

	float distance = INT_MAX;
	Vec2 intersection = Vec2(0, 0);

	//Check for each line segment
	for (int i = 0; i < 4; i++)
	{
		//The line segment runs from A to B. P is the point.
		int j = (i + 1) % 4;
		auto A = mPoints[i];
		auto B = mPoints[j];
		auto P = point;
		auto M = B - A;

		//T is the intersection point if given line segment is treated as a line.
		auto T = Vec2::dot(M, P - A) / Vec2::dot(M, M);

		//C is the closest intersection point. Since we need to check for a line segment, T -> [0,1]
		Vec2 C;
		if (T < 0)
			C = A;
		else if (T > 1)
			C = B;
		else
			C = A + T * M;

		//If the closest intersection point is the shortest among all four, save it
		if (distance > P.distance(C))
		{
			intersection = C;
			distance = P.distance(C);
		}
	}
	return intersection;
}

