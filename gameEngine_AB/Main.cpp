#pragma once
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <SDL.h>
#include "Engine.h"
#include "vector2.h"
int main(int argc, char *argv[])
{
	Engine *engine;

	engine = Engine::getInstance();

	engine->Start();

	

	return EXIT_SUCCESS;
}