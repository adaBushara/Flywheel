#pragma once
#include "Component.h"
#include "collider.h"

class CircleCollider :public Component<CircleCollider> , public Collider
{
public:
	CircleCollider():radius(1.0f) {}
	float radius;
	friend class Entity;
};