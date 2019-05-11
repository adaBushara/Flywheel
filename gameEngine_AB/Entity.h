#pragma once
#include <SDL.h>
#include <type_traits>
#include <vector>
#include "collider.h"
#include <tuple>

class Entity
{
public:
	
	~Entity();
	unsigned int num_components;
	template <typename C>
	C& AddComponent()
	{
		
		static_assert(
			std::is_base_of<Component<C>, C>::value,
			"ERROR: Can only add Component Types");
		_componentMask = _componentMask | C::getMask();
		C& ret = C::addComponentWithEntityID(_ID);
		ret.owner = this;
		//ret._ID = C::UID_counter++;
		return ret;
	}


	/**
	Returns the first component in the list	
	*/
	template <typename C>
	C& getComponent()
	{
		return C::getComponentWithEntityID(_ID);
	}

	template <typename C>
	std::vector<C>& getComponents()
	{
		return C::getComponentsWithEntityID(_ID);
	}

	template <class C>
	bool hasComponent()
	{
		return (getComponentMask() & C::getMask()) == C::getMask();
	}
	


	template <typename C>
	void RemoveComponent(C& comp)
	{
		C::removeComponentWithEntityID(_ID, comp);
	}

	uint64_t getComponentMask();

	size_t getID()
	{
		return this->_ID;
	}

	
	
	//static Methods
	static std::vector<Entity>& getEntityList();
	static Entity& create();


	static Entity& getEntityWithID(size_t ID)
	{
		return _entityList[ID];
	}

private:
	Entity();
	Entity(size_t ID);
	
	
	
	size_t _ID;
	uint64_t _componentMask;

	static std::vector<Entity> _entityList;
};