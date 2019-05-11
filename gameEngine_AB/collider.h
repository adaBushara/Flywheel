#pragma once
#include <Box2D\Box2D.h>

//base collider class
//this will make it easier to handle multiple collision types
class Entity;

/*
our user data is filled with the entity class
with this we can check if we have a rigidBody
let the rigidBody signal all collisions (if we have multiple colliders)
if not, register the collision
*/

class Collider : public b2ContactListener
{
public:

	Collider() {}

	Entity& getEntity()
	{
		return *gameObject;
	}

	void SetOwner(Entity &e)
	{
		gameObject = &e;
	}

	void BeginContact(b2Contact* contact);
	

	void EndContact(b2Contact* contact);
	

protected:
	friend class Entity;
	Entity *gameObject;
};