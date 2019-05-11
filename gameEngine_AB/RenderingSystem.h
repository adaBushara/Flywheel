#pragma once
#include "ISystem.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>

class RenderingSystem : System<RenderingSystem>
{
public:
	RenderingSystem(SDL_Window *window);
	~RenderingSystem();

	void Update();

private:
	SDL_Renderer *renderer; 
	SDL_Texture* _loadTexture(std::string &texture);
	std::unordered_map<std::string, SDL_Texture*> _textureMap; //stores our textures by name
};