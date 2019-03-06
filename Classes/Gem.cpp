#include "Gem.h"
#include "GameScene.h"
#include "Types.h"
#include "Helpers.h"
#include "RNG.h"

Gem::Gem(Vec2 position) 
{
	setName(ClassStrings::Drop);
	
	/*mPhysicsBody = PhysicsBody::createBox(mSprite->getBoundingBox().size, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	mPhysicsBody->setDynamic(true);
	mPhysicsBody->setLinearDamping(0.15f);
	mPhysicsBody->setContactTestBitmask(0xF1F1FFFF);
	mPhysicsBody->setRotationEnable(false);
	mSprite->addComponent(mPhysicsBody);*/

	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= 8; i++)
	{
		sprintf(str, Animations::Essence::fIdle.c_str(), i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}

	Animation *anim = Animation::createWithSpriteFrames(animFrames, 1 / 16.f);

	auto jiggle1 = RotateTo::create(0.25, 15);
	auto jiggle2 = RotateTo::create(0.25, -15);
	auto jiggleSequence = Sequence::create(jiggle1, jiggle2, nullptr);

	auto moveTo = MoveTo::create(30, Helpers::placeAtAngle(RNG::getUniformInteger(0, 360), ScaleConstants::RING_RADIUS));

	mSprite = Sprite::createWithSpriteFrame(animFrames.front());

	mSprite->setAnchorPoint(Vec2(0.5, 0.5));
	mSprite->setPosition(position);
	mSprite->setScale(0.5);

	mSprite->runAction(RepeatForever::create(Animate::create(anim)));
	mSprite->runAction(RepeatForever::create(jiggleSequence));
	mSprite->runAction(RepeatForever::create(moveTo));
	addChild(mSprite);

	scheduleUpdate();

	schedule([this](float a)
	{
		mSprite->setVisible(false);
		markedForDeletion = true;
	}, 0, 0, 15, "Delete");
}

void Gem::update(float time)
{
    if(!markedForDeletion)
        auto a = mSprite->getPosition();
}

void Gem::handleTouch(Touch * touch)
{}

Rect Gem::getBoundingBox()
{
    if(!markedForDeletion)
        return mSprite->getBoundingBox();
    
    else
        return Rect();
}

Gem::~Gem()
{}

void Gem::expire()
{
	//removeFromParentAndCleanup(true);
}

void Gem::collect()
{
	auto moveTo = MoveTo::create(0.5, Vec2(0, 0));
	mSprite->runAction(moveTo);

	schedule([this](float a)
	{
		removeFromParentAndCleanup(true);
	}, 0, 0, 0.5, "Destroy");
}
