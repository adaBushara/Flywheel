#include "collider.h"
#include "Entity.h"
#include "RigidBody.h"
#include "Engine.h"

void Collider::BeginContact(b2Contact * contact)
{
	//fixtureA
	Entity *e = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	auto fixtureA = contact->GetFixtureA();
	//fixtureB
	Entity *e1 = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());
	auto fixtureB = contact->GetFixtureB();

	PhysicsSystem *phys = Engine::getInstance()->getPhysicsSystem();
	//std::cout << "Collision!" <<gameObject->getID() << std::endl;
	//std::cout << e1->getID() << std::endl;
	
		if (e->hasComponent<RigidBody>())
		{
			if (fixtureA->IsSensor() || fixtureB->IsSensor())
			{
				//std::cout << "Trigger Enter" << std::endl;
				phys->registerTriggerEnter(e, e1);
			}
			else
			{
				//signal fixture B as other
				phys->registerCollisionEnter(e, e1);
			}

		}
		if (e1->hasComponent<RigidBody>())
		{
			if (fixtureA->IsSensor() || fixtureB->IsSensor())
			{
				//std::cout << "Trigger Enter" << std::endl;
				phys->registerTriggerEnter(e1, e);
			}
			else
			{
				//fixture A as other
				phys->registerCollisionEnter(e1, e);
			}
		}
	
}

void Collider::EndContact(b2Contact * contact)
{

	
	//fixtureA
	Entity *e = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	auto fixtureA = contact->GetFixtureA();
	//void *t = contact->GetFixtureA()->GetUserData();
	//fixtureB
	Entity *e1 = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());
	auto fixtureB = contact->GetFixtureB();

	PhysicsSystem *phys = Engine::getInstance()->getPhysicsSystem();

	if (e->hasComponent<RigidBody>())
	{
		//signal fixture B as other
		if (fixtureA->IsSensor() || fixtureB->IsSensor())
		{
			//std::cout << "Trigger Exit" << std::endl;
			phys->registerTriggerExit(e, e1);
		}
		else
		{
			phys->registerCollisionExit(e, e1);
			//std::cout << "Collision Exit" << std::endl;
		}

	}
	if (e1->hasComponent<RigidBody>())
	{
		//fixture A as other
		if (fixtureA->IsSensor() || fixtureB->IsSensor())
		{
			//std::cout << "Trigger Exit" << std::endl;
			phys->registerTriggerExit(e1, e);
		}
		else
		{
			phys->registerCollisionExit(e1, e);
		}
	}
	
}
