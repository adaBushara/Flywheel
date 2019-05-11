#pragma once
#include "RenderingSystem.h"
#include "Entity.h"
#include "Position.h"
#include "Sprite.h"
#include "Rotator.h"


RenderingSystem::RenderingSystem(SDL_Window * window)
{
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		std::cerr << "Unable to create SDL Renderer: " << std::endl;
		std::cerr << SDL_GetError() << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	int IMG_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ( (IMG_Init(IMG_flags) & IMG_flags) != IMG_flags)
	{
		std::cerr << "Could Not Initialize SDL_IMAGE" << std::endl;
		std::cerr << IMG_GetError() << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	//the components that we are looking for, we need both a position and sprite
	componentMask = Position::getMask() | Sprite::getMask();

}

RenderingSystem::~RenderingSystem()
{
	SDL_DestroyRenderer(renderer);
}

void RenderingSystem::Update()
{
	//set clear colour
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

	//clear the screen buffer
	SDL_RenderClear(renderer);

	//loop through and draw all entities
	//using a for-each loop as we do not care for index
	//there are many ways of looping through a list
	//for-each
	//using c# iterator
	for (Entity &entity : Entity::getEntityList())
	{
		//only want to render an entity if it contains a sprite & position mask
		if ((entity.getComponentMask() & this->componentMask) == componentMask)
		{
			//get the position and sprite components
			Position& position = entity.getComponent<Position>();
			Sprite& sprite = entity.getComponent<Sprite>();
			//check if the sprites texture is loaded
			//do not want to load multiple of the same image
			//can simply reference one
			if (_textureMap.count(sprite.texture) < 1)
			{
				//we use [square] brackets to access elements
				//we use (round) brackets to pass arguments
				//we use {curly} bracets for sectioning code and denoting lists
				_textureMap[sprite.texture] = _loadTexture(sprite.texture);
				//_tectureMap is an unordered map
				//this means that elements are not always stored in order
				//they are stored as key/value pairs
				//the sprite.texture is the key, SDL_Texture is the value
				//the definition of _textureMap:
				//std::unordered_map<std::string, SDL_Texture*> _textureMap;
			}

			//set the sprite's height and width
			SDL_QueryTexture(_textureMap[sprite.texture], NULL, NULL, &sprite.width, &sprite.height);
			//values get assigned to height and width
			//& is the reference command
			//the pointer or the memory adress of the variable
			//if it is the pointer then there is no need to use &
			//as pointers * are already memories

			//create drawing rectangles
			//curly bracets are used to initiate a structure
			//structure is similar to a class
			//structure does not contain methods
			//to create one you have to either provide a list of values
			//or assign each one individuallt as the need arrives
			int rendererHeight;
			SDL_GetRendererOutputSize(renderer, nullptr, &rendererHeight);

			SDL_Rect sourceRect = { 0, 0, sprite.width, sprite.height };//image source
			//image destination
			//where it will be drawn on the screen
			SDL_Rect destRect = { (int)position.x - (sprite.width / 2),
				rendererHeight - (int)position.y - (sprite.height / 2) ,
				sprite.width,
				sprite.height };

			//blit the texture to screen
			//blit means to copy
			/*SDL_RenderCopy(renderer, _textureMap[sprite.texture], &sourceRect, &destRect);*/
			double angle = entity.hasComponent<Rotator>() ? (double)entity.getComponent<Rotator>().angle : 0;
			angle *= -1; //sync angle with box2D (for intuitive reasons)
			SDL_RenderCopyEx(renderer, _textureMap[sprite.texture], &sourceRect, &destRect, angle, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
		}
	}

	//update screen
	SDL_RenderPresent(renderer);
}

//loads and returns a texture pointer based off a file path
SDL_Texture * RenderingSystem::_loadTexture(std	::string &texture)
{
	//load texture into SDL_Surface
	//SDL_Surface is a texture that exists in CPU ram
	//we would use this for software rendering
	SDL_Surface* surface = IMG_Load(texture.c_str());
	if (surface == NULL)
	{
		std::cerr << "Unable to load image" << std::endl;
		std::cerr << IMG_GetError() << std::endl;
		system("Pause");
		exit(EXIT_FAILURE);
	}
	//attempt to initialize texture in VRAM (gpu memory)
	SDL_Texture * spriteTexture = SDL_CreateTextureFromSurface(renderer, surface);
	if (spriteTexture == nullptr)
	{
		std::cerr << "Unable to create texture from surface" << std::endl;
		std::cerr << SDL_GetError() << std::endl;
		system("Pause");
		exit(EXIT_FAILURE);
	}
	SDL_FreeSurface(surface);
	return spriteTexture;
}