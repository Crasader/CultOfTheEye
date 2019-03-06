#pragma once
#include <vector>
#include <thread>
#include <memory>

#include "cocos2d.h"

#include "Audio.h"
#include "Tower.h"
#include "Projectile.h"
#include "Core.h"
#include "CollisionHandler.h"
#include "MonsterManager.h"
#include "Monster.h"
#include "Gem.h"
#include "ResourceManager.h"
#include "TowerManager.h"

using namespace std;
using namespace cocos2d;

struct GameSceneUI
{
	bool		mCameraLock = false;
	bool		mIsZoomedOut;
	Camera*		mCamera;

	Label*		mLevelInfoTooltip	= nullptr;
	Label*		mLevelInfo			= nullptr;
	Label*		mLoading;

	GameUI*	mGameUI;

	DrawNode*	mOverlayBackground;
	const Vec2	mOverlayRectangle[4] = 
	{
		Vec2(0, 0), Vec2(1920, 0), Vec2(1920, 1080), Vec2(0, 1080)
	};

};

class GameScene : public cocos2d::Scene
{
	vector<Gem*>	mGems;	
	TowerManager*	mTowerManager;
	MonsterManager*	mMonsterManager;

	Audio			mAudioManager;

	Core*	mCore;
	CollisionHandler mCollisionHandler;

	bool mIsPlayerNavigating;	
	int	 mMultiTouchCounter;

	GameSceneUI mUI;
public:  
	Core*			getCore();
	TowerManager*	getTowerManager();
	vector<Tower*>	getTowers(int quadrant);	
	
	bool	init();
	void	initUI();
	void	initUserInput();	
	void	loadGameScene();
	void	loadPhysicsInfo();
	void	loadSpriteFrames();

	void update(float dt);

	int		getEssenceAmount();
	bool	useEssence(int amount);
	void	spawnEssence(Vec2 position);
	GameUI*	getGameUI();
	
	void	dolly(Vec2 position);
	void	zoomIn(int quadrant);
	void	zoomOut(float delay = 0.5f);

	enum class	EndGameOutcome {Victory, Defeat};
	void		endGame(EndGameOutcome outcome);
	
	GameScene();
	~GameScene();
	static GameScene* create();	
};
