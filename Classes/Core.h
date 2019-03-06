#pragma once
#include "cocos2d.h"
#include "Types.h"

#include <array>
#include <bitset>

using namespace std;
using namespace cocos2d;
using namespace ui;

class UIButton;

class Core : public Node
{
public:
	Core();
	~Core();

	bool containsPointOnPlanet(Vec2 point);
	bool containsPointOnRing(Vec2 point);
	bool containsPointWithinRing(Vec2 point);
	bool containsPointOnButton(Vec2 point);

	void SetSlotOccupied(int slotIndex);
	void SetSlotFree(int slotIndex);

	void update(float dt);

	//Getter for UI Active State
	bool isUIActive()
	{
		return mIsUIActive;
	}

	CREATE_FUNC(Core);

private:
	////NOTE THAT THE LAST NODE IS A DUMMY NODE, TO ENABLE A PARENT CHILD RELATIONSHIP FOR PHYSICS
	array<Sprite*,4>		mCoreSprites;
	//array<PhysicsBody*,5>	mPhysicsBody;
	array<Sprite*, 4>		mRingSprites;
	Sprite*					mFogSprite;
	array<UIButton*, 8>		mUIButtons;
	
	bitset<4>			isQuadrantAlive;
	bitset<4>			isQuadrantFull;

	//array<CCParticleSystemQuad*, (360/ScaleConstants::SLOT_ANGLE)> mSlotIndicators;
	
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	void initUI();
	void openUI();
	void closeUI();

	bool uiOpened(Vec2 initialLocation, Vec2 finalLocation);

	bool uiClosed(Vec2 initialLocation, Vec2 finalLocation);

	bool mUIInit;

	bool mIsUIActive;

	bool mIsCreatingTower;

	void CreateCoreSprites();
	void CreateRingSprites();
	//void CreateDeadRingSprites(int quadrant);
	void CreateFogSprites();
	void CreateButtons();
	void CreateSlotIndicators();

	uint8_t touchCounter;

	//Label*	mMenuMessage;
};

