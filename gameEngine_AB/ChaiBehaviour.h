#pragma once
#include "Component.h"
#include <string>


class ChaiBehaviour : public Component<ChaiBehaviour>
{
protected:	
	friend class BehaviourSystem;
	std::string className; //the name of our chai script class, contains a derrived class similar to this class
};