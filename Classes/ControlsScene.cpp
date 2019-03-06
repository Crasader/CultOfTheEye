#include "ControlsScene.h"
#include "Types.h"
#include "TitleScene.h"

bool ControlsScene::init()
{
		setAnchorPoint(Vec2(0, 0));
		setPosition(0, 0);

		auto background = Sprite::create(FilePaths::Background::Tutorial);
		background->setAnchorPoint(Vec2(0.5, 0.5));
		background->setScale(0.5, 0.5);
		background->setPosition(950, 550);
		addChild(background);

		auto controls = Sprite::create(FilePaths::UITutorialControls);
		controls->setAnchorPoint(Vec2(1, 1));
		controls->setScale(0.8, 0.8);
		controls->setPosition(1700, 1000);
		addChild(controls);


		mButtonMenu = Button::create(FilePaths::UI::Buttons::MenuNormal, FilePaths::UI::Buttons::MenuSelected);
		mButtonMenu->setAnchorPoint(Vec2(0.5, 0.5));
		mButtonMenu->setScale(0.5, 0.5);
		mButtonMenu->setPosition(Vec2(1550, 430));
		mButtonMenu->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED:
				Director::getInstance()->pushScene(TitleScene::create());
				break;
			}
		});
		addChild(mButtonMenu);

		mButtonBack = Button::create(FilePaths::UI::Buttons::BackNormal, FilePaths::UI::Buttons::BackSelected);
		mButtonBack->setAnchorPoint(Vec2(0.5, 0.5));
		mButtonBack->setScale(0.5,0.5);
		mButtonBack->setPosition(Vec2(950, 230));
		mButtonBack->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED:
				if (mCurrentPage != 1) {
					changeToPage(mCurrentPage - 1);
				}
				break;
			}
		});
		//addChild(mButtonBack);

		mButtonNext = Button::create(FilePaths::UI::Buttons::NextNormal, FilePaths::UI::Buttons::NextSelected);
		mButtonNext->setAnchorPoint(Vec2(0.5, 0.5));
		mButtonNext->setScale(0.5, 0.5);
		mButtonNext->setPosition(Vec2(1550, 400));
		mButtonNext->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED:
				if (mCurrentPage != 0) {
					changeToPage(mCurrentPage + 1);	
				}
				break;
			}
		});
		mButtonNext->setOpacity(0);
		//addChild(mButtonNext);

	return true;
}

void ControlsScene::changeToPage(int pageNumber)
{
	/*switch (pageNumber) {
		case(1):
			mButtonBack->setOpacity(0);

			auto controls = Sprite::create(Controls);
			controls->setAnchorPoint(Vec2(1, 1));
			controls->setScale(0.8, 0.8);
			controls->setPosition(1700, 1000);
			controls->setOpacity(0);
			addChild(controls);
			break;

		case(2):
			//mButtonBack->setOpacity(1);

			auto controls = Sprite::create(Controls);
			controls->setAnchorPoint(Vec2(1, 1));
			controls->setScale(0.8, 0.8);
			controls->setPosition(1700, 1000);
			controls->setOpacity(0);
			addChild(controls);
			break;

		case(3):
			//mButtonBack->setOpacity(1);

			/*auto controls = Sprite::create(Controls);
			controls->setAnchorPoint(Vec2(1, 1));
			controls->setScale(0.8, 0.8);
			controls->setPosition(1700, 1000);
			controls->setOpacity(0);
			addChild(controls);
			break;

		case(4):
			//mButtonBack->setOpacity(1);

			/*auto controls = Sprite::create(Controls);
			controls->setAnchorPoint(Vec2(1, 1));
			controls->setScale(0.8, 0.8);
			controls->setPosition(1700, 1000);
			controls->setOpacity(0);
			addChild(controls);
			break;

	}*/
}
