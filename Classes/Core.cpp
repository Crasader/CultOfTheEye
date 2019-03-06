#include "Core.h"
#include "Monster.h"
#include "Types.h"
#include "GameScene.h"
#include "PhysicsShapeCache.h"
#include "Helpers.h"
#include "UIButton.h"

Core::Core()
{
	setName(ClassStrings::Core);

	CreateFogSprites();
	CreateCoreSprites();
	CreateRingSprites();
	//CreateSlotIndicators();

	mIsUIActive = false;
	mUIInit = false;
	mIsCreatingTower = false;
	touchCounter = 0;

	isQuadrantAlive.set();
	isQuadrantFull.reset();

	//auto winSize = Director::getInstance()->getVisibleSize();
	//mMenuMessage = Label::createWithTTF("0", FilePaths::Fonts::StandardFont, 75);
	//mMenuMessage->setAnchorPoint(Vec2(0.5, 0.5));
	//mMenuMessage->setTextColor(Color4B().WHITE);
	//mMenuMessage->setPosition(Vec2(winSize/2,winSize/2));
	//mMenuMessage->setVisible(false);
	//addChild(mMenuMessage);

	scheduleUpdate();

	auto uiTouchListener = EventListenerTouchOneByOne::create();

	uiTouchListener->onTouchBegan = CC_CALLBACK_2(Core::onTouchBegan, this);
	uiTouchListener->onTouchMoved = CC_CALLBACK_2(Core::onTouchMoved, this);
	uiTouchListener->onTouchEnded = CC_CALLBACK_2(Core::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(uiTouchListener, this);
}

Core::~Core()
{}

void Core::update(float dt)
{
	static GameScene* activeScene = dynamic_cast<GameScene*>(this->getScene());

	if (mUIInit && mIsUIActive)
	{
		activeScene->getTowerManager()->indicateTowerUpgradability();
	}

	for (int i = 0; i < QuadrantCount; i++)
	{
		if (!((activeScene->getTowerManager()->isQuadrantAlive(i))) && (isQuadrantAlive[i]))
		{
			isQuadrantAlive[i] = false;
			
			if(mIsUIActive)	
				closeUI();
			
			switch (i)
			{
			case 0:
				mCoreSprites[i]->setSpriteFrame("Core_destroyed_1_topLeft_1.png");
				mRingSprites[i]->setColor(Color3B(93, 15, 188));
				break;
			case 1:
				mCoreSprites[i]->setSpriteFrame("Core_destroyed_1_topRight_1.png");
				mRingSprites[i]->setColor(Color3B(93, 15, 188));
				break;
			case 2:
				mCoreSprites[i]->setSpriteFrame("Core_destroyed_1_botRight_1.png");
				mRingSprites[i]->setColor(Color3B(93, 15, 188));
				break;
			case 3:
				mCoreSprites[i]->setSpriteFrame("Core_destroyed_1_botLeft_1.png");
				mRingSprites[i]->setColor(Color3B(93, 15, 188));
				break;
			default: break;
			}
			
			//if (!mIsUIActive)
			//	mPhysicsBody[i]->setEnabled(false);
			//mCoreSprites[i]->removeComponent(mPhysicsBody[i]);
		}

		isQuadrantFull[i] = activeScene->getTowerManager()->isQuadrantFull(i);
		
		if (isQuadrantFull[i])
		{
			//mMenuMessage->setVisible(true);
		}
	}

	if (mUIInit)
	{
		for (int i = 0; i < mUIButtons.size(); i++)
		{
			if ((!(isQuadrantAlive[i / 2])) || (isQuadrantFull[i/2]))
			{
				mUIButtons[i]->deactivate();
			}
		}
	}
}

bool Core::containsPointOnPlanet(Vec2 point)
{
	auto radius = 964;
	
	//Should be triggered only at the core
	radius /= 4;

	auto hype = sqrt((point.x*point.x) + (point.y*point.y));

	if (hype <= radius)
	{
		return true;
	}
	
	return false;
}

//Service function for the GameScene to prevent accidental swipes
bool Core::containsPointWithinRing(Vec2 point)
{
	auto radius = ScaleConstants::RING_RADIUS;

	auto hype = sqrt((point.x*point.x) + (point.y*point.y));

	if (hype < radius)
	{
		return true;
	}

	return false;
}

bool Core::containsPointOnRing(Vec2 point)
{
	auto radius = ScaleConstants::RING_RADIUS;

	auto hype = sqrt((point.x*point.x) + (point.y*point.y));

	if (hype >= radius)
	{
		return true;
	}

	return false;	
}

inline bool Core::uiClosed(Vec2 initialLocation, Vec2 finalLocation)
{
	static GameScene* activeScene = dynamic_cast<GameScene*>(this->getScene());

	if (activeScene->getTowerManager()->containsPointOnTower(finalLocation) || containsPointOnButton(initialLocation))
		return false;
	
	return (initialLocation == finalLocation);
}

inline bool Core::uiOpened(Vec2 initialLocation, Vec2 finalLocation)
{
	return (containsPointOnPlanet(initialLocation) && containsPointOnPlanet(finalLocation));
}

bool Core::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

void Core::onTouchMoved(Touch* touch, Event* event)
{}

void Core::onTouchEnded(Touch* touch, Event* event)
{
	GameScene* activeScene = (GameScene*)this->getScene();

	//Variables for Storing Touch
	float t = touch->getStartLocation().x;
	float u = touch->getStartLocation().y;
	float t1 = touch->getLocation().x;
	float u1 = touch->getLocation().y;

	auto cam = activeScene->getDefaultCamera()->getPosition();
	if (cam.x < 0)
	{
		t -= 1920;
		t1 -= 1920;
	}

	if (cam.y < 0)
	{
		u -= 1080;
		u1 -= 1080;
	}

	auto initialWorldLocation = Vec2(t, u);
	auto finalWorldLocation = Vec2(t1, u1);

	if (!mUIInit && uiOpened(initialWorldLocation,finalWorldLocation))
	{		
		initUI();
	}

	else if (!mIsUIActive && uiOpened(initialWorldLocation, finalWorldLocation))
	{
		openUI();
	}

	else if (mIsUIActive && uiClosed(initialWorldLocation, finalWorldLocation))
	{
		closeUI();
	}
}

void Core::initUI()
{
	static uint8_t towerCount = ((Quadrants::QuadrantCount)*(eTowerType::TOWER_COUNT));
	static GameScene* activeScene = (GameScene*)this->getScene();

	static uint8_t initAngle = 180;
	static int angle = initAngle;
	static int8_t angleIncrement = -30;

	//mPhysicsBody[0]->setEnabled(false);
	//mPhysicsBody[1]->setEnabled(false);
	//mPhysicsBody[2]->setEnabled(false);
	//mPhysicsBody[3]->setEnabled(false);

	//for (int i = 0; i < mSlotIndicators.size(); i++)
	//{
	//	if (!activeScene->getTowerManager()->isSlotAvailable(i))
	//	{
	//		SetSlotOccupied(i);
	//	}
	//}

	//for (int i = 0; i < mSlotIndicators.size(); i++)
	//{
	//	//if (activeScene->getTowerManager()->isSlotAvailable(i))
	//		//mSlotIndicators[i]->resumeEmissions();
	//}

	if (isQuadrantAlive[0])
	{
		mCoreSprites[0]->setSpriteFrame("core_UI_topLeft_1.png");
		mCoreSprites[0]->setAnchorPoint(Vec2(1, 0));
	}

	if (isQuadrantAlive[1])
	{
		mCoreSprites[1]->setSpriteFrame("core_UI_topRight_1.png");
		mCoreSprites[1]->setAnchorPoint(Vec2(0, 0));
	}

	if (isQuadrantAlive[2])
	{
		mCoreSprites[2]->setSpriteFrame("core_UI_botRight_1.png");
		mCoreSprites[2]->setAnchorPoint(Vec2(0, 1));
	}

	if (isQuadrantAlive[3])
	{
		mCoreSprites[3]->setSpriteFrame("core_UI_botLeft_1.png");
		mCoreSprites[3]->setAnchorPoint(Vec2(1, 1));
	}

	CreateButtons();

	for (int i = 0; i < towerCount; i++)
	{
		if (isQuadrantAlive[i / 2])
		{
			mUIButtons[i]->activate();
		}
	}

	activeScene->getTowerManager()->straightenTowers();
	activeScene->getTowerManager()->indicateTowerUpgradability();

	activeScene->getTowerManager()->deactivateTowers();

	mIsUIActive = true;
	mUIInit = true;
}

void Core::openUI()
{
	static GameScene* activeScene = (GameScene*)this->getScene();
	static uint8_t towerCount = ((Quadrants::QuadrantCount)*(eTowerType::TOWER_COUNT));

	//for (int i = 0; i < mSlotIndicators.size(); i++)
	//{
	//	//if (activeScene->getTowerManager()->isSlotAvailable(i))
	//		//mSlotIndicators[i]->resumeEmissions();
	//}

	if (isQuadrantAlive[0])
	{
		//mPhysicsBody[0]->setEnabled(false);
		mCoreSprites[0]->setSpriteFrame("core_UI_topLeft_1.png");
		mCoreSprites[0]->setAnchorPoint(Vec2(1, 0));
	}
	if (isQuadrantAlive[1])
	{
		//mPhysicsBody[1]->setEnabled(false);
		mCoreSprites[1]->setSpriteFrame("core_UI_topRight_1.png");
		mCoreSprites[1]->setAnchorPoint(Vec2(0, 0));
	}
	if (isQuadrantAlive[2])
	{
		//mPhysicsBody[2]->setEnabled(false);
		mCoreSprites[2]->setSpriteFrame("core_UI_botRight_1.png");
		mCoreSprites[2]->setAnchorPoint(Vec2(0, 1));
	}
	if (isQuadrantAlive[3])
	{
		//mPhysicsBody[3]->setEnabled(false);
		mCoreSprites[3]->setSpriteFrame("core_UI_botLeft_1.png");
		mCoreSprites[3]->setAnchorPoint(Vec2(1, 1));
	}

	for (int i = 0; i < towerCount; i++)
	{
		if (isQuadrantAlive[i / 2])
		{
			mUIButtons[i]->activate();
		}
	}

	activeScene->getTowerManager()->straightenTowers();
	activeScene->getTowerManager()->indicateTowerUpgradability();

	activeScene->getTowerManager()->deactivateTowers();

	mIsUIActive = true;
}

void Core::closeUI()
{
	static GameScene* activeScene = (GameScene*)this->getScene();
	static uint8_t towerCount = ((Quadrants::QuadrantCount)*(eTowerType::TOWER_COUNT));

	//for (int i = 0; i < mSlotIndicators.size(); i++)
	//{
	//	mSlotIndicators[i]->pauseEmissions();
	//}
	
	if (isQuadrantAlive[0])
	{
		mCoreSprites[0]->setSpriteFrame("core_fullHealth_topLeft_1.png");
		mCoreSprites[0]->setAnchorPoint(Vec2(1, 0));
		//mPhysicsBody[0]->setEnabled(true);
	}

	if (isQuadrantAlive[1])
	{
		mCoreSprites[1]->setSpriteFrame("core_fullHealth_topRight_1.png");
		mCoreSprites[1]->setAnchorPoint(Vec2(0, 0));
		//mPhysicsBody[1]->setEnabled(true);
	}

	if (isQuadrantAlive[2])
	{
		mCoreSprites[2]->setSpriteFrame("core_fullHealth_botRight_1.png");
		mCoreSprites[2]->setAnchorPoint(Vec2(0, 1));
		//mPhysicsBody[2]->setEnabled(true);
	}

	if (isQuadrantAlive[3])
	{
		mCoreSprites[3]->setSpriteFrame("core_fullHealth_botLeft_1.png");
		mCoreSprites[3]->setAnchorPoint(Vec2(1, 1));
		//mPhysicsBody[3]->setEnabled(true);
	}

	for (int i = 0; i < towerCount; i++)
	{
		mUIButtons[i]->deactivate();
	}

	activeScene->getTowerManager()->rotateTowers();
	activeScene->getTowerManager()->exitUpgradeMode();

	activeScene->getTowerManager()->activateTowers();

	mIsUIActive = false;
}

void Core::CreateCoreSprites()
{
	mCoreSprites[0] = Sprite::createWithSpriteFrameName("core_fullHealth_topLeft_1.png");
	mCoreSprites[0]->setScale(ScaleConstants::CORE_SCALE);
	addChild(mCoreSprites[0]);

	mCoreSprites[1] = Sprite::createWithSpriteFrameName("core_fullHealth_topRight_1.png");
	mCoreSprites[1]->setScale(ScaleConstants::CORE_SCALE);
	addChild(mCoreSprites[1]);

	mCoreSprites[2] = Sprite::createWithSpriteFrameName("core_fullHealth_botRight_1.png");
	mCoreSprites[2]->setScale(ScaleConstants::CORE_SCALE);
	addChild(mCoreSprites[2]);

	mCoreSprites[3] = Sprite::createWithSpriteFrameName("core_fullHealth_botLeft_1.png");
	mCoreSprites[3]->setScale(ScaleConstants::CORE_SCALE);
	addChild(mCoreSprites[3]);

	//mCoreSprites[4] = Sprite::create();
	//addChild(mCoreSprites[4]);

	//mPhysicsBody[0] = PhysicsShapeCache::getInstance()->createBodyWithName("CoreTopLeft");
	//mPhysicsBody[1] = PhysicsShapeCache::getInstance()->createBodyWithName("CoreTopRight");
	//mPhysicsBody[2] = PhysicsShapeCache::getInstance()->createBodyWithName("CoreBottomRight");
	//mPhysicsBody[3] = PhysicsShapeCache::getInstance()->createBodyWithName("CoreBottomLeft");
	//mPhysicsBody[4] = PhysicsShapeCache::getInstance()->createBodyWithName("Core");

	//mCoreSprites[0]->addComponent(mPhysicsBody[0]);
	//mCoreSprites[1]->addComponent(mPhysicsBody[1]);
	//mCoreSprites[2]->addComponent(mPhysicsBody[2]);
	//mCoreSprites[3]->addComponent(mPhysicsBody[3]);
	//mCoreSprites[4]->addComponent(mPhysicsBody[4]);

	//mPhysicsBody[0]->setPositionOffset(Vec2(550, -150));
	//mPhysicsBody[1]->setPositionOffset(Vec2(-550, -150));
	//mPhysicsBody[2]->setPositionOffset(Vec2(-550, 150));
	//mPhysicsBody[3]->setPositionOffset(Vec2(550, 150));
	//mPhysicsBody[4]->setPositionOffset(Vec2(-400, 400));
	mCoreSprites[0]->setAnchorPoint(Vec2(1, 0));
	mCoreSprites[1]->setAnchorPoint(Vec2(0, 0));
	mCoreSprites[2]->setAnchorPoint(Vec2(0, 1));
	mCoreSprites[3]->setAnchorPoint(Vec2(1, 1));
}

void Core::CreateFogSprites()
{
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> fogFrames;
	char str[100];

	for (int i = 1; i <= 64; i++)
	{
		sprintf(str, "smoke%d.png", i);
		fogFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}

	Animation *anim = Animation::createWithSpriteFrames(fogFrames, AnimationConstants::FOG_SPEED);

	mFogSprite = Sprite::createWithSpriteFrame(fogFrames.front());
	mFogSprite->runAction(RepeatForever::create(Animate::create(anim)));
	mFogSprite->setScale(ScaleConstants::FOG_SCALE);
	addChild(mFogSprite, -1);
}

void Core::CreateRingSprites()
{
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> ringFrames1;
	Vector<SpriteFrame*> ringFrames2;
	Vector<SpriteFrame*> ringFrames3;
	Vector<SpriteFrame*> ringFrames4;
	char str[100];

	for (int i = 1; i <= 64; i++)
	{
		sprintf(str, "ring_top_left_%02d.png", i);
		ringFrames1.pushBack(spritecache->getSpriteFrameByName(str));
	}

	Animation *anim1 = Animation::createWithSpriteFrames(ringFrames1, AnimationConstants::RING_SPEED);

	mRingSprites[0] = Sprite::createWithSpriteFrame(ringFrames1.front());
	mRingSprites[0]->runAction(RepeatForever::create(Animate::create(anim1)));
	mRingSprites[0]->setAnchorPoint(Vec2(1, 0));
	mRingSprites[0]->setPosition(0, ScaleConstants::TOP_QUADRANT_Y_OFFSET);
	mRingSprites[0]->setScale(ScaleConstants::RING_SCALE);
	addChild(mRingSprites[0]);

	for (int i = 1; i <= 64; i++)
	{
		sprintf(str, "ring_top_right_%02d.png", i);
		ringFrames2.pushBack(spritecache->getSpriteFrameByName(str));
	}

	Animation *anim2 = Animation::createWithSpriteFrames(ringFrames2, AnimationConstants::RING_SPEED);

	mRingSprites[1] = Sprite::createWithSpriteFrame(ringFrames2.front());
	mRingSprites[1]->runAction(RepeatForever::create(Animate::create(anim2)));
	mRingSprites[1]->setAnchorPoint(Vec2(0, 0));
	mRingSprites[1]->setPosition(0, ScaleConstants::TOP_QUADRANT_Y_OFFSET);
	mRingSprites[1]->setScale(ScaleConstants::RING_SCALE);
	addChild(mRingSprites[1]);

	for (int i = 1; i <= 64; i++)
	{
		sprintf(str, "ring_bottom_right_%02d.png", i);
		ringFrames3.pushBack(spritecache->getSpriteFrameByName(str));
	}

	Animation *anim3 = Animation::createWithSpriteFrames(ringFrames3, AnimationConstants::RING_SPEED);

	mRingSprites[2] = Sprite::createWithSpriteFrame(ringFrames3.front());
	mRingSprites[2]->runAction(RepeatForever::create(Animate::create(anim3)));
	mRingSprites[2]->setPosition(0, ScaleConstants::BOTTOM_QUADRANT_Y_OFFSET);
	mRingSprites[2]->setAnchorPoint(Vec2(0, 1));
	mRingSprites[2]->setScale(ScaleConstants::RING_SCALE);
	addChild(mRingSprites[2]);

	for (int i = 1; i <= 64; i++)
	{
		sprintf(str, "ring_bottom_left_%02d.png", i);
		ringFrames4.pushBack(spritecache->getSpriteFrameByName(str));
	}

	Animation *anim4 = Animation::createWithSpriteFrames(ringFrames4, AnimationConstants::RING_SPEED);

	mRingSprites[3] = Sprite::createWithSpriteFrame(ringFrames4.front());
	mRingSprites[3]->runAction(RepeatForever::create(Animate::create(anim4)));
	mRingSprites[3]->setAnchorPoint(Vec2(1, 1));
	mRingSprites[3]->setPosition(0, ScaleConstants::BOTTOM_QUADRANT_Y_OFFSET);
	mRingSprites[3]->setScale(ScaleConstants::RING_SCALE);
	addChild(mRingSprites[3]);
}

//void Core::CreateDeadRingSprites(int quadrant)
//{
//	auto spritecache = SpriteFrameCache::getInstance();
//	Vector<SpriteFrame*> ringFrames;
//	char str[100];
//
//	for (int i = 1; i <= 16; i++)
//	{
//		sprintf(str, "red ring_cut%d.png", i);
//		ringFrames.pushBack(spritecache->getSpriteFrameByName(str));
//	}
//
//	Animation *anim = Animation::createWithSpriteFrames(ringFrames, AnimationConstants::RING_SPEED);
//
//	switch (quadrant)
//	{
//	case 1:
//		removeChild(mRingSprites[0]);
//		mRingSprites[0] = Sprite::createWithSpriteFrame(ringFrames.front());
//		mRingSprites[0]->runAction(RepeatForever::create(Animate::create(anim)));
//		mRingSprites[0]->setAnchorPoint(Vec2(1, 0));
//		//mRingSprites[0]->setPosition(0, ScaleConstants::TOP_QUADRANT_Y_OFFSET);
//		mRingSprites[0]->setPosition(0, 0);
//		mRingSprites[0]->setScale(ScaleConstants::RING_SCALE);
//		addChild(mRingSprites[0]);
//		break;
//	case 2:
//		removeChild(mRingSprites[1]);
//		mRingSprites[1] = Sprite::createWithSpriteFrame(ringFrames.front());
//		mRingSprites[1]->runAction(RepeatForever::create(Animate::create(anim)));
//		mRingSprites[1]->setAnchorPoint(Vec2(1, 0));
//		//mRingSprites[1]->setPosition(0, ScaleConstants::BOTTOM_QUADRANT_Y_OFFSET);
//		mRingSprites[1]->setPosition(0, 0);
//		mRingSprites[1]->setRotation(90);
//		mRingSprites[1]->setScale(ScaleConstants::RING_SCALE);
//		addChild(mRingSprites[1]);
//		break;
//	case 3:
//		removeChild(mRingSprites[2]);
//		mRingSprites[2] = Sprite::createWithSpriteFrame(ringFrames.front());
//		mRingSprites[2]->runAction(RepeatForever::create(Animate::create(anim)));
//		mRingSprites[2]->setAnchorPoint(Vec2(1, 0));
//		//mRingSprites[2]->setPosition(0, ScaleConstants::BOTTOM_QUADRANT_Y_OFFSET);
//		mRingSprites[2]->setPosition(0, 0);
//		mRingSprites[2]->setRotation(180);
//		mRingSprites[2]->setScale(ScaleConstants::RING_SCALE);
//		addChild(mRingSprites[2]);
//		break;
//	case 4:
//		removeChild(mRingSprites[3]);
//		mRingSprites[3] = Sprite::createWithSpriteFrame(ringFrames.front());
//		mRingSprites[3]->runAction(RepeatForever::create(Animate::create(anim)));
//		mRingSprites[3]->setAnchorPoint(Vec2(1, 0));
//		//mRingSprites[3]->setPosition(0, ScaleConstants::BOTTOM_QUADRANT_Y_OFFSET);
//		mRingSprites[3]->setPosition(0, 0);
//		mRingSprites[3]->setRotation(270);
//		mRingSprites[3]->setScale(ScaleConstants::RING_SCALE);
//		addChild(mRingSprites[3]);
//		break;
//	}
//}

void Core::CreateButtons()
{
	for (int buttonIndex = 1; buttonIndex <= 8; buttonIndex++)
	{
		float angle = -1;

		switch (buttonIndex)
		{
		case 1:
		case 8: angle = 152.5; break;
		case 2:
		case 7: angle = 121.5; break;
		case 3:
		case 6: angle = 58.5; break;
		case 4:
		case 5: angle = 27.5; break;
		}

		if (buttonIndex > 4)
		{
			angle = -angle;
		}

		if (buttonIndex % 2 == 0)
		{
			mUIButtons[buttonIndex - 1] = UIButton::create(eTowerType::NORMAL, Helpers::placeAtAngle(angle, 312));
		}

		else
		{
			mUIButtons[buttonIndex - 1] = UIButton::create(eTowerType::SHIELD, Helpers::placeAtAngle(angle, 312));
		}

		addChild(mUIButtons[buttonIndex - 1]);
	}
}

void Core::CreateSlotIndicators()
{
	//for (int i = 0; i < mSlotIndicators.size(); i++)
	//{
	//	mSlotIndicators[i] = CCParticleSystemQuad::create("Images/slotbeam.plist");
	//	mSlotIndicators[i]->setPosition(Helpers::placeAtAngle((ScaleConstants::SLOT_ANGLE / 2) + (15 * i), ScaleConstants::RING_RADIUS / 2));
	//	mSlotIndicators[i]->setRotation(90 - (ScaleConstants::SLOT_ANGLE / 2 + (15 * i)));
	//	mSlotIndicators[i]->pauseEmissions();
	//	mSlotIndicators[i]->setStartColor(Color4F(1, 0, 0, 0.5));
	//	mSlotIndicators[i]->setEndColor(Color4F(1, 0, 0, 0.5));
	//	this->addChild(mSlotIndicators[i], -1);
	//}
}

void Core::SetSlotFree(int slotIndex)
{
	//this->removeChild(mSlotIndicators[slotIndex]);
	//mSlotIndicators[slotIndex] = CCParticleSystemQuad::create("Images/slotbeam.plist");
	//mSlotIndicators[slotIndex]->setPosition(Helpers::placeAtAngle((ScaleConstants::SLOT_ANGLE / 2) + (15 * slotIndex), ScaleConstants::RING_RADIUS / 2));
	//mSlotIndicators[slotIndex]->setRotation(90 - (ScaleConstants::SLOT_ANGLE / 2 + (15 * slotIndex)));
	//mSlotIndicators[slotIndex]->setStartColor(Color4F(1, 0, 0, 0.5));
	//mSlotIndicators[slotIndex]->setEndColor(Color4F(1, 0, 0, 0.5));
	//this->addChild(mSlotIndicators[slotIndex], -1);

	//if (!mIsUIActive)
	//	mSlotIndicators[slotIndex]->pauseEmissions();
}

void Core::SetSlotOccupied(int slotIndex)
{
	////mSlotIndicators[slotIndex]->pauseEmissions();
	//this->removeChild(mSlotIndicators[slotIndex]);
	//mSlotIndicators[slotIndex] = CCParticleSystemQuad::create("Images/slotbeam.plist");
	//mSlotIndicators[slotIndex]->setPosition(Helpers::placeAtAngle((ScaleConstants::SLOT_ANGLE / 2) + (15 * slotIndex), ScaleConstants::RING_RADIUS / 2));
	//mSlotIndicators[slotIndex]->setRotation(90 - (ScaleConstants::SLOT_ANGLE / 2 + (15 * slotIndex)));
	////mSlotIndicators[slotIndex]->setStartColor(Color4F(0.4, 0, 0, 0.5));
	////mSlotIndicators[slotIndex]->setEndColor(Color4F(0.4, 0, 0, 0.5));
	//this->addChild(mSlotIndicators[slotIndex], -1);
}

bool Core::containsPointOnButton(Vec2 point)
{
	for (int i = 0; i < mUIButtons.size(); i++)
	{
		if (mUIButtons[i]->containsPoint(point) == true)
			return true;
	}
	return false;
}
