#include "EndScene.h"
#include "TitleScene.h"
#include "Types.h"

bool EndScene::init()
{
	setAnchorPoint(Vec2(0, 0));
	setPosition(0, 0);

	auto background = Sprite::create(FilePaths::Background::End);
	background->setAnchorPoint(Vec2(0.5, 0.5));
	background->setPosition(1920 / 2, 1080 / 2);
	addChild(background);

	Sprite* condition;
	if (mOutcome == GameScene::EndGameOutcome::Victory)
	{
		condition = Sprite::create(FilePaths::UI::Victory);
	}
	else
	{
		condition = Sprite::create(FilePaths::UI::Defeat);
	}
	
	condition->setAnchorPoint(Vec2(0.5, 0.5));
	condition->setPosition(1920 / 2, 700);
	condition->setScale(0.5);
	addChild(condition);

	mButtonPlay = Button::create(FilePaths::UI::Buttons::MenuNormal, FilePaths::UI::Buttons::MenuSelected);
	mButtonPlay->setAnchorPoint(Vec2(0.5, 0.5));
	mButtonPlay->setScale(0.5, 0.5);
	mButtonPlay->setPosition(Vec2(1920/2, 150));
	mButtonPlay->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) 
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->pushScene(TitleScene::create());
			break;
		}
	});
	addChild(mButtonPlay);
	
	return true;
}

EndScene::EndScene(GameScene::EndGameOutcome outcome)
{
	mOutcome = outcome;
}