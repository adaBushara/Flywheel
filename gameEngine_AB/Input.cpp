#include "Input.h"

namespace Input
{
	bool getPressed(Keys key)
	{
		const Uint8*_keyboardState = SDL_GetKeyboardState(NULL);
		return _keyboardState[(Uint8)key];
	}

	bool getMousePress(MouseButtons mouseButton)
	{
		Uint32 mouseButtonMask = SDL_BUTTON((Uint32)mouseButton);
		return ((SDL_GetMouseState(NULL, NULL) & mouseButtonMask) != 0);
	}
	
	int getMouseX()
	{
		int x;
		SDL_GetMouseState(&x, NULL);
		return x;
	}

	int getMouseY()
	{
		int y;
		SDL_GetMouseState(NULL, &y);
		return y;
	}

	bool getPressed_uint(unsigned int key)
	{
		return getPressed((Keys)key);
	}
	bool getMousePress_uint(unsigned int mouseButton)
	{
		return getMousePress((MouseButtons)mouseButton);
	}
}