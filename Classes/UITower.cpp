#include "UITower.h"
#include "TowerManager.h"
#include "GameScene.h"
#include "Core.h"
#include "UIButton.h"

UITower::UITower(eTowerType iTowerType) : mCategory(iTowerType)
{
	switch (mCategory)
	{
	case SHIELD:
		mSprite = Sprite::createWithSpriteFrameName("_shieldTower0101.png");
		break;
	case NORMAL:
		mSprite = Sprite::createWithSpriteFrameName("_attackTower0101.png");
		break;
	default:
		break;
	}

	addChild(mSprite);

	mVelocity = Vec2(0,0);
	
	mQueueSize = 0;

	mCanDrag = true;

	mTowerManager = nullptr;
	mActiveScene = nullptr;
	mCore = nullptr;

	mRootPosition = Vec2(0, 0);

	mPhysicsBody = PhysicsBody::createBox(mSprite->getBoundingBox().size, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setCollisionBitmask(0x00000000);
	mSprite->addComponent(mPhysicsBody);

	mSprite->setVisible(false);
	mSprite->setScale(0.25);

	//We don't want update to be called until the initial position of the tower is set
	mReadyToUpdate = false;

	mTouchListener = EventListenerTouchOneByOne::create();

	mTouchListener->onTouchBegan = CC_CALLBACK_2(UITower::onTouchBegan, this);
	mTouchListener->onTouchMoved = CC_CALLBACK_2(UITower::onTouchMoved, this);
	mTouchListener->onTouchEnded = CC_CALLBACK_2(UITower::onTouchEnded, this);

	mTouchListener->setEnabled(false);
	mTouchListener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mTouchListener, 1);

	this->scheduleUpdate();
}

UITower::~UITower()
{}

void UITower::update(float dt)
{
	mCurrentPosition = mPhysicsBody->getPosition();

	Vec2 distance = mCurrentPosition - Vec2(0, 0);

	mSprite->setOpacity(((distance.length()) / (ScaleConstants::RING_RADIUS))*255);
	
	if (mReadyToUpdate)
	{	
		//mPhysicsBody->applyForce(((Vec2(0,0) - mSprite->getPosition()).getNormalized()) * 200);
		
		mActiveScene = (GameScene*)this->getScene();
		mTowerManager = mActiveScene->getTowerManager();
		mCore = mActiveScene->getCore();

		Quadrants currentQuadrant;

		//Compares the Towers' root quadrant to its current quadrant and resets gracefully if it goes out of bounds
		if (mCurrentPosition.x < 0)
		{
			if (mCurrentPosition.y < 0)
			{
				currentQuadrant = D;
			}
			else
			{
				currentQuadrant = A;
			}
		}

		else
		{
			if (mCurrentPosition.y < 0)
			{
				currentQuadrant = C;
			}
			else
			{
				currentQuadrant = B;
			}
		}

		if (currentQuadrant != mRootQuadrant)
		{
			reset();
		}

		if (mCore->containsPointOnRing(mCurrentPosition) && (this->getVelocity() != Vec2(0, 0)))
		{
			Vec2 spawnedTowerPosition = mTowerManager->spawnTowerAt(mCurrentPosition, mCategory);
			
			if (spawnedTowerPosition == Vec2(0,0))
				reset();
			else
				deactivate(spawnedTowerPosition);
		}
	}

	else
	{
		if (mCurrentPosition == mRootPosition)
			mReadyToUpdate = true;
	}
}

void UITower::reset()
{	
	//(dynamic_cast<UIButton*>(getParent()))->flashRed();
	
	mTouchListener->setEnabled(false);
	mReadyToUpdate = false;
	this->unscheduleUpdate();

	auto a = Vec2(0, 0);
	setVelocity(a);
	setPositionGracefully(mRootPosition);

	auto scaleTo = ScaleTo::create(0.25, 0.25);
	mSprite->runAction(scaleTo);

	schedule([this](float a)
	{
		mSprite->setVisible(false);
	}, 0, 0, 0.25, "PlaceTower");

	mQueueSize = 0;
	clearPositionQueue();
}

bool UITower::onTouchBegan(Touch* touch, Event* event)
{	
	//We don't need to update until the touch is completed
	mReadyToUpdate = false;

	return true;
}

void UITower::onTouchMoved(Touch* touch, Event* event)
{	
	if (mCanDrag)
	{		
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

		auto worldLocation = Vec2(x, y);
		setPosition(worldLocation);

		if (mQueueSize < 10)
		{
			mPositionOverTime.push(worldLocation);
			mQueueSize++;
		}

		else
		{
			mPositionOverTime.pop();
			mPositionOverTime.push(worldLocation);
		}

		mCurrentPosition.x = x;
		mCurrentPosition.y = y;

		if (mCore->containsPointOnRing(mCurrentPosition))
		{
			mCanDrag = false;

			Vec2 spawnedTowerPosition = mTowerManager->spawnTowerAt(mCurrentPosition, mCategory);

			if (spawnedTowerPosition == Vec2(0, 0))
				this->reset();

			else
				this->deactivate(spawnedTowerPosition);
		}
	}
}

void UITower::onTouchEnded(Touch* touch, Event* event)
{
	if (touch->getLocation() == touch->getStartLocation())
	{
		mQueueSize = 0;
		clearPositionQueue();
		deactivate();
		return;
	}
	
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

	if (mPositionOverTime.size() > 0)
	{
		Vec2 flickVelocity = Vec2((x - mPositionOverTime.front().x), (y - mPositionOverTime.front().y));

		if ((abs(flickVelocity.x) > 100) || (abs(flickVelocity.y) > 100))
		{
			auto a = flickVelocity * 5;
			setVelocity(a);
		}

		else
		{
			reset();
		}

		mQueueSize = 0;

		clearPositionQueue();
	}
	mCanDrag = true;
	mReadyToUpdate = true;
}

void UITower::clearPositionQueue()
{
	std::queue<Vec2> empty;
	std::swap(mPositionOverTime, empty);
}

void UITower::deactivate()
{
	mTouchListener->setEnabled(false);

	mPhysicsBody->setEnabled(false);
	
	mRootPosition = Vec2(0, 0);
	
	mReadyToUpdate = false;
	setVelocity(Vec2(0, 0));

	auto scaleTo = ScaleTo::create(0.25, 0.25);
	mSprite->runAction(scaleTo);

	schedule([this](float a)
	{
		auto pos = Vec2(0, 0);
		setPosition(pos);
		mSprite->pause();
		mSprite->setVisible(false);
		this->unscheduleUpdate();
	}, 0, 0, 0.25, "PlaceTower");
}

void UITower::deactivate(Vec2 position)
{
	(dynamic_cast<UIButton*>(getParent()))->flashBlue();
	
	mTouchListener->setEnabled(false);

	mPhysicsBody->setEnabled(false);

	mRootPosition = Vec2(0, 0);

	mReadyToUpdate = false;
	this->unscheduleUpdate();

	setVelocity(Vec2(0, 0));

	auto scaleTo = ScaleTo::create(0.25, 1);
	mSprite->runAction(scaleTo);

	setPositionGracefully(position);

	schedule([this](float a)
	{
		deactivate();
	}, 0, 0, 0.25, "PlaceTower");
}

void UITower::activate(Vec2 position)
{
	this->unscheduleAllCallbacks();
	
	mTouchListener->setEnabled(true);

	mCanDrag = true;

	mPhysicsBody->setEnabled(true);

	mRootPosition = position;

	mSprite->resume();
	this->setPosition(position);
	mSprite->setVisible(true);
	auto scaleTo = ScaleTo::create(0.25, 2);
	mSprite->runAction(scaleTo);
	
	//All the pos checks to are to check whether you are flicking into another quadrant
	if (mRootPosition.x < 0)
	{
		if (mRootPosition.y < 0)
		{
			mRootQuadrant = D;
		}
		else
		{
			mRootQuadrant = A;
		}
	}

	else
	{
		if (mRootPosition.y < 0)
		{
			mRootQuadrant = C;
		}
		else
		{
			mRootQuadrant = B;
		}
	}
	
	this->scheduleUpdate();
}

void UITower::setPosition(Vec2 pos)
{
	mCurrentPosition = pos;
	mSprite->setPosition(mCurrentPosition);
}

void UITower::setPositionGracefully(Vec2 pos)
{
	mCurrentPosition = pos;
	auto moveTo = MoveTo::create(0.25, mCurrentPosition);
	mSprite->runAction(moveTo);
}

void UITower::setVelocity(Vec2 velocity)
{
	mVelocity = velocity;
	mPhysicsBody->setVelocity(mVelocity);
}

Vec2 UITower::getVelocity() const
{
	return mVelocity;
}

bool UITower::containsPoint(Vec2 point)
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

array<Vec2, 4> UITower::getBoundingPolygon()
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

Vec2 UITower::findNearestPoint(Vec2 point)
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
