#pragma once
#include "Component.h"
class Sprite : public Component<Sprite>
{
public:
	//ID of our texture
	std::string texture;
	int height, width; //height and width of pixels
	
};