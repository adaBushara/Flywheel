#include "PhysicsSystem.h"
#include "MathUtils.h"
#include "Utils.h"
#include "Rotator.h"

PhysicsSystem::PhysicsSystem()
{
	//create a 2D world
	_gravity = b2Vec2(0, -9.81f);
	_world = new b2World(_gravity);
	componentMask = Position::getMask();// | RigidBody::getMask();
	_lastWorldStepTime = system_clock::now();
	timeStep = 1.0f / 60.0f;
	velocityIterations = 8;
	positionIterations = 4;
}

PhysicsSystem::~PhysicsSystem()
{
	delete _world;
}

void PhysicsSystem::setGravity(const b2Vec2 &vec)
{
	_world->SetGravity(vec);
	_gravity = vec;
}

b2Vec2 PhysicsSystem::getGravity()
{
	return _world->GetGravity();
}

//helper function, same as Entity::HasComponent
template <class C>
bool hasComponent(Entity& entity)
{
	return (entity.getComponentMask() & C::getMask()) == C::getMask();
}

void PhysicsSystem::Update()
{
	float physicsScale = Engine::getInstance()->physicsScale;
	for (Entity &e : Entity::getEntityList())
	{
		this->syncTransform(e);
	}
	//make sure we only update once every 60 frames
	duration<float> delta = system_clock::now() - _lastWorldStepTime;
	if (delta.count() > timeStep)
	{
		_lastWorldStepTime = system_clock::now();
		_world->Step(timeStep, velocityIterations, positionIterations);
	}

	for (Entity &e : Entity::getEntityList())
	{
		//compare the component maask to know if we should be doing physics operations on this entity
		if ((e.getComponentMask() & componentMask) == componentMask)
		{
			Position &pos = e.getComponent<Position>();
			
			createColliders(e);
			/*
			//Collider handling
			if (hasComponent<CircleCollider>(e))
			{
				if (_bodies.count(e.getID()) < 1)
				{
					b2Body *body = createBody(e.getID(), pos, physicsScale);
					body->SetType(b2BodyType::b2_staticBody);
				}
				b2Body * body = _bodies[e.getID()];
				if (body->GetFixtureList() == nullptr)
				{
					b2CircleShape shape = b2CircleShape();
					CircleCollider &col = e.getComponent<CircleCollider>();
					shape.m_radius = col.radius / physicsScale;
					body->CreateFixture(&shape, 1.0f);
				}
			}
			else if (hasComponent<RectangleCollider>(e))
			{
				if (_bodies.count(e.getID()) < 1)
				{
					b2Body *body = createBody(e.getID(), pos, physicsScale);
					body->SetType(b2BodyType::b2_staticBody);
				}
				b2Body * body = _bodies[e.getID()];
				if (body->GetFixtureList() == nullptr)
				{
					b2PolygonShape shape;
					RectangleCollider& rect = e.getComponent<RectangleCollider>();
					shape.SetAsBox(rect.width / 2.0f / physicsScale, rect.height / 2.0f / physicsScale);
					body->CreateFixture(&shape, 0.0f);
				}
			}

			*/
			syncRigidBody(e);
		/*	//Rigid Body handling
			if ((e.getComponentMask() & RigidBody::getMask()) == RigidBody::getMask())
			{
				if (_bodies.count(e.getID()) < 1)
				{
					b2Body *b = createBody(e.getID(), pos, physicsScale);
				}

				b2Body *body = _bodies[e.getID()];
				if (body->GetType() != b2BodyType::b2_dynamicBody)
				{
					body->SetType(b2BodyType::b2_dynamicBody);
				}
				b2Vec2 bodyPos = _bodies[e.getID()]->GetPosition();
				pos.x = bodyPos.x * physicsScale;
				pos.y = bodyPos.y * physicsScale;
			}*/
		}
	}
}

void PhysicsSystem::registerCollisionEnter(Entity * a, Entity * b)
{
	_collisionEnter.push_back(std::make_tuple(a, b));
}

void PhysicsSystem::registerCollisionExit(Entity * a, Entity * b)
{
	_collisionExit.push_back(std::make_tuple(a, b));
}

void PhysicsSystem::registerTriggerEnter(Entity *a, Entity *b)
{
	_triggerEnter.push_back(std::make_tuple(a, b));;
}

void PhysicsSystem::registerTriggerExit(Entity *a, Entity *b)
{
	_triggerExit.push_back(std::make_tuple(a, b));
}

std::vector<std::tuple<Entity*, Entity*>>& PhysicsSystem::getCollisionEnter()
{
	return _collisionEnter;
}

std::vector<std::tuple<Entity*, Entity*>>& PhysicsSystem::getCollisionExit()
{
	return _collisionExit;
}

std::vector<std::tuple<Entity*, Entity*>>& PhysicsSystem::getTriggerEnter()
{
	return _triggerEnter;
}

std::vector<std::tuple<Entity*, Entity*>>& PhysicsSystem::getTriggerExit()
{
	return _triggerExit;
}

void PhysicsSystem::clearCollisions()
{
	_collisionEnter.clear();
	_collisionExit.clear();
	_triggerEnter.clear();
	_triggerExit.clear();
}

b2Body* PhysicsSystem::createBody(size_t entityID, Position& pos, float physicsScale)
{
	//the definition of our rigidbody
	b2BodyDef bodyDef;
	bodyDef.position.x = pos.x / physicsScale;
	bodyDef.position.y = pos.y / physicsScale;

	//add the body to the world
	b2Body* body = _world->CreateBody(&bodyDef);
	_bodies[entityID] = body;
	return body;
}

void PhysicsSystem::registerContactListener(Collider & col)
{
	b2ContactListener *b = static_cast<b2ContactListener*>(&col);
	_world->SetContactListener(b);
	
}

void PhysicsSystem::syncRigidBody(Entity &e)
{
	/*float physicsScale = Engine::getInstance()->physicsScale;
	if (hasComponent<Position>(e))
	{
		
		Position &pos = e.getComponent<Position>();
		if ((e.getComponentMask() & RigidBody::getMask()) == RigidBody::getMask())
		{
			if (_bodies.count(e.getID()) < 1)
			{
				b2Body *b = createBody(e.getID(), pos, physicsScale);
				std::cout << "Body Created" << std::endl;
			}

			b2Body *body = _bodies[e.getID()];
			if (body->GetType() != b2BodyType::b2_dynamicBody)
			{
				body->SetType(b2BodyType::b2_dynamicBody);
				std::cout << "Set to Dynamic" << std::endl;
			}
			b2Vec2 bodyPos = _bodies[e.getID()]->GetPosition();
			pos.x = bodyPos.x * physicsScale;
			pos.y = bodyPos.y * physicsScale;
		}

	}*/


	if (e.hasComponent<Position>() && e.hasComponent<RigidBody>() && e.hasComponent<Rotator>())
	{
		//having an RB guarantees a box2D pointer?
		//it should
		RigidBody &rb = e.getComponent<RigidBody>();
		Position &pos = e.getComponent<Position>();
		Rotator &rot = e.getComponent<Rotator>();
		if (rb.isDynamic() || rb.isKinematic())
		{
			b2Vec2 &loc = rb.GetPosition();
			pos.x = loc.x;
			pos.y = loc.y;
			rot.angle = rb.getAngle() * Math::Rad2Deg;
		}
		//Rotator component uses degrees
		//Box2D uses radians
		//have to convert when assigning values
	}

}


void PhysicsSystem::createColliders(Entity &e)
{
	
	Position &pos = e.getComponent<Position>();
	float physicsScale = Engine::getInstance()->physicsScale;

	if (hasComponent<CircleCollider>(e))
	{
		if (_bodies.count(e.getID()) < 1)
		{
			b2Body *body = createBody(e.getID(), pos, physicsScale);
			body->SetType(b2BodyType::b2_staticBody);
		}
		b2Body * body = _bodies[e.getID()];
		if (body->GetFixtureList() == nullptr)
		{
			b2CircleShape shape = b2CircleShape();
			CircleCollider &col = e.getComponent<CircleCollider>();
			shape.m_radius = col.radius / physicsScale;
			body->CreateFixture(&shape, 1.0f);//	 ->SetSensor(true);
			body->SetUserData(&e);
			this->registerContactListener(col);
			
		}
		if (e.hasComponent<RigidBody>())
		{
			//default is going to be dynamic
			//otherwise keep it static
			RigidBody &rb = e.getComponent<RigidBody>();
			rb.SetBody(body);
			rb.setDynamic();
		}
	}
	else if (hasComponent<RectangleCollider>(e))
	{
		if (_bodies.count(e.getID()) < 1)
		{
			b2Body *body = createBody(e.getID(), pos, physicsScale);
			body->SetType(b2BodyType::b2_staticBody);
		}
		b2Body * body = _bodies[e.getID()];
		if (body->GetFixtureList() == nullptr)
		{
			
			b2PolygonShape shape;
			RectangleCollider& rect = e.getComponent<RectangleCollider>();
			shape.SetAsBox(rect.width / 2.0f / physicsScale, rect.height / 2.0f / physicsScale);
			body->CreateFixture(&shape, 0.0f);
			body->SetUserData(&e);			
			registerContactListener(rect);
			
		}
		if (e.hasComponent<RigidBody>())
		{
			//default is going to be dynamic
			//otherwise keep it static
			RigidBody &rb = e.getComponent<RigidBody>();
			rb.SetBody(body);
			rb.setDynamic();
		}
	}
}

void PhysicsSystem::syncTransform(Entity & e)
{

	if (e.hasComponent<RigidBody>() && e.hasComponent<Position>() && e.hasComponent<Rotator>())
	{
		Position &p = e.getComponent<Position>();
		RigidBody &rb = e.getComponent<RigidBody>();
		Rotator &rot = e.getComponent<Rotator>();
		if (!Math::approximately(p, rb.GetPosition()))
		{
			b2Vec2 pos = b2Vec2(p.x, p.y);
			rb.setPosition(pos);
		}
		if (!Math::approximately(rot.angle * Math::Deg2Rad, rb.getAngle()) )
		{
			rb.SetAngle(rot.angle * Math::Deg2Rad);
		}
	}
}
