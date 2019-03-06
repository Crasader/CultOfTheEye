#include "CollisionHandler.h"
#include "Tower.h"
#include "Projectile.h"
#include "Gem.h"
#include "Core.h"
#include "Monster.h"
#include "Types.h"
#include "Helpers.h"
#include <utility>

CollisionHandler::CollisionHandler(Node* node)
{
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(CollisionHandler::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, node);
}

bool CollisionHandler::matchCategory(pair<PhysicsShape*, PhysicsShape*>& p, string type1, string type2)
{
	string category1 = p.first->getBody()->getNode()->getParent()->getName();
	string category2 = p.second->getBody()->getNode()->getParent()->getName();
	if (type1 == category1 && type2 == category2)
		return true;
	else if (type1 == category2 && type2 == category1)
	{
		swap(p.first, p.second);
		return true;
	}
	else return false;
}

bool CollisionHandler::onContactBegin(PhysicsContact& contact)
{
	pair<PhysicsShape*, PhysicsShape*> contactPair = make_pair(contact.getShapeA(), contact.getShapeB());

	auto shapeA = contact.getShapeA();
	auto bodyA = shapeA->getBody();
	auto parentA = bodyA->getNode();
	
	if (parentA == nullptr)
		return true;
	auto nodeA = parentA->getParent();

	auto shapeB = contact.getShapeB();
	auto bodyB = shapeB->getBody();
	auto parentB = bodyB->getNode();

	if (parentB == nullptr)
		return true;
	auto nodeB = parentB->getParent();

	if (nodeA && nodeB)
	{
		if (matchCategory(contactPair, ClassStrings::Projectile, ClassStrings::Core))
		{
			auto projectile = (Projectile*)contactPair.first->getBody()->getNode()->getParent();

			projectile->removeFromParentAndCleanup(true);

			return true;
		}		
		else if (matchCategory(contactPair, ClassStrings::Projectile, ClassStrings::Monster))
		{
			auto projectile = (Projectile*)contactPair.first->getBody()->getNode()->getParent();
			auto monster = (Monster*)contactPair.second->getBody()->getNode()->getParent();

			monster->handleProjectileCollision(projectile);
			projectile->removeFromParentAndCleanup(true);

			return true;
		}
		else if (matchCategory(contactPair, ClassStrings::Tower, ClassStrings::Monster))
		{
			auto tower = (Tower*)contactPair.first->getBody()->getNode()->getParent();

			tower->damage(1);

			return true;
		}
		else if (matchCategory(contactPair, ClassStrings::Drop, ClassStrings::Core))
		{
			auto essence = (Gem*)contactPair.first->getBody()->getNode()->getParent();
			//TODO

			return true;
		}	
		else if (matchCategory(contactPair, ClassStrings::Projectile, ClassStrings::Tower))
		{
			auto projectile =	(Projectile*)contactPair.first->getBody()->getNode()->getParent();
			auto tower		=	(Tower*)contactPair.second->getBody()->getNode()->getParent();

			projectile->handleCollision(tower);
			return true;
		}
	}
	return true;
}

