#pragma once
#include "Component.h"
#include "collider.h"

class RectangleCollider :public Component<RectangleCollider>,public Collider
{
public:
	RectangleCollider() : width(1.0f), height(1.0f)	{}
	float width, height;
	friend class Entity;
};