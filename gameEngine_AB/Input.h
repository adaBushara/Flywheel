#pragma once
#include <SDL.h>
#include "Keys.h"
#include "Mouse.h"

namespace Input
{
	bool getPressed(Keys key);
	bool getMousePress(MouseButtons mouseButton);
	int getMouseX();
	int getMouseY();


	bool getPressed_uint(unsigned int key);
	
	bool getMousePress_uint(unsigned int mouseButton);
	

}