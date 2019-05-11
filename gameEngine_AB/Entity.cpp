#pragma once
#include "Entity.h"

Entity::Entity()
{

}

Entity::Entity(size_t ID) :
	_ID(ID), _componentMask(0)
{

}

Entity::~Entity()
{

}

uint64_t Entity::getComponentMask()
{
	return _componentMask;
}

//statics
std::vector<Entity> Entity::_entityList;

std::vector<Entity>& Entity::getEntityList()
{
	return Entity::_entityList;
}

Entity& Entity::create()
{
	//Create an entity and give it a unique ID
	//this ID is tied to the length of the vector
	Entity e(_entityList.size() );//create entity in entitylist.size(), this will be length+1 of the vector
	_entityList.push_back(e);//add it to the back of the list
	return _entityList.back();//return the last element(it will always be the one we just added)
}
