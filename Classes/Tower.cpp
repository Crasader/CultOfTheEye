#include "Tower.h"
#include "GameScene.h"
#include "PhysicsShapeCache.h"
#include "SimpleAudioEngine.h"
#include "Helpers.h"

int NormalTower::sBuildCost = 2;
int ShieldTower::sBuildCost = 3;

void Tower::superInit()
{
	mShield = nullptr;

	mActDeactCtr = 0;
	setName(ClassStrings::Tower);
		
	mLevel = 0;
	mActiveAggro = 0;

	mState = ACTIVE;

	//If you're not spawning the Tower Upright, it is the initial spawn!
	mPhysicsBody = PhysicsShapeCache::getInstance()->setBodyOnSprite(mPhysicsFile, mSprite);
	mPhysicsBody->setPositionOffset(Vec2(0, 0));

	auto physicsShapes = mPhysicsBody->getShapes();

	for (auto itr = physicsShapes.begin(); itr != physicsShapes.end(); itr++)
	{
		((MyPhysicsShapePolygon*)*itr)->setScale(1.35, 1.35);
	}
	
	mSprite->setAnchorPoint(Vec2(0.5, 0.5));
	mSprite->setPosition(mPosition);
	auto rect = mSprite->getBoundingBox();
	//log("%f %f", rect.getMaxX() - rect.getMinX(), rect.getMaxY() - rect.getMinY());
	mSprite->setScale(1.15);

	mSprite->setName("Sprite");
	addChild(mSprite);	

	addChild(mBallista);

	mCurrentColor = DEFAULT;

	mProgressTimer = ProgressTimer::create(Sprite::create("Images/upgradeMeter.png"));
	//mProgressTimer->setRotation(180);
	//mProgressTimer->setPosition(mPosition);
	mProgressTimer->setScale(0.7);
	mProgressTimer->setType(cocos2d::ProgressTimer::Type::RADIAL);
	mProgressTimer->setPercentage(0);
	mProgressTimer->setOpacity(128);
	mProgressTimer->setVisible(false);
	addChild(mProgressTimer);

	mUpgradeCostImage->setPosition(mPosition);
	mUpgradeCostImage->setScale(0.75);
	//mUpgradeCostImage->setVisible(false);
	addChild(mUpgradeCostImage);

	mTouchListener = EventListenerTouchOneByOne::create();
	mTouchListener->onTouchBegan = CC_CALLBACK_2(Tower::onTouchBegan, this);
	mTouchListener->onTouchMoved = CC_CALLBACK_2(Tower::onTouchMoved, this);
	mTouchListener->onTouchEnded = CC_CALLBACK_2(Tower::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mTouchListener, this);

    if (!mSpawnStraight)
    {
		mUpgradeCostImage->setOpacity(0);
		
		mSprite->setRotation(90 - CC_RADIANS_TO_DEGREES(mPosition.getAngle()));
		mUpgradeModeActive = false;
        //mTouchListener->setEnabled(true);
    }
    else
    {
		mSprite->setOpacity(128);
        if (canUpgrade())
            mSprite->setColor(Color3B(0, 255, 0));
        else
            mSprite->setColor(Color3B(255, 0, 0));
		mUpgradeModeActive = true;
        //mTouchListener->setEnabled(false);
    }

	mUpgradeTime = UpgradeCosts::UpgradeTime;

	//mIndicator = CCParticleSystemQuad::create("Images/slot.plist");
	//mIndicator->pauseEmissions();
	//mIndicator->setPosition(mPosition);
	//this->addChild(mIndicator);

	mRootAngle = CC_RADIANS_TO_DEGREES(mPosition.getAngle());
	mCurrentAngle = mRootAngle;
	mCurrentRadius = ScaleConstants::RING_RADIUS;
}

bool Tower::onTouchBegan(Touch* touch, Event* event)
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

	if (containsPoint(Vec2(x, y)) && !mUpgradeModeActive && mState == ACTIVE)
	{
		mBallista->activate();
		mBallistaActive = true;
	}

	else if (containsPoint(Vec2(x, y)) && mUpgradeModeActive && canUpgrade())
	{
		mProgressTimer->setPosition(mPosition);
		mProgressTimer->setVisible(true);
		mIsBeingUpgraded = true;
		//mIndicator->resumeEmissions();
		this->scheduleUpdate();

		//auto tintTo = TintTo::create(UpgradeCosts::UpgradeTime, Color3B(0, 255, 255));
		//mProgressTimer->runAction(tintTo);
	}

	return true;
}

void Tower::onTouchMoved(Touch* touch, Event* event)
{
	if (mIsBeingUpgraded)
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

		Vec2 worldPosition = Vec2(x, y);

		if (!containsPoint(worldPosition))
		{
			mIsBeingUpgraded = false;
			mUpgradeTime = UpgradeCosts::UpgradeTime;
			mProgressTimer->setVisible(false);
			this->unscheduleUpdate();
		}
	}
	
	if (mBallistaActive)
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

		Vec2 worldPosition = Vec2(x, y);

		Vec2 distance = worldPosition - mPosition;
		float correctedDistance = log(distance.getLengthSq());
		distance.normalize();
		distance *= correctedDistance;

		mSprite->setPosition(mPosition + distance);
	}
}

void Tower::onTouchEnded(Touch* touch, Event* event)
{
	if (mIsBeingUpgraded)
	{
		mIsBeingUpgraded = false;
		mUpgradeTime = UpgradeCosts::UpgradeTime;
		mProgressTimer->setVisible(false);
		this->unscheduleUpdate();
		//mIndicator->pauseEmissions();
	}
	
	else if (mBallistaActive)
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

		Vec2 worldPosition = Vec2(x, y);

		Vec2 distance = mPosition - worldPosition;
		
		float correctedDistance = log(distance.getLengthSq());
		float correctedDistance1 = 0.1*log(distance.getLengthSq());
		float correctedDistance2 = 0.05*log(distance.getLengthSq());
		float correctedDistance3 = 0.025*log(distance.getLengthSq());
		
		distance.normalize();
		distance *= correctedDistance;

		Vec2 distance1 = distance * correctedDistance1;
		auto moveTo1 = MoveTo::create(0.1, (mPosition + distance1));

		Vec2 distance2 = distance * correctedDistance2;
		auto moveTo2 = MoveTo::create(0.075, (mPosition - distance2));

		Vec2 distance3 = distance * correctedDistance3;
		auto moveTo3 = MoveTo::create(0.05, (mPosition + distance3));

		auto moveTo4 = MoveTo::create(0.025, mPosition);

		// create a sequence with the actions and callbacks
		auto seq = Sequence::create(moveTo1, moveTo2, moveTo3, moveTo4, nullptr);

		mSprite->runAction(seq);

		mBallistaActive = false;

	}
}


bool Tower::containsPoint(Vec2 point)
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

array<Vec2, 4> Tower::getBoundingPolygon()
{
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

Vec2 Tower::findNearestPoint(Vec2 point)
{
	//TODO Fix After GDC
	//return mSprite->getPosition();

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

pair<float, float> Tower::getSweep()
{
	auto a = getBoundingPolygon();
	array<float, 4> angle = { a[0].getAngle(), a[1].getAngle(), a[2].getAngle(), a[3].getAngle() };
	pair<float, float> sweep;
	sweep.first = min(angle[0], min(angle[1], min(angle[2], angle[3])));
	sweep.second = max(angle[0], max(angle[1], max(angle[2], angle[3])));
	return sweep;
}

void Tower::indicateUpgradeMode()
{
	mUpgradeModeActive = true;
	
	if (canUpgrade() && (mCurrentColor != GREEN))
	{		
		mCurrentColor = GREEN;
		auto action = TintTo::create(0.5, 0, 255, 0);
		mSprite->runAction(action);
	}
	
	if ((!canUpgrade()) && (mCurrentColor != RED))
	{
		mCurrentColor = RED;
		auto action = TintTo::create(0.5, 255, 0, 0);
		mSprite->runAction(action);
	}
}

void Tower::exitUpgradeMode()
{
	mUpgradeModeActive = false;

	mCurrentColor = DEFAULT;
	auto action = TintTo::create(0.5, 255, 255, 255);
	mSprite->runAction(action);
}

bool Tower::canUpgrade()
{
	int essence = ((GameScene*)Director::getInstance()->getRunningScene())->getEssenceAmount();
	if (mLevel < mUpgradeCost.size() && mUpgradeCost[mLevel] <= essence)
		return true;
	else
		return false;
}

void Tower::update(float dt)
{
	//{
	//removeChildByName("DEBUG");
	//	auto sweep = getSweep();
	//	Vec2 first = Vec2(1500, 0);
	//	Vec2 second = Vec2(1500, 0);
	//	first = first.rotateByAngle(Vec2(0, 0), sweep.first);
	//	second = second.rotateByAngle(Vec2(0, 0), sweep.second);
	//	DrawNode* a = DrawNode::create();
	//	a->drawLine(Vec2(0, 0), first, Color4F::ORANGE);
	//	a->drawLine(Vec2(0, 0), second, Color4F::ORANGE);
	//	a->setName("DEBUG");
	//	addChild(a);
	//}

	//{
	//	removeChildByName("DEBUGR");
	//	DrawNode* a = DrawNode::create();
	//	auto p = getBoundingPolygon();
	//	a->drawLine(p[0], p[1], Color4F::GREEN);
	//	a->drawLine(p[1], p[2], Color4F::GREEN);
	//	a->drawLine(p[2], p[3], Color4F::GREEN);
	//	a->drawLine(p[3], p[0], Color4F::GREEN);
	//	a->setName("DEBUGR");
	//	addChild(a);
	//}
	//if (!mBallistaActive)
	//{
	//	static float radiusThreshold = 10;
	//	static float angleThreshold = 20;

	//	if (mCurrentAngle > (mRootAngle + angleThreshold))
	//	{
	//		mCurrentAngle -= dt;
	//	}

	//	else if (mCurrentAngle < (mRootAngle - angleThreshold))
	//	{
	//		mCurrentAngle += dt;
	//	}

	//	if (mCurrentRadius > (ScaleConstants::RING_RADIUS + radiusThreshold))
	//	{
	//		mCurrentRadius -= (dt*100);
	//	}

	//	else if (mCurrentAngle < (ScaleConstants::RING_RADIUS - radiusThreshold))
	//	{
	//		mCurrentRadius += (dt*100);
	//	}

	//	mSprite->setPosition(Helpers::placeAtAngle(mCurrentAngle, mCurrentRadius));

	//	log("\n%f %f", mCurrentAngle, mCurrentRadius);
	//}

	mUpgradeTime -= dt;

	float percentage = (mUpgradeTime / UpgradeCosts::UpgradeTime) * 100;

	mProgressTimer->setPercentage(100 - percentage);

	if (percentage <= 0)
	{
		upgrade();
		mUpgradeTime = UpgradeCosts::UpgradeTime;
		this->unscheduleUpdate();
		mProgressTimer->setVisible(false);
	}

	//damage(1);
}

void Tower::enterCooldown()
{
	mActDeactCtr++;
    mSprite->setOpacity(128);

	mState = COOLDOWN;
}

void Tower::exitCooldown()
{	
	mActDeactCtr--;
	if (mActDeactCtr <= 0)
	{
		mActDeactCtr = 0;
		mSprite->setOpacity(255);
	}	
	
	mState = ACTIVE;
}

void Tower::shield()
{
	if (!mShield)
	{		
		mShield = Sprite::create("Images/Shield.png");

		auto size = mSprite->getContentSize() * mSprite->getScale();
		auto mySize = mShield->getContentSize();
		auto x = size.width / mySize.width;
		auto y = size.height / mySize.height;

		mShield->setScale((x + y) / 2.f);
		mShield->setAnchorPoint(Vec2(0.5, 0.5));
		mShield->setPosition(Vec2(size.width / 2, size.height / 2));
		mSprite->addChild(mShield);
	}
}

void Tower::damage(float dmg)
{
	if (mCanTakeDamage && mCurrentHealth > 0)
	{
		mCanTakeDamage = false;
		this->schedule([this](float a) {this->mCanTakeDamage = true; }, 2, "Shields Off");

		if (mShield)
		{
			mSprite->removeChild(mShield, true);
			mShield = nullptr;
			return;
		}

		mCurrentHealth = max(0.f, mCurrentHealth - dmg);
		int healthPercent = (mCurrentHealth * 100) / mMaximumHealth;

		if (33 < healthPercent && healthPercent <= 66)
		{
			Audio::getInstance()->playEffect(FilePaths::Audio::SFX::Tower::Damaged);

			if (mCategory == TowerCategory::Defense && mLevel == 0)
				mSprite->setSpriteFrame("_shieldTower0102.png");
			else if (mCategory == TowerCategory::Offense && mLevel == 0)
				mSprite->setSpriteFrame("_attackTower0102.png");
			else if (mCategory == TowerCategory::Offense && mLevel == 1)
				mSprite->setSpriteFrame("_attackTower0202.png");
		}

		else if (0 < healthPercent && healthPercent <= 33)
		{
			Audio::getInstance()->playEffect(FilePaths::Audio::SFX::Tower::Damaged);

			if (mCategory == TowerCategory::Defense && mLevel == 0)
				mSprite->setSpriteFrame("_shieldTower0103.png");
			else if (mCategory == TowerCategory::Offense && mLevel == 0)
				mSprite->setSpriteFrame("_attackTower0103.png");
			else if (mCategory == TowerCategory::Offense && mLevel == 1)
				mSprite->setSpriteFrame("_attackTower0203.png");
		}

		else if (mCurrentHealth <= 0)
		{
			Audio::getInstance()->playEffect(FilePaths::Audio::SFX::Tower::Destroyed);

			((TowerManager*)this->getParent())->removeTower(this, mQuadrant);
			unscheduleAllCallbacks();
			removeFromParentAndCleanup(true);
		}				
	}

	log("\n%d %d", mMaximumHealth, mCurrentHealth);
}

void Tower::rotate()
{
	auto rotateBy = RotateTo::create(0.5, 90 - CC_RADIANS_TO_DEGREES(mPosition.getAngle()));
	mSprite->runAction(rotateBy);

	auto fadeTo = FadeTo::create(0.5, 0);
	mUpgradeCostImage->runAction(fadeTo);

	auto moveBy = MoveBy::create(0.5, Vec2(0, 60));
	mUpgradeCostImage->runAction(moveBy);
}

void Tower::straighten()
{
	auto rotateBy = RotateTo::create(0.5, 0.f);
	mSprite->runAction(rotateBy);

	auto fadeTo = FadeTo::create(0.5, 255);
	mUpgradeCostImage->runAction(fadeTo);
	
	auto moveBy = MoveBy::create(0.5, Vec2(0,-60));
	mUpgradeCostImage->runAction(moveBy);
}

void Tower::moveTo(const Vec2& pos)
{
	mPosition = pos;
	//auto moveTo = MoveTo::create(0.25,mPosition);
	//mSprite->runAction(moveTo);
	auto moveTo = MoveTo::create(0.25, mPosition);
	//auto easeIn = EaseElasticOut::create(moveTo->clone());
	mSprite->runAction(moveTo);
	mUpgradeCostImage->setPosition(mPosition + Vec2(0,-60));
}


/************************************************ 
*				-----------------				*
*				TOWER DEFINITIONS				*
*				-----------------				*
*************************************************/

NormalTower::NormalTower(Vec2 position, int quadrant, bool straight)
{
	mSprite = Sprite::createWithSpriteFrameName("_attackTower0101.png");
	mPhysicsFile = "_attackTower";

	mUpgradeCostImage = Sprite::create("Images/Core/attackBtnUpgrade.png");

	mBaseAggro = 1;
	mMaximumHealth = mCurrentHealth = 2;
	
	mUpgradeCost = { 3 };
	mCategory = TowerCategory::Offense;
	mSpawnStraight = straight;

	mPosition = position;
	mSpriteFilename = FilePaths::AttackTower;

	mQuadrant = quadrant;
	
	mBallista = Ballista::create(this);
	mBallista->setSpriteFilenames(FilePaths::AttackBallista, FilePaths::AttackDirMarker, FilePaths::AttackProjectile);
	mBallista->setSlingAction([](BallisticInfo info) { return NormalProjectile::create(info); });
	mBallista->setCoolDown(1.5);
	mBallista->setDamage(1);

	superInit();	
}

int NormalTower::getBuildCost()
{
	return NormalTower::sBuildCost;
}

void NormalTower::upgrade()
{
	if (canUpgrade())
	{
		static GameScene* currentGameScene = dynamic_cast<GameScene*>(this->getScene());
		
		//Spend Appropriate Amount Essence to Upgrade
		switch (mLevel)
		{
		case 0:
			if (mCategory == TowerCategory::Defense)
				currentGameScene->useEssence(UpgradeCosts::DefenseL1);
			else if (mCategory == TowerCategory::Offense)
				currentGameScene->useEssence(UpgradeCosts::OffenseL1);
			break;
		default: break;
		}

		mSprite->setSpriteFrame("_attackTower0201.png");
		mSprite->setAnchorPoint(Vec2(0.5, 0.25));

		mMaximumHealth = mCurrentHealth = 3;
		mBallista->setCoolDown(1);
		mBallista->setDamage(2);
		mBallista->setSlingAction([](BallisticInfo info) 
		{ 
			auto proj = NormalProjectile::create(info); 
			proj->setImpulse(20000); 
			return proj;
		});

		mBaseAggro = 4;

		//Change Physics File to updated physics body
		mPhysicsFile = "_attackTower2";
		mSprite->removeComponent(mPhysicsBody);
		mPhysicsBody = PhysicsShapeCache::getInstance()->setBodyOnSprite(mPhysicsFile, mSprite);
		mLevel++;

		if (mShield)
		{
			mSprite->removeChild(mShield, true);
			mShield = nullptr;
			shield();
		}
	}
}


ShieldTower::ShieldTower(Vec2 position, int quadrant, bool straight)
{
	mSprite = Sprite::createWithSpriteFrameName("_shieldTower0101.png");
	mPhysicsFile = "_shieldTower";

	mUpgradeCostImage = Sprite::create("Images/Core/shieldBtnUpgrade.png");
	
	mBaseAggro = 0;
	mMaximumHealth = mCurrentHealth = 2;

	mUpgradeCost = {};
	mCategory = TowerCategory::Defense;
	mSpawnStraight = straight;

	mPosition = position;
	mSpriteFilename = FilePaths::ShieldTower;

	mQuadrant = quadrant;
	
	mBallista = Ballista::create(this);
	mBallista->setSpriteFilenames(FilePaths::ShieldBallista, FilePaths::ShieldDirMarker, FilePaths::ShieldProjectile);
	mBallista->setSlingAction([](BallisticInfo info) { return ShieldProjectile::create(info); });
	mBallista->setCoolDown(2.5);
	mBallista->setDamage(0);

	superInit();	
}

int ShieldTower::getBuildCost()
{
	return ShieldTower::sBuildCost;
}

void ShieldTower::upgrade()
{
	if (canUpgrade())
	{
		mSprite->setSpriteFrame("_shieldTower0101.png");
		mLevel++;
	}
	mSprite->setAnchorPoint(Vec2(0.5, 0.5));
}


