#include "CreditsScene.h"
#include "Types.h"
#include "TitleScene.h"

using namespace FilePaths;

bool CreditsScene::init()
{
	setAnchorPoint(Vec2(0, 0));
	setPosition(0, 0);

	auto background = Sprite::create(FilePaths::Background::Credits);
	background->setAnchorPoint(Vec2(0.5, 0.5));
	background->setScale(0.5, 0.5);
	background->setPosition(950, 550);
	addChild(background);

	/*mProducer = Label::create("Producer: ", CreditsLabels, 40);
	mProducer->setAnchorPoint(Vec2(0.5, 0.5));
	mProducer->setTextColor(Color4B().WHITE);
	mProducer->setPosition(Vec2(800, 750));
	addChild(mProducer);

	mPetra = Label::create("Petra", CreditsLabels, 40);
	mPetra->setAnchorPoint(Vec2(0.5, 0.5));
	mPetra->setTextColor(Color4B().ORANGE);
	mPetra->setPosition(Vec2(1200, 750));
	addChild(mPetra);

	mEngineer1 = Label::create("Engineer: ", CreditsLabels, 40);
	mEngineer1->setAnchorPoint(Vec2(0.5, 0.5));
	mEngineer1->setTextColor(Color4B().WHITE);
	mEngineer1->setPosition(Vec2(400, 650));
	addChild(mEngineer1);

	mIan = Label::create("Ekshit", CreditsLabels, 40);
	mIan->setAnchorPoint(Vec2(0.5, 0.5));
	mIan->setTextColor(Color4B().ORANGE);
	mIan->setPosition(Vec2(800, 650));
	addChild(mIan);

	mEngineer2 = Label::create("Engineer: ", CreditsLabels, 40);
	mEngineer2->setAnchorPoint(Vec2(0.5, 0.5));
	mEngineer2->setTextColor(Color4B().WHITE);
	mEngineer2->setPosition(Vec2(400, 550));
	addChild(mEngineer2);

	mVarun = Label::create("Varun", CreditsLabels, 40);
	mVarun->setAnchorPoint(Vec2(0.5, 0.5));
	mVarun->setTextColor(Color4B().ORANGE);
	mVarun->setPosition(Vec2(800, 550));
	addChild(mVarun);

	mEngineer3 = Label::create("Engineer: ", CreditsLabels, 40);
	mEngineer3->setAnchorPoint(Vec2(0.5, 0.5));
	mEngineer3->setTextColor(Color4B().WHITE);
	mEngineer3->setPosition(Vec2(400, 450));
	addChild(mEngineer3);

	mKhadeidre = Label::create("Khadeidre", CreditsLabels, 40);
	mKhadeidre->setAnchorPoint(Vec2(0.5, 0.5));
	mKhadeidre->setTextColor(Color4B().ORANGE);
	mKhadeidre->setPosition(Vec2(800, 450));
	addChild(mKhadeidre);

	mArtist1 = Label::create("Artist: ", CreditsLabels, 40);
	mArtist1->setAnchorPoint(Vec2(0.5, 0.5));
	mArtist1->setTextColor(Color4B().WHITE);
	mArtist1->setPosition(Vec2(1200, 650));
	addChild(mArtist1);

	mSarah = Label::create("Sarah", CreditsLabels, 40);
	mSarah->setAnchorPoint(Vec2(0.5, 0.5));
	mSarah->setTextColor(Color4B().ORANGE);
	mSarah->setPosition(Vec2(1600, 650));
	addChild(mSarah);

	mArtist2 = Label::create("Artist: ", CreditsLabels, 40);
	mArtist2->setAnchorPoint(Vec2(0.5, 0.5));
	mArtist2->setTextColor(Color4B().WHITE);
	mArtist2->setPosition(Vec2(1200, 550));
	addChild(mArtist2);

	mEmi = Label::create("Emi", CreditsLabels, 40);
	mEmi->setAnchorPoint(Vec2(0.5, 0.5));
	mEmi->setTextColor(Color4B().ORANGE);
	mEmi->setPosition(Vec2(1600, 550));
	addChild(mEmi);

	mArtist3 = Label::create("Artist: ", CreditsLabels, 40);
	mArtist3->setAnchorPoint(Vec2(0.5, 0.5));
	mArtist3->setTextColor(Color4B().WHITE);
	mArtist3->setPosition(Vec2(400, 350));
	addChild(mArtist3);

	mChris = Label::create("Chris", CreditsLabels, 40);
	mChris->setAnchorPoint(Vec2(0.5, 0.5));
	mChris->setTextColor(Color4B().ORANGE);
	mChris->setPosition(Vec2(800, 350));
	addChild(mChris);

	mArtist4 = Label::create("Artist: ", CreditsLabels, 40);
	mArtist4->setAnchorPoint(Vec2(0.5, 0.5));
	mArtist4->setTextColor(Color4B().WHITE);
	mArtist4->setPosition(Vec2(1200, 450));
	addChild(mArtist4);

	mMiguel = Label::create("Miguel", CreditsLabels, 40);
	mMiguel->setAnchorPoint(Vec2(0.5, 0.5));
	mMiguel->setTextColor(Color4B().ORANGE);
	mMiguel->setPosition(Vec2(1600, 450));
	addChild(mMiguel);

	mArtist5 = Label::create("Artist: ", CreditsLabels, 40);
	mArtist5->setAnchorPoint(Vec2(0.5, 0.5));
	mArtist5->setTextColor(Color4B().WHITE);
	mArtist5->setPosition(Vec2(1200, 350));
	addChild(mArtist5);

	mXiaotong = Label::create("Xiaotong", CreditsLabels, 40);
	mXiaotong->setAnchorPoint(Vec2(0.5, 0.5));
	mXiaotong->setTextColor(Color4B().ORANGE);
	mXiaotong->setPosition(Vec2(1600, 350));
	addChild(mXiaotong);*/


	mButtonBack = Button::create(FilePaths::UI::Buttons::BackNormal, FilePaths::UI::Buttons::BackSelected);
	mButtonBack->setAnchorPoint(Vec2(0.5, 0.5));
	mButtonBack->setScale(0.5, 0.5);
	mButtonBack->setPosition(Vec2(300, 160));
	mButtonBack->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->pushScene(TitleScene::create());
			break;
		}
	});
	addChild(mButtonBack);

	return true;
}