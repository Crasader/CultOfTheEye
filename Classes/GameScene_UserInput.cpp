#include "GameScene.h"
#include <functional>
#include "Types.h"
#include "TitleScene.h"
#include "PhysicsShapeCache.h"

void GameScene::initUserInput()
{
	//Setting Keyboard Input (Windows)
	{
		auto listener = EventListenerKeyboard::create();

		listener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event* event)
		{
			auto pos = this->getDefaultCamera()->getPosition();

			if (key == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
			{
				if (pos.y == 1080 / 2)
					dolly(Vec2(0, -1080));
			}
			if (key == EventKeyboard::KeyCode::KEY_UP_ARROW)
			{
				if (pos.y == -1080 / 2)
					dolly(Vec2(0, +1080));					
			}
			if (key == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			{
				if (pos.x == 1920 / 2)
					dolly(Vec2(-1920, 0));
			}
			if (key == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			{
				if (pos.x == -1920 / 2)
					dolly(Vec2(+1920, 0));
			}
		};

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}

	//Setting Mouse Input (Windows)
	{
		auto listener = EventListenerMouse::create();

		listener->onMouseScroll = [this](EventMouse* event)
		{
			if (!mUI.mIsZoomedOut)
			{
				if (event->getScrollY() > 0)
					zoomOut();
			}
		};

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}

	//Setting Touch Input (Android + Windows(Mouse))
	{
		auto listener = EventListenerTouchOneByOne::create();

		listener->onTouchBegan = [this](Touch* touch, Event* event)
		{
			//if (mIsZoomedOut)
			//	return true;

			if (mUI.mIsZoomedOut)
			{
				if ((touch->getStartLocation() - touch->getLocation()).length() < 10)
				{
					auto pos = touch->getLocation();
					int padding = 25;

					if (pos.x < 1920 / 2 - padding && pos.y > 1080 / 2 + padding)
						zoomIn(0);
					else if (pos.x > 1920 / 2 + padding && pos.y > 1080 / 2 + padding)
						zoomIn(1);
					else if (pos.x > 1920 / 2 + padding && pos.y < 1080 / 2 - padding)
						zoomIn(2);
					else if (pos.x < 1920 / 2 - padding && pos.y < 1080 / 2 - padding)
						zoomIn(3);
				}
			}

			#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
			if (mMultiTouchCounter)
				return true;
			#endif

			mIsPlayerNavigating = false;
			
			float x = touch->getLocation().x;
			float y = touch->getLocation().y;

			auto cam = this->getDefaultCamera()->getPosition();
			if (cam.x < 0)
				x -= 1920;
			if (cam.y < 0)
				y -= 1080;

			auto worldLocation = Vec2(x, y);
			
			//Check if Touch coincides with a Gem
			for (auto itr = mGems.begin(); itr != mGems.end(); itr++)
			{
				if ((*itr)->markedForDeletion == false)
				{
					if ((*itr)->getBoundingBox().containsPoint(worldLocation))
					{
						(*itr)->collect();
						mGems.erase(itr);
						mUI.mGameUI->increase(1);
						return true;
					}
				}
                else
                    (*itr)->expire();
			}

			//Check if Touch Coincides with Tower
			if (mTowerManager->containsPointOnTower(worldLocation))
				return true;

			if (!mCore->containsPointWithinRing(worldLocation))
				mIsPlayerNavigating = true;

			//NOTE that the remaining touch checks are handled by eventListeners of the corresponding classes
			return true;
		};

		listener->onTouchMoved = [this](Touch* touch, Event* event)
		{};

		listener->onTouchEnded = [this](Touch* touch, Event* event)
		{

			#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
			if (mMultiTouchCounter != 1)
				return true;
			#endif

			if (mUI.mIsZoomedOut)
				return true;
				
			else if (mIsPlayerNavigating)
			{
				float x1 = touch->getStartLocation().x;
				float y1 = touch->getStartLocation().y;

				float x2 = touch->getLocation().x;
				float y2 = touch->getLocation().y;

				auto cam = this->getDefaultCamera()->getPosition();

				if (cam.x < 0)
				{
					x1 -= 1920;
					x2 -= 1920;
				}
				if (cam.y < 0)
				{
					y1 -= 1080;
					y2 -= 1080;
				}

				auto initWorldLocation = Vec2(x1, y1);
				auto finalWorldLocation = Vec2(x2, y2);

				auto p1 = touch->getStartLocation();
				auto p2 = touch->getLocation();

				auto u = p2 - p1;

				auto v1 = Vec2(100, 0);
				auto v2 = Vec2(0, 100);

				auto cosThetaWithX = (u.x * v1.x + u.y * v1.y) / (u.length() * v1.length());
				auto cosThetaWithY = (u.x * v2.x + u.y * v2.y) / (u.length() * v2.length());

				auto thetaX = CC_RADIANS_TO_DEGREES(acos(cosThetaWithX));
				auto thetaY = CC_RADIANS_TO_DEGREES(acos(cosThetaWithY));

				int padding = 25;

				bool down = (abs(thetaY) < padding);
				bool up = (abs(thetaY - 180) < padding);
				bool left = (abs(thetaX) < padding);
				bool right = (abs(thetaX - 180) < padding);
				
				auto pos = this->getDefaultCamera()->getPosition();

				if (up && pos.y == -1080 / 2)
					dolly(Vec2(0, +1080));
				
				else if (down && pos.y == 1080 / 2)
					dolly(Vec2(0, -1080));
				
				else if (left && pos.x == 1920 / 2)
					dolly(Vec2(-1920, 0));
				
				else if (right && pos.x == -1920 / 2)
					dolly(Vec2(+1920, 0));
				
				mIsPlayerNavigating = false;
			}
			
			return true;
		};

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}

	//Setting MultiTouch Input (Windows)
	{
		auto listener = EventListenerTouchAllAtOnce::create();

		listener->onTouchesBegan = [&](const std::vector<Touch*>& touches, Event* evt)
		{
			mMultiTouchCounter++;
		};

		listener->onTouchesMoved = [&](const std::vector<Touch*>& touches, Event* evt)
		{
			if (touches.size() != 2)
				return;

			float angle1 = CC_RADIANS_TO_DEGREES((touches[0]->getStartLocation() - touches[0]->getLocation()).getAngle());
			float angle2 = CC_RADIANS_TO_DEGREES((touches[1]->getLocation() - touches[1]->getStartLocation()).getAngle());

			if (abs(angle1 - angle2) < 15)
			{
				if (!mUI.mIsZoomedOut)
					zoomOut();
			}
		};

		listener->onTouchesEnded = [this](const std::vector<Touch*>& touches, Event* event)
		{
            mMultiTouchCounter = 0;
		};

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
	
}

