#pragma once
#include <SDL.h>
template<typename Derrived>
class System
{
public:
	uint64_t componentMask;
	void Update()
	{
		static_cast<Derrived>(this).Update();
	}
};
