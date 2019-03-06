//Display its sprite and essentially acts as a tower manager to create, merge and destroy towers
#include "TowerManager.h"
#include "GameScene.h"
#include "Core.h"
#include "Helpers.h"
#include "SimpleAudioEngine.h"
#include "Debris.h"

using namespace std;
using namespace CocosDenshion;

TowerManager::TowerManager()
{
	setName(ClassStrings::TowerManager);

	towerSlots.reset();

	//for (int i = 0; i < 24; i += 3)
		//towerSlots[i] = true;

	mTowers[0].reserve(4);
	mTowers[1].reserve(4);
	mTowers[2].reserve(4);
	mTowers[3].reserve(4);

	mDebris.reserve(4);

	//Local variables needed for tower placement algorithm
	uint8_t towerCount = Quadrants::QuadrantCount;
	uint8_t initAngle = 180;
	int angle = initAngle;
	uint8_t angleIncrement = 45;

	float ringRadius = ScaleConstants::RING_RADIUS;

	float a1, a2, a3, x1, x2, x3;

	for (int i = 0; i < (towerCount);)
	{
		angle -= angleIncrement;

		if ((abs(angle) % 90) == 0)
			continue;

		mTowers[i].push_back(NormalTower::create(Helpers::placeAtAngle(angle+7.5, ringRadius), i, false));
		addChild(mTowers[i].back());

		if (angle < 0)
			towerSlots[int((360 + angle) / ScaleConstants::SLOT_ANGLE)] = true;

		else
			towerSlots[int((angle) / ScaleConstants::SLOT_ANGLE)] = true;

		i++;
	}
}

TowerManager::~TowerManager()
{}

Vec2 TowerManager::spawnTowerAt(Vec2 pos, eTowerType type)
{
	static GameScene* currentScene = dynamic_cast<GameScene*>(this->getScene());
	
	auto ringRadius = ScaleConstants::RING_RADIUS;

	float intersectionRadius = pos.getDistance(Vec2(0, 0));

	Vec2 reference = Vec2(ringRadius, 0);

	double angle = acos(reference.dot(pos) / (intersectionRadius*ringRadius));
	angle = CC_RADIANS_TO_DEGREES(angle);

	Quadrants currentQuadrant;

	auto cam = currentScene->getDefaultCamera()->getPosition();

	//Let's compute the quadrant and the final angle
	if (cam.x < 0)
	{
		if (cam.y < 0)
		{
			currentQuadrant = D;
			angle = 360 - angle;
		}
		else
		{
			currentQuadrant = A;
		}
	}

	else
	{
		if (cam.y < 0)
		{
			currentQuadrant = C;
			angle = 360 - angle;
		}
		else
		{
			currentQuadrant = B;
		}
	}

	const uint8_t slotAngle = ScaleConstants::SLOT_ANGLE;

	//Round to the nearest multiple of 15
	//angle = round(angle);
	//int remainder = (int(angle)) % slotAngle;
	//if (remainder < (slotAngle/2))
	//	angle -= remainder;
	//else
	//	angle += (slotAngle - remainder);

	int slot = angle / slotAngle;
	
	if (slot == (360/slotAngle))
		slot--;
	//int direction = 1;

	//switch (currentQuadrant)
	//{
	//case A:
	//	while (towerSlots[slot])
	//	{
	//		if (slot == 12)
	//			direction = -1;
	//		else if (slot == 6)
	//			direction = 1;
	//		slot += direction;
	//	}
	//	break;
	//case B:
	//	while (towerSlots[slot])
	//	{
	//		
	//		if (slot == 6)
	//			direction = -1;
	//		else if (slot == 0)
	//			direction = 1;
	//		slot += direction;
	//	}
	//	break;
	//case C:
	//	while (towerSlots[slot])
	//	{
	//		if (slot == 23)
	//			direction = -1;
	//		else if (slot == 18)
	//			direction = 1;
	//		slot += direction;

	//	}
	//	break;
	//case D:
	//	while (towerSlots[slot])
	//	{
	//		if (slot == 18)
	//			direction = -1;
	//		else if (slot == 12)
	//			direction = 1;
	//		slot += direction;
	//	}
	//	break;
	//}

	angle = (((slot+1) * slotAngle)-(slotAngle/2));
	
	if (mTowers[currentQuadrant].size() < 4)
	{
		if (towerSlots[slot] == false)
		{
			int totalEssence = 0;
			int essenceRequired = 0;

			switch (type)
			{
			case NORMAL:
//#ifndef _DEBUG				
				totalEssence = currentScene->getEssenceAmount();
				essenceRequired = NormalTower::getBuildCost();

				if (essenceRequired <= totalEssence)
				{
					currentScene->useEssence(essenceRequired);
					towerSlots[slot] = true;
					currentScene->getCore()->SetSlotOccupied(slot);
				}
				else
				{
					currentScene->useEssence(essenceRequired);
					return Vec2(0,0);
				}
//#endif
				//currentScene->getCore()->SetSlotOccupied(slot);
				mTowers[currentQuadrant].push_back(NormalTower::create(Vec2(pos.x, pos.y), currentQuadrant, true));
				mTowers[currentQuadrant].back()->moveTo(Helpers::placeAtAngle(angle, ringRadius));
				break;

			case SHIELD:
//#ifndef _DEBUG				
				totalEssence = currentScene->getEssenceAmount();
				essenceRequired = ShieldTower::getBuildCost();
				if (essenceRequired <= totalEssence)
				{
					currentScene->useEssence(essenceRequired);
					towerSlots[slot] = true;
					currentScene->getCore()->SetSlotOccupied(slot);
				}
				else
				{
					currentScene->useEssence(essenceRequired);
					return Vec2(0,0);
				}
//#endif

				//currentScene->getCore()->SetSlotOccupied(slot);
				mTowers[currentQuadrant].push_back(ShieldTower::create(Vec2(pos.x, pos.y), currentQuadrant, true));
				mTowers[currentQuadrant].back()->moveTo(Helpers::placeAtAngle(angle, ringRadius));			
				break;

			default: break;
			}

			addChild(mTowers[currentQuadrant].back());
			return (Helpers::placeAtAngle(angle, ringRadius));
		}
		return Vec2(0, 0);
	}
	return Vec2(999, 999);
}

vector<Tower*> TowerManager::getTowers(int quadrant)
{
	return mTowers[quadrant];
}

void TowerManager::removeTower(Tower* tower, int quadrant)
{
	static GameScene* currentScene = dynamic_cast<GameScene*>(this->getScene());
	
	for (auto itr = mTowers[quadrant].begin();  itr != mTowers[quadrant].end(); itr++)
	{
		if (*itr == tower)
		{
			mTowers[quadrant].erase(itr);

			//currentScene->getCore()->SetSlotFree(i);

			auto pos = tower->getPosition();
			//auto ringRadius = ScaleConstants::RING_RADIUS;
			//float intersectionRadius = pos.getDistance(Vec2(0, 0));
			//Vec2 reference = Vec2(ringRadius, 0);
			//double angle = acos(reference.dot(pos) / (intersectionRadius*ringRadius));
			double angle = pos.getAngle();
			angle = CC_RADIANS_TO_DEGREES(angle);

			if (angle < 0)
				angle = 360 + angle;

			int slot = angle / ScaleConstants::SLOT_ANGLE;

			if (slot == (360 / ScaleConstants::SLOT_ANGLE))
				slot--;

			towerSlots[slot] = false;

			TowerCategory category = TowerCategory(tower->getCategory());
			auto level = tower->getLevel();
			auto a = CallFunc::create(CC_CALLBACK_0(TowerManager::spawnDebrisAt, this, pos, category, level));
			this->runAction(a);
			return;
		}
	}
}

void TowerManager::activateTowers()
{
	for (int i = 0; i < Quadrants::QuadrantCount; i++)
	{
		for (auto itr = mTowers[i].begin(); itr != mTowers[i].end(); itr++)
		{
			(*itr)->exitCooldown();
		}
	}
}

void TowerManager::deactivateTowers()
{
	for (int i = 0; i < Quadrants::QuadrantCount; i++)
	{
		for (auto itr = mTowers[i].begin(); itr != mTowers[i].end(); itr++)
		{
			(*itr)->enterCooldown();
		}
	}
}

void TowerManager::rotateTowers()
{
	for (int i = 0; i < Quadrants::QuadrantCount; i++)
	{
		for (auto itr = mTowers[i].begin(); itr != mTowers[i].end(); itr++)
		{
			(*itr)->rotate();
		}
	}
}

void TowerManager::straightenTowers()
{
	for (int i = 0; i < Quadrants::QuadrantCount; i++)
	{
		for (auto itr = mTowers[i].begin(); itr != mTowers[i].end(); itr++)
		{
			(*itr)->straighten();
		}
	}
}

bool TowerManager::containsPointOnTower(Vec2 point)
{
	for (int i = 0; i < Quadrants::QuadrantCount; i++)
	{
		for (auto itr = mTowers[i].begin(); itr != mTowers[i].end(); itr++)
		{
			if ((*itr)->containsPoint(point))
				return true;
		}
	}

	return false;
}

void TowerManager::indicateTowerUpgradability()
{
	for (int i = 0; i < Quadrants::QuadrantCount; i++)
	{
		for (auto itr = mTowers[i].begin(); itr != mTowers[i].end(); itr++)
		{
			(*itr)->indicateUpgradeMode();
		}
	}
}

void TowerManager::exitUpgradeMode()
{
	for (int i = 0; i < Quadrants::QuadrantCount; i++)
	{
		for (auto itr = mTowers[i].begin(); itr != mTowers[i].end(); itr++)
		{
			(*itr)->exitUpgradeMode();
		}
	}
}

bool TowerManager::isQuadrantAlive(int quadrant)
{
	return (mTowers[quadrant].size() > 0);
}

bool TowerManager::isQuadrantFull(int quadrant)
{
	return (mTowers[quadrant].size() == 4);
}

void TowerManager::spawnDebrisAt(Vec2 pos, TowerCategory type, int level)
{
	switch (type)
	{
	case Offense:
		mDebris.push_back(Debris::create(pos, eTowerType::NORMAL, level));
		break;

	case Defense:
		mDebris.push_back(Debris::create(pos, eTowerType::SHIELD, level));
		break;

	default: break;
	}

	addChild(mDebris.back(), -2);
}

bool TowerManager::isSlotAvailable(int slotIndex)
{
	return !towerSlots[slotIndex];
}
