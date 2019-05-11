#pragma once
#include "Component.h"
class Position : public Component<Position>
{
public:
	//we use an initializer list
	//call the constructors of member variables
	//these can be inhereted or declared variables
	Position() : x(0.0f), y(0.0f)
	{

	}

	~Position() {}

	//store our x and y positions
	float x, y;

	//internal type is the data type of our x/y components
	//this is useful for template stuff
	typedef decltype(x) internal_type; 
};



