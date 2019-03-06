#include "TitleScene.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"
#include "Types.h"
#include "ControlsScene.h"
#include "CreditsScene.h"

bool TitleScene::init()
{
	setAnchorPoint(Vec2(0, 0));
	setPosition(0, 0);

	auto background = Sprite::create(FilePaths::Background::Title);
	background->setAnchorPoint(Vec2(0.5, 0.5));
	background->setScale(0.5, 0.5);
	background->setPosition(960, 540);
	addChild(background);

	mButtonPlay = Button::create(FilePaths::UI::Buttons::PlayNormal, FilePaths::UI::Buttons::PlaySelected);
	mButtonPlay->setAnchorPoint(Vec2(0.5, 0.5));
	mButtonPlay->setScale(0.5, 0.5);
	mButtonPlay->setPosition(Vec2(990, 140));
	mButtonPlay->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) 
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->pushScene(GameScene::create());
			break;
		}
	});
	addChild(mButtonPlay);

	mButtonControls = Button::create(FilePaths::UI::Buttons::ControlsNormal, FilePaths::UI::Buttons::ControlsSelected);
	mButtonControls->setAnchorPoint(Vec2(0.5, 0.5));
	mButtonControls->setScale(0.5, 0.5);
	mButtonControls->setPosition(Vec2(400, 150));
	mButtonControls->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->pushScene(ControlsScene::create());
			break;
		}
	});
	addChild(mButtonControls);

	mButtonCredits = Button::create(FilePaths::UI::Buttons::CreditsNormal, FilePaths::UI::Buttons::CreditsSelected);
	mButtonCredits->setAnchorPoint(Vec2(0.5, 0.5));
	mButtonCredits->setScale(0.5, 0.5);
	mButtonCredits->setPosition(Vec2(1500, 150));
	mButtonCredits->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->pushScene(CreditsScene::create());
			break;
		}
	});
	addChild(mButtonCredits);

	return true;
}
