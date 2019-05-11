#include "MovementSystem.h"

MovementSystem::MovementSystem()
{
	this->componentMask = Position::getMask() | Sprite::getMask();
}

MovementSystem::~MovementSystem()
{

}

void MovementSystem::Update()
{
	for (Entity& entity : Entity::getEntityList())
	{
		if ((entity.getComponentMask() & componentMask) == componentMask)
		{
			Position &p = entity.getComponent<Position>();

			if (Input::getPressed(Keys::LEFT))
			{
				p.x -= 0.1f;
			}
			if (Input::getPressed(Keys::RIGHT))
			{
				p.x += 0.1f;
			}
			if (Input::getPressed(Keys::UP))
			{
				p.y -= 0.1f;
			}
			if (Input::getPressed(Keys::DOWN))
			{
				p.y += 0.1f;
			}
		}
	}
}