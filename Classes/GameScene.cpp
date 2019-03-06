#include "GameScene.h"
#include <functional>
#include "Types.h"
#include "EndScene.h"
#include "PhysicsShapeCache.h"

GameScene::GameScene() : mCollisionHandler(this), mMultiTouchCounter(0), mTowerManager(nullptr)
{}

GameScene::~GameScene()
{
	for (auto itr : Tentacle::mShapes)
		for (auto jtr : itr)
			for (auto ktr : jtr)
				ktr->release();

	for (auto itr : Head::mShapes)
		for (auto jtr : itr)
			for (auto ktr : jtr)
				ktr->release();
}

GameScene* GameScene::create()
{
	GameScene *pRet = new(std::nothrow) GameScene();
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

bool GameScene::init()
{
	Scene::initWithPhysics();
	_physicsWorld->setGravity(Vec2(0, 0));

	setAnchorPoint(Vec2(0, 0));
	setPosition(0, 0);

	mUI.mLoading = Label::createWithTTF("Loading...", FilePaths::Fonts::StandardFont, 30);
	mUI.mLoading->setAnchorPoint(Vec2(0.5, 0.5));
	mUI.mLoading->setTextColor(Color4B().WHITE);
	mUI.mLoading->setPosition(Vec2(1920 / 2.f, 1080 / 2.f));
	addChild(mUI.mLoading, 5);

#ifdef _DEBUG
	this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	loadGameScene();
#else
	ccSchedulerFunc loader = [this](float a) {loadGameScene(); };
	scheduleOnce(loader, .1f, "loader");
#endif	

	scheduleUpdate();
	return true;
}

//Getter Function for the Tower Manager aka. TowerManager
TowerManager* GameScene::getTowerManager()
{
	return mTowerManager;
}

//Getter Function for the UI aka. Core
Core* GameScene::getCore()
{
	return mCore;
}

void GameScene::spawnEssence(Vec2 position)
{
	Gem* essence = Gem::create(position);
	mGems.push_back(essence);
	addChild(essence);
}

GameUI * GameScene::getGameUI()
{
	return mUI.mGameUI;
}

void GameScene::zoomOut(float delay)
{
	if (mUI.mCameraLock)
		return;

	mUI.mCameraLock = true;
	mUI.mIsZoomedOut = true;
	
	this->getDefaultCamera()->lookAt(Vec3(0, 0, 0), Vec3::ZERO);

	auto zoomOutAction = MoveTo::create(delay, Vec3(0, 0, 1870));
	auto pauseAction = CallFunc::create([this]() {
		Director::getInstance()->pause(); 
		_physicsWorld->setSpeed(0);
		mUI.mCameraLock = false;
	});

	this->getDefaultCamera()->runAction(Sequence::create(zoomOutAction, pauseAction, nullptr));
	mUI.mOverlayBackground->runAction(FadeIn::create(delay));	
}

void GameScene::dolly(Vec2 position)
{
	if (mUI.mCameraLock)
		return;

	mUI.mCameraLock = true;
	auto moveBy = MoveBy::create(0.5, position);
	auto cameraUnlock = CallFunc::create([this]() {mUI.mCameraLock = false; });
	this->getDefaultCamera()->runAction(Sequence::create(moveBy, cameraUnlock, nullptr));
}

void GameScene::zoomIn(int quadrant)
{
	if (mUI.mCameraLock)
		return;

	if (mUI.mLevelInfo)
	{
		removeChild(mUI.mLevelInfo);
		removeChild(mUI.mLevelInfoTooltip);
		mUI.mLevelInfo = nullptr;
		mUI.mLevelInfoTooltip = nullptr;

		mUI.mOverlayBackground->clear();
		mUI.mOverlayBackground->drawPolygon(mUI.mOverlayRectangle, 4, Color4F(1, 1, 1, 0.25), 1, Color4F(0, 0, 0, 1));
	}

	mUI.mIsZoomedOut = false;
	Director::getInstance()->resume();
	_physicsWorld->setSpeed(1);
		
	this->getDefaultCamera()->stopAllActions();

	float x = 1920 / 2;
	if (quadrant == 0 || quadrant == 3)
		x *= -1;

	float y = 1080 / 2;
	if (quadrant == 2 || quadrant == 3)
		y *= -1;

	this->getDefaultCamera()->lookAt(Vec3(x, y, 0), Vec3::ZERO);
	mUI.mCameraLock = true;
	
	auto moveCamera = MoveTo::create(0.3, Vec3(x, y, 1870 / 2));
	auto disableFlag = CallFunc::create([this]() {mUI.mCameraLock = false; });
	auto cameraSequence = Sequence::create(moveCamera, disableFlag, nullptr);
	this->getDefaultCamera()->runAction(cameraSequence);

	mUI.mOverlayBackground->runAction(FadeOut::create(0.3));	
}

int GameScene::getEssenceAmount()
{
	return mUI.mGameUI->getAmount();
}

bool GameScene::useEssence(int amount)
{
	return mUI.mGameUI->decrease(amount);
}

vector<Tower*> GameScene::getTowers(int quadrant)
{
	return mTowerManager->getTowers(quadrant);
}

void GameScene::update(float dt)
{
	if (mTowerManager)
	{
		bool defeated = true;
		for (int i = 0; i < 4; i++)
		{
			if (mTowerManager->getTowers(i).size() != 0)
				defeated = false;
		}

		if (defeated)
			endGame(EndGameOutcome::Defeat);
	}	
}

void GameScene::endGame(EndGameOutcome outcome)
{
	Director::getInstance()->pushScene(EndScene::create(outcome));
}

void GameScene::initUI()
{
	mUI.mCamera = Camera::createOrthographic(1920, 1080, 1, 1000);
	mUI.mCamera->lookAt(Vec3(0, 0, 0), Vec3::ZERO);
	mUI.mCamera->setPosition(0, 0);
	mUI.mCamera->setPositionZ(500);
	mUI.mCamera->setCameraFlag(CameraFlag::USER1);
	addChild(mUI.mCamera);

	mUI.mOverlayBackground = DrawNode::create();
	mUI.mOverlayBackground->drawPolygon(mUI.mOverlayRectangle, 4, Color4F(1, 1, 1, 0.75), 1, Color4F(0, 0, 0, 1));
	mUI.mOverlayBackground->setCameraMask((unsigned short)CameraFlag::USER1, true);
	addChild(mUI.mOverlayBackground);

	mUI.mGameUI = GameUI::create();
	mUI.mGameUI->setCameraMask((unsigned short)CameraFlag::USER1, true);
	addChild(mUI.mGameUI);

	mUI.mLevelInfoTooltip = Label::createWithTTF("", FilePaths::Fonts::StandardFont, 60);
	mUI.mLevelInfoTooltip->setAnchorPoint(Vec2(0.5, 0.5));
	mUI.mLevelInfoTooltip->setTextColor(Color4B().RED);
	mUI.mLevelInfoTooltip->setPosition(Vec2(1920 / 2.f, 1080 / 2.f + 40));
	mUI.mLevelInfoTooltip->setCameraMask((unsigned short)CameraFlag::USER1, true);
	addChild(mUI.mLevelInfoTooltip, 5);

	mUI.mLevelInfo = Label::createWithTTF("Tap on a Quadrant to Start!", FilePaths::Fonts::StandardFont, 45);
	mUI.mLevelInfo->setAnchorPoint(Vec2(0.5, 0.5));
	mUI.mLevelInfo->setTextColor(Color4B().RED);
	mUI.mLevelInfo->setPosition(Vec2(1920 / 2.f, 1080 / 2.f - 40));
	mUI.mLevelInfo->setCameraMask((unsigned short)CameraFlag::USER1, true);
	addChild(mUI.mLevelInfo, 5);
}

