#include "Projectile.h"
#include <string>
#include "Tower.h"
#include "Types.h"
#include "GameScene.h"

void Projectile::superInit()
{
	setName(ClassStrings::Projectile);
	mHasGoneStatic = false;

	mSprite = Sprite::create(mSpriteFilename);
	mSprite->setAnchorPoint(Vec2(0.5, 0.5));
	mSprite->setPosition(mBallisticInfo.mPosition);
	mSprite->setScale(1.5);
	addChild(mSprite);

	mPhysicsBody = PhysicsBody::createBox(mSprite->getBoundingBox().size, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	mPhysicsBody->setDynamic(true);
	mPhysicsBody->applyImpulse(mImpulse * mBallisticInfo.mDirection * mBallisticInfo.mForce * 5);
	mPhysicsBody->setLinearDamping(0.4f);
	mPhysicsBody->setCollisionBitmask(0x00000000);
	mPhysicsBody->setRotationEnable(false);
	//mPhysicsBody->setVelocityLimit(500);
	mSprite->addComponent(mPhysicsBody);

	mEmitter = CCParticleSystemQuad::create("Images/projectileEffect.plist");

	// set the duration
	mEmitter->setDuration(ParticleSystem::DURATION_INFINITY);
	mEmitter->setPosition(mSprite->getPosition());

	addChild(mEmitter, 10);

	scheduleUpdate();

	schedule([this](float a)
	{
		mPhysicsBody->setCollisionBitmask(0xFFFFFFFF);
		mPhysicsBody->setContactTestBitmask(0xFFFFFFFF);
	}, 0, 0, 1, "EnableTowerCollision");

	schedule([this](float a)
	{
		if (mCoreReference->containsPointWithinRing(mSprite->getPosition()))
		{
			mPhysicsBody->setVelocityLimit(500);
			disintegrate();
		}
	}, 0, CC_REPEAT_FOREVER, 1, "EnableRingCollisions");
}

Projectile::Projectile(const int impulse, const int gravity) : mImpulse(impulse), mGravity(gravity)
{}

void Projectile::update(float dt)
{
	mCoreReference = (dynamic_cast<GameScene*>(this->getScene()))->getCore();
	
	mPhysicsBody->applyForce(mSprite->getPosition() * mGravity * 25);

	if (mPhysicsBody->getVelocity().length() < 15 && !mHasGoneStatic)
	{
		mHasGoneStatic = true;
		schedule([this](float a)
		{
			if (mPhysicsBody->getVelocity().length() < 15)
				removeFromParentAndCleanup(true);
			else
				mHasGoneStatic = false;
		}, 3, 0, 0, "RemoveStatic");		
	}

	mEmitter->setPosition(mSprite->getPosition());
}

int Projectile::getDamage()
{
	return mBallisticInfo.mDamage;
}

Tower* Projectile::getTower()
{
	return (Tower*)(mBallisticInfo.mTower);
}

void Projectile::setImpulse(int impulse)
{
	mImpulse = impulse;
}

void Projectile::handleCollision(Node* node)
{}

NormalProjectile::NormalProjectile(BallisticInfo info) 
	: Projectile(ProjectileConstants::NormalImpulse, ProjectileConstants::NormalGravity)
{
	mSpriteFilename = FilePaths::AttackProjectile;
	mBallisticInfo = info;
	superInit();
}

ShieldProjectile::ShieldProjectile(BallisticInfo info) 
	: Projectile(ProjectileConstants::ShieldImpulse, ProjectileConstants::ShieldGravity)
{
	mSpriteFilename = FilePaths::ShieldProjectile;
	mBallisticInfo = info;
	superInit();
}

void ShieldProjectile::handleCollision(Node* node)
{
	if (node->getName() == ClassStrings::Tower)
	{
		//Shield The Tower
		if (mBallisticInfo.mTower != node)
		{
			removeFromParentAndCleanup(true);
			((Tower*)node)->shield();
		}			
	}
}

void Projectile::disintegrate()
{
	unschedule("EnableRingCollisions");

	auto shrinkTo = ScaleTo::create(1, 0);
	mSprite->runAction(shrinkTo);

	schedule([this](float a)
	{
		removeFromParentAndCleanup(true);
	}, 0, 0, 1, "Disintegrate");
}
