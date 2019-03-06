#include "UIButton.h"
#include "UITower.h"

UIButton::UIButton(eTowerType iTowerType, Vec2 iPosition) : mCategory(iTowerType), mRootPosition(iPosition)
{
	switch (mCategory)
	{
	case SHIELD:
		mSprite = Sprite::createWithSpriteFrameName("shieldBtnUp.png");
		break;
	case NORMAL:
		mSprite = Sprite::createWithSpriteFrameName("attackBtnUp.png");
		break;
	default:
		break;
	}

	mSprite->setAnchorPoint(Vec2(0.5, 0));
	//mUIButtons[buttonIndex - 1]->setOpacity(128);
	mSprite->setScale(0.775);
	mSprite->setPosition(mRootPosition);
	mSprite->setRotation(90 - (CC_RADIANS_TO_DEGREES(mRootPosition.getAngle())));
	mSprite->setVisible(false);

	addChild(mSprite);

	mUITower = UITower::create(iTowerType);
	addChild(mUITower);

	mIsCreatingTower = false;

	mTouchListener = EventListenerTouchOneByOne::create();

	mTouchListener->onTouchBegan = CC_CALLBACK_2(UIButton::onTouchBegan, this);
	mTouchListener->onTouchMoved = [this](Touch* touch, Event* event) {};
	mTouchListener->onTouchEnded = [this](Touch* touch, Event* event) 
	{
		mIsCreatingTower = false;

		switch (mCategory)
		{
		case SHIELD:
			mSprite->setSpriteFrame("shieldBtnUp.png");
			break;
		case NORMAL:
			mSprite->setSpriteFrame("attackBtnUp.png");
			break;
		default:
			break;
		}
	};

	//mTouchListener->setSwallowTouches(true);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mTouchListener, this);
}

UIButton::~UIButton()
{}

bool UIButton::onTouchBegan(Touch* touch, Event* event)
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

	if (containsPoint(Vec2(x, y)))
	{
		mUITower->activate(Vec2(x,y));
		mIsCreatingTower = true;

		switch (mCategory)
		{
		case SHIELD:
			mSprite->setSpriteFrame("shieldBtnDown.png");
			break;
		case NORMAL:
			mSprite->setSpriteFrame("attackBtnDown.png");
			break;
		default:
			break;
		}

		return true;
	}

	return false;
}

void UIButton::deactivate()
{
	mTouchListener->setEnabled(false);
	mSprite->pause();
	mSprite->setVisible(false);
	//mUITower->deactivate();
}

void UIButton::activate()
{
	mTouchListener->setEnabled(true);
	mSprite->resume();
	mSprite->setVisible(true);
}

bool UIButton::containsPoint(Vec2 point)
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

array<Vec2, 4> UIButton::getBoundingPolygon()
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

Vec2 UIButton::findNearestPoint(Vec2 point)
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

void UIButton::flashBlue()
{
	auto flashBlue = TintTo::create(0.45f, 0, 0, 255);
	auto setNormalColor = TintTo::create(0.45f, 255, 255, 255);
	auto sequence = Sequence::create(flashBlue, setNormalColor, nullptr);
	mSprite->runAction(sequence);
}

void UIButton::flashRed()
{
	auto flashRed = TintTo::create(0.45f, 255, 0, 0);
	auto setNormalColor = TintTo::create(0.45f, 255, 255, 255);
	auto sequence = Sequence::create(flashRed, setNormalColor, nullptr);
	mSprite->runAction(sequence);
}
