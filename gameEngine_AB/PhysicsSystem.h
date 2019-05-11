#pragma once
#include "ISystem.h"
#include <Box2D/Box2D.h>
#include <unordered_map>
#include <chrono>

#include "Entity.h"
#include "Position.h"
#include "RigidBody.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"

#include "Engine.h"

using namespace std::chrono;



class PhysicsSystem : public System<PhysicsSystem>
{
public:
	float timeStep;
	int velocityIterations;
	int positionIterations;

	PhysicsSystem(void);
	~PhysicsSystem(void);

	void Update();

	void registerCollisionEnter(Entity *a, Entity *b);

	void registerCollisionExit(Entity *a, Entity *b);


	void registerTriggerEnter(Entity *a, Entity *b);

	void registerTriggerExit(Entity *a, Entity *b);

	void setGravity(const b2Vec2 &vec);

	b2Vec2 getGravity();

	std::vector<std::tuple<Entity*, Entity*>  > & getCollisionEnter();

	std::vector<std::tuple<Entity*, Entity*>  > & getCollisionExit();

	std::vector<std::tuple<Entity*, Entity * > > & getTriggerEnter();

	std::vector<std::tuple<Entity*, Entity * > > & getTriggerExit();

	void clearCollisions();
	
private:

	friend class Engine;
	friend class RigidBody;

	//synchronise the transform with the rigid body
	void syncRigidBody(Entity &e);

	void createColliders(Entity &e);

	//sync the rigid body with the Transform
	void syncTransform(Entity &e);

	b2Vec2 _gravity;
	b2World* _world;
	std::unordered_map<size_t, b2Body *> _bodies;
	time_point<system_clock> _lastWorldStepTime;
	b2Body* createBody(size_t EntityID, Position &position, float physicsScale);
	std::vector<std::tuple<Entity *, Entity*> > _collisionEnter, _collisionExit;
	std::vector<std::tuple<Entity *, Entity *> > _triggerEnter, _triggerExit;
	void registerContactListener(Collider &col);
	b2Body* getb2Body(size_t _id)
	{
		if (_bodies.count(_id))
		{
			return _bodies[_id];
		}
		return nullptr;
	}
};

