#pragma once
#include <SDL.h>
#include <memory>
#include <unordered_map>
#include <vector>

class Entity;

class BaseComponent
{
protected:
	static uint64_t _componentTypeCount;
};

template <typename Derived>
class Component : public BaseComponent
{
public:
	//Mask is the Unique of ID of every component type
	//ex Position has mask value of 1b
	//Sprite has mask value of 10b
	static uint64_t getMask()
	{
		static uint64_t _mask = 1 << _componentTypeCount++;
		return _mask;
	}

	size_t getID()
	{
		static size_t _ID = UID_counter++;
		return _ID;
	}

	Entity& getOwner()
	{
		return *owner;
	}

	Component()
	{
		getID();
	}
	
private:
	friend class Entity;
	static std::unordered_map<uint64_t, std::vector<Derived> > componentMap;
	static size_t UID_counter;


	//size_t is generally defined as an unsigned integer
	static Derived& addComponentWithEntityID(size_t & _ID)
	{
		Derived newComponent = Derived();
		componentMap[_ID].push_back(newComponent);
		return componentMap[_ID].back();
	}

	static Derived& getComponentWithEntityID(size_t & _ID)
	{
		
			return componentMap[_ID].front();
		
	}
	
	static std::vector<Derived> & getComponentsWithEntityID(size_t & _ID)
	{
		return componentMap[_ID];
	}

	static void removeComponentWithEntityID(size_t &_ID, Derived &ent)
	{
		//componentMap.erase(_ID);
		//componentMap[_ID].remove()
		//TODO:
		//search through index
		//remove index
		auto _componentList = componentMap[_ID];
		for (size_t i = 0; i < _componentList.size(); i++)
		{
			if ( (_componentList[i].getID()) == (ent.getID()) )
			{
				_componentList.erase(_componentList.begin() + i);
				break;
			}
		}
	}

	static bool hasComponents(size_t &_ID)
	{
		return componentMap[_ID].empty();
	}

	Entity *owner;



};

//static initialization of component map for all classes
template<typename Derived>
std::unordered_map<uint64_t, std::vector<Derived> > Component<Derived>::componentMap;

template<typename Derived>
size_t Component<Derived>::UID_counter;




//preprocessor stuff to get name of class 
//gets it as const char *
template<typename T>
struct TypeParseTraits;

#define REGISTER_PARSE_TYPE(X) template <> struct TypeParseTraits<X> \
    { static const char* name; } ; const char* TypeParseTraits<X>::name = #X

