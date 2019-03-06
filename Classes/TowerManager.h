#pragma once
#include "cocos2d.h"
#include "Tower.h"
#include "Types.h"
#include <vector>
#include <bitset>

using namespace cocos2d;

class Debris;

class TowerManager : public Node
{
public:
	TowerManager();
	~TowerManager();

	vector<Tower*> getTowers(int quadrant);
	void removeTower(Tower* tower, int quadrant);

	Vec2 spawnTowerAt(Vec2 pos, eTowerType type);

	void rotateTowers();
	void straightenTowers();

	void activateTowers();
	void deactivateTowers();

	void indicateTowerUpgradability();
	void exitUpgradeMode();
 
	bool containsPointOnTower(Vec2 point);

	bool isQuadrantAlive(int quadrant);

	bool isQuadrantFull(int quadrant);

	void spawnDebrisAt(Vec2 pos, TowerCategory type, int level);

	bool isSlotAvailable(int slotIndex);

	CREATE_FUNC(TowerManager);

private:
	array<vector<Tower*>,4>	mTowers;
	vector<Debris*> mDebris;
	bitset<24> towerSlots;
};