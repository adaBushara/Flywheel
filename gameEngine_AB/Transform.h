#pragma once
#include "Position.h"
#include "Rotator.h"
#include <Box2D\Box2D.h>
#include "Utils.h"

class Transform : public Component<Transform>
{
public:
	
	//get the world position
	//this takes into account parent rotations + translations

	//b2Vec2 getWorldPosition()
	//{
	//	std::vector<Transform*> hierarchy;
	//	Transform *p = this;
	//	while (Utils::isValid(p->parent))
	//	{
	//		//this goes from child upwards to the root
	//		hierarchy.push_back(p);
	//		p = p->parent;
	//	}
	//	//this should give us the list of parent transforms
	//	//from here we can go up the list and transform our position values
	//	b2Vec2 result = b2Vec2(position->x, position->y);
	//	if (hierarchy.empty())
	//	{
	//		return result;
	//	}
	//	b2Vec2 tmp = b2Vec2(0, 0);
	//	for (auto it = hierarchy.begin(); it != hierarchy.end(); it++)
	//	{
	//		Transform *inst = *it;
	//		if (inst->parent != nullptr)
	//		{
	//			b2Vec2 pos = b2Vec2(inst->position->x, inst->position->y);
	//			
	//		}
	//	}
	//}

	//get the highest level parent transform
	/*Transform* getRoot()
	{

	}*/


	//Position in local space
	Position* position;
	//Local rotation around it's center point
	Rotator* rotator;
	//the parent transform, if we are parented this affects our world space position
	Transform* parent;
};


//A quick note about references in a class
//unless it is in the initalizer list
//C++ requires that classes have a fixed size
//a pointer will always have a sizeof(int)
//a reference will not have a fixed size (vs a pointer)