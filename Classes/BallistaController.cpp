#include "BallistaController.h"
#include "Projectile.h"
#include "Tower.h"
#include "Audio.h"

using namespace CocosDenshion;

Ballista::Ballista(Tower* node) : mTower(node)
{
	mCoolDown = 0;
	mCanActivate = true;

	mTouchListener = EventListenerTouchOneByOne::create();

	mTouchListener->onTouchBegan = CC_CALLBACK_2(Ballista::onTouchBegan, this);
	mTouchListener->onTouchMoved = CC_CALLBACK_2(Ballista::onTouchMoved, this);
	mTouchListener->onTouchEnded = CC_CALLBACK_2(Ballista::onTouchEnded, this);

	auto polygon = mTower->getBoundingPolygon();

	mThis = this;
	
	mTouchListener->setEnabled(false);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mTouchListener, 1);
}

bool Ballista::onTouchBegan(Touch* touch, Event* event)
{
	auto towerSprite = (Sprite*)(mTower->getChildByName("Sprite"));
	auto polygon = mTower->getBoundingPolygon();

	mSlingSprite = Sprite::create(mSlingFilename);
	mSlingSprite->setAnchorPoint(Vec2(0.5, 0));
	mSlingSprite->setRotation(towerSprite->getRotation());
	mSlingSprite->setPosition((polygon[2] + polygon[3]) / 2);
	addChild(mSlingSprite);

	mMarkerSprite = Sprite::create(mMarkerFilename);
	mMarkerSprite->setAnchorPoint(Vec2(0.5, 0));
	mMarkerSprite->setRotation(towerSprite->getRotation());
	mMarkerSprite->setPosition((polygon[2] + polygon[3]) / 2);
	//mMarkerSprite->setScale(2);
	addChild(mMarkerSprite);

	float x = touch->getLocation().x;
	float y = touch->getLocation().y;

	auto cam = this->getScene()->getDefaultCamera()->getPosition();

	if (cam.x < 0)
	{
		x -= 1920;
	}

	if (cam.y < 0)
	{
		y -= 1080;
	}


	mProjectileSprite = Sprite::create(FilePaths::AttackProjectile);
	mProjectileSprite->setAnchorPoint(Vec2(0.5, 0.5));
	mProjectileSprite->setPosition(Vec2(x,y));
	mProjectileSprite->setScale(1.5);
	addChild(mProjectileSprite, 2);

	mReferenceVector = ((polygon[2] + polygon[3]) / 2) - ((polygon[0] + polygon[1]) / 2);
	mReferenceAngle = CC_RADIANS_TO_DEGREES(mReferenceVector.getAngle());
	return true;
}

void Ballista::onTouchMoved(Touch* touch, Event* event)
{
	float x = touch->getLocation().x;
	float y = touch->getLocation().y;

	Vec2 cam;

	auto camera = this->getScene()->getDefaultCamera();

	if(camera)
		cam = camera->getPosition();

	else
		return;

	if (cam.x < 0)
	{
		x -= 1920;
	}

	if (cam.y < 0)
	{
		y -= 1080;
	}

	Vec2 mousePosition = Vec2(x, y);
	Vec2 projectileDistance = mousePosition - mMarkerSprite->getPosition();
	Vec2 projectilePosition;

	auto a = dynamic_cast<Tower*>(this->getParent())->getBoundingPolygon();

	//array<float, 4> angle = { a[0].getAngle(worldPositionOriginal), a[1].getAngle(worldPositionOriginal), a[2].getAngle(worldPositionOriginal), a[3].getAngle(worldPositionOriginal) };
	array<float, 4> angle = { a[0].getAngle(), a[1].getAngle(), a[2].getAngle(), a[3].getAngle() };
	pair<float, float> sweep;

	if(projectileDistance.getLengthSq() > 40000)
	{
		projectileDistance.normalize();
		projectileDistance *= (195);

		projectilePosition = mMarkerSprite->getPosition() + projectileDistance;

		Vec2 directionVector = mousePosition - projectilePosition;
		double elasticity = log(directionVector.getLengthSq());
		directionVector.normalize();
		directionVector *= (elasticity* 10);

		projectilePosition += directionVector;
	}

	else
	{
		projectilePosition = mMarkerSprite->getPosition() + projectileDistance;
	}

	mProjectileSprite->setPosition(projectilePosition);

	sweep.first = min(angle[0], min(angle[1], min(angle[2], angle[3])));
	sweep.second = max(angle[0], max(angle[1], max(angle[2], angle[3])));
	{
		removeChildByName("DEBUG");
		Vec2 first = Vec2(1000, 0);
		Vec2 second = Vec2(1000, 0);
		first = first.rotateByAngle(Vec2(0, 0), sweep.first);
		second = second.rotateByAngle(Vec2(0, 0), sweep.second);
		DrawNode* a = DrawNode::create();
		a->drawLine(projectilePosition, first, Color4F::GRAY);
		a->drawLine(projectilePosition, second, Color4F::GRAY);
		a->setName("DEBUG");
		addChild(a);
	}

	int factor = 0;
	float distance = touch->getLocation().distance(touch->getStartLocation());
	
	if (distance >= mMinRange)
	{
		
		//if (distance >= 25 && distance < 50)
		//	factor = 1;
		//else if (distance >= 50 && distance < 75)
		//	factor = 2;
		//else if (distance >= 75 && distance < 100)
		//	factor = 3;
		//else
		//	factor = 4;

		auto direction = (touch->getStartLocation() - touch->getLocation());
		float x1 = mReferenceVector.x, y1 = mReferenceVector.y, x2 = direction.x, y2 = direction.y;
		auto angle = -CC_RADIANS_TO_DEGREES(atan2(x1 * y2 - y1 * x2, x1 * x2 + y1 * y2));

		//Clip the angle to min and max range
		angle = max(-90.f, angle);
		angle = min(90.f, angle);

		//Set the angle as an offset
		mMarkerSprite->setRotation(90 - mReferenceAngle + angle);		
	}

	//TintTo* indicator;
	//switch (factor)
	//{
	//case 0: indicator = TintTo::create(0, 255,	255,	255);	break;
	//case 1: indicator = TintTo::create(0, 0,	255,	0);		break;
	//case 2: indicator = TintTo::create(0, 255,	255,	0);		break;
	//case 3: indicator = TintTo::create(0, 255,	0,		0);		break;
	//case 4: indicator = TintTo::create(0, 0,	0,		0);		break;
	//}
	//mMarkerSprite->runAction(indicator);
}

void Ballista::onTouchEnded(Touch* touch, Event* event)
{
	removeChildByName("DEBUG");
	
	auto polygon = mTower->getBoundingPolygon();
	auto angle = CC_DEGREES_TO_RADIANS(mMarkerSprite->getRotation());
	auto A = (polygon[0] + polygon[1]) / 2;
	auto B = Vec2(A.x + sin(angle), A.y + cos(angle));
	auto C = B - A;

	float distance = touch->getLocation().distance(touch->getStartLocation());
	if (distance < mMinRange)
	{
		//TODO: The Projectile is cancelled!
	}
	else
	{
		if (distance >= 25 && distance < 50)
			distance = 25;
		else if (distance >= 50 && distance < 75)
			distance = 50;
		else if (distance >= 75 && distance < 100)
			distance = 75;
		else
			distance = 100;

		//distance -= mMinRange;
		//distance = min(distance, mMaxRange);

		BallisticInfo info;
		info.mDirection = C / C.length();
		info.mPosition = mProjectileSprite->getPosition();
		info.mForce = distance / 45.f;
		info.mTower = mTower;
		info.mDamage = mDamage;

		auto ball = mSlingAction(info);
		mTower->addChild(ball, 1);

		Audio::getInstance()->playEffect(FilePaths::Audio::SFX::Tower::Shoot);

		mCanActivate = false;
		mTower->enterCooldown();

		schedule([this](float a)
		{
			mCanActivate = true;
			mTower->exitCooldown();
		}, mCoolDown, 0, 0, "ReActivate");
	}
	removeAllChildrenWithCleanup(true);
	mTouchListener->setEnabled(false);
}

void Ballista::setSpriteFilenames(string sling, string slingBall, string projectile)
{
	mSlingFilename = sling;
	mMarkerFilename = slingBall;
	mProjectileFilename = projectile;
}

Ballista::~Ballista()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(mTouchListener);
}

void Ballista::setSlingAction(function<Projectile* (BallisticInfo)> action)
{
	mSlingAction = action;
}

void Ballista::activate()
{
	if (mCanActivate)
		mTouchListener->setEnabled(true);
}

void Ballista::deactivate()
{
	mTouchListener->setEnabled(false);
}

bool Ballista::canActivate()
{
	return mCanActivate;
}

void Ballista::setCoolDown(int cooldown)
{
	mCoolDown = cooldown;
}

int Ballista::getCoolDown()
{
	return mCoolDown;
}

void Ballista::setDamage(int dmg)
{
	mDamage = dmg;
}

int Ballista::getDamage()
{
	return mDamage;
}

