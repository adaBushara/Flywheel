#pragma once
#include "ISystem.h"
#include "Entity.h"
#include "Input.h"
#include "Position.h"
#include "Sprite.h"

class MovementSystem : public System<MovementSystem>
{
public:
	MovementSystem();
	~MovementSystem();
	void Update();
};