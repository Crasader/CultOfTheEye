#include "GameScene.h"
#include <functional>
#include "Types.h"
#include "TitleScene.h"
#include "PhysicsShapeCache.h"

void GameScene::loadGameScene()
{
	//Set Up Background Image
	auto Background = Sprite::create("Images/Background.png");
	Background->setPosition(0, 0);

	//Load Sprites & Physics
	loadSpriteFrames();
	loadPhysicsInfo();

	//Setup Audio
	Audio::getInstance()->playMusic(FilePaths::Audio::Music::AndroidBackground);

	//Set Up Camera
	removeChild(_defaultCamera, true);
	_defaultCamera = Camera::createPerspective(60, (GLfloat)1920 / 1080, 0, 3840);
	addChild(_defaultCamera);
	this->getDefaultCamera()->setPositionZ(1870);
	this->getDefaultCamera()->setPosition(0, 0);
	
	//Initialize UI and User Input
	initUI();
	initUserInput();

	//Setup Core, Monster & Tower Managers
	mCore = Core::create();
	mTowerManager = TowerManager::create();
	mMonsterManager = MonsterManager::create();
	//mUI.mEssence->mMonster = mMonsterManager;

	//Add Children
	removeChild(mUI.mLoading);

	addChild(Background);
	addChild(mCore);
	addChild(mTowerManager);
	addChild(mMonsterManager);

	zoomOut(0);
}

void GameScene::loadPhysicsInfo()
{
	PhysicsShapeCache::getInstance()->addShapesWithFile("physicsData.plist");

	int i = 0;
	for (auto s : FilePaths::Physics::Monster::Tentacle::Strings)
	{
		Tentacle::mShapes.resize(i + 1);
		PhysicsShapeCache::getInstance()->addDynamicShapesWithFile(s.second, s.first);
		PhysicsShapeCache::getInstance()->createDynamicShapesWithName(s.first, Tentacle::mShapes[i]);
		i++;
	}
	Tentacle::mShapes.resize(i + 1);
	PhysicsShapeCache::getInstance()->createDynamicShapesWithName(FilePaths::Physics::Monster::Tentacle::Strings[3].first, Tentacle::mShapes[4]);
	reverse(Tentacle::mShapes[4].begin(), Tentacle::mShapes[4].end());

	i = 0;
	for (auto s : FilePaths::Physics::Monster::Head::Strings)
	{
		Head::mShapes.resize(i + 1);
		PhysicsShapeCache::getInstance()->addDynamicShapesWithFile(s.second, s.first);
		PhysicsShapeCache::getInstance()->createDynamicShapesWithName(s.first, Head::mShapes[i]);
		i++;
	}

	i = 0;
	for (auto s : FilePaths::Physics::Monster::Swim::Strings)
	{
		Body::mShapes.resize(i + 1);
		PhysicsShapeCache::getInstance()->addDynamicShapesWithFile(s.second, s.first);
		PhysicsShapeCache::getInstance()->createDynamicShapesWithName(s.first, Body::mShapes[i]);
		i++;
	}
}

void GameScene::loadSpriteFrames()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/core.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Monster_01/_monsterSwim1.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Monster_01/_monsterSwim2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Monster_01/_tentacleIdle01.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/_attackTower.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/_shieldTower.plist");

	for (auto s : FilePaths::Spritesheets::Monster::Head::Strings)
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s);

	for (auto s : FilePaths::Spritesheets::Monster::Tentacle::Strings)
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s);

	for (auto s : FilePaths::Spritesheets::Monster::Swim::Strings)
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(FilePaths::Spritesheets::Essence::FilePath);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Fog/Fog1.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Fog/Fog2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Fog/Fog3.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Fog/Fog4.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Fog/Fog5.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Fog/Fog6.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Fog/Fog7.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Fog/Fog8.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Ring/Ring1.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Ring/Ring2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Ring/Ring3.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Ring/Ring4.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Ring/Ring5.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Core/Core1.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Core/Core2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Core/Core3.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Images/Core/menuBtns.plist");

}



