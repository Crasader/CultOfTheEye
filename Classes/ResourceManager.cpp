#include "ResourceManager.h"
#include "MonsterManager.h"
#include <string>
#include <sstream>

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

#ifdef _DEBUG

#define INITIAL_RESOURCES 99

#else

#define INITIAL_RESOURCES 0

#endif // _DEBUG

GameUI::GameUI() : mAmount(INITIAL_RESOURCES)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	{
		mSprite = Sprite::create(FilePaths::UI::ResourceBar);
		mSprite->setScale(0.4);
		mSprite->setAnchorPoint(Vec2(1, 1));
		mSprite->setPosition(Vec2(winSize.width - 5, winSize.height - 5));
		addChild(mSprite);

		auto size = mSprite->getBoundingBox();
		mResourceMeter = Label::createWithTTF("0", FilePaths::Fonts::StandardFont, 75);
		mResourceMeter->setAnchorPoint(Vec2(0.5, 0.5));
		mResourceMeter->setTextColor(Color4B().WHITE);
		mResourceMeter->setPosition(Vec2(size.size.width / 2 + 188, size.size.height / 2 + 57));
		mSprite->addChild(mResourceMeter);
		scheduleUpdate();
	}

	mMonsterHealth = MonsterHealthUI::create();
	mMonsterHealth->setPosition(Vec2(winSize.width / 2.f, winSize.height - 5));
	addChild(mMonsterHealth);
}

int GameUI::getAmount()
{
	return mAmount;
}

void GameUI::update(float time)
{
	std::string text = to_string(mAmount);
	mResourceMeter->setString(text);
}

void GameUI::increase(int amount)
{
	mAmount = min(99, amount + mAmount);
}

bool GameUI::decrease(int amount)
{
	if (mAmount >= amount)
	{
		mAmount -= amount;
		auto flashBlue = TintTo::create(0.45f, 5, 200, 200);
		auto setNormalColor = TintTo::create(0.45f, 255, 255, 255);
		auto sequence = Sequence::create(flashBlue, setNormalColor, nullptr);
		mSprite->runAction(sequence);
		return true;
	}
	
	auto flashRed = TintTo::create(0.2f, 255, 0, 0);
	auto setNormalColor = TintTo::create(0.2f, 255, 255, 255);
	auto sequence = Repeat::create(Sequence::create(flashRed, setNormalColor, nullptr), 2);
	mSprite->runAction(sequence);
	
	return false;
}

void GameUI::showTowerUI()
{
	//mTowerInfoUI = Node::create();
}

void GameUI::resetTemporaryDamage()
{
	mMonsterHealth->reset(MonsterHealthUI::Overlay::White);
}

void GameUI::indicateTemporaryDamage(float currentHealthPercent)
{
	mMonsterHealth->expand(MonsterHealthUI::Overlay::White, currentHealthPercent / 100.f);
}

void GameUI::indicatePermanentDamage(float currentHealthPercent)
{
	mMonsterHealth->expand(MonsterHealthUI::Overlay::Black, currentHealthPercent / 100.f);
	mMonsterHealth->reset(MonsterHealthUI::Overlay::White);
	mMonsterHealth->advance(MonsterHealthUI::Overlay::White);
}
