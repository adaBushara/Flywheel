#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include "Entity.h"
#include "Position.h"
#include "Sprite.h"
#include "RigidBody.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"
#include "ChaiBehaviour.h"
#include "Transform.h"
//namespaces let us add a name in front of our function
//so as to separate them from other libraries
//this ensures that we are nto running into any function naming conflicts
namespace Scene
{

	void load(const char * file);

}