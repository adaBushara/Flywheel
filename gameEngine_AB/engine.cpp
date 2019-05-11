#pragma once
#include "Engine.h"
#include "Entity.h"
#include "Position.h"
#include "Sprite.h"
#include "Scene.h"
#include <iostream>
#include "Input.h"

#pragma region CONSTRCTOR_AND_DESTRUCTOR
Engine::Engine()
{
	physicsScale = 100.0f;

	//Init SDL	
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "Unable to Initialize SDL" << std::endl;
	}
	window = SDL_CreateWindow
	("Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		NULL);

	if (window == nullptr)
	{
		std::cerr << "Unable to Create window" << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	//Once SDL and the window is created, we can create our other objects

	quit = false;
	renderingSystem = new RenderingSystem(window);
	movementSystem = new MovementSystem();
	physicsSystem = new PhysicsSystem();
	behaviourSystem = new BehaviourSystem();

}

Engine::~Engine()
{
	SDL_DestroyWindow(window);
	//Delete any and all pointers held within Engine
	delete renderingSystem;
	delete movementSystem;
	delete physicsSystem;
	delete behaviourSystem;

	SDL_Quit();
	
}

#pragma endregion


void Engine::Start()
{
	Scene::load("Assets\\scene.json");

	for (Entity &e : Entity::getEntityList())
	{
		physicsSystem->createColliders(e);
	}

	//signal all functions to start
	behaviourSystem->Start();

	while (!quit)
	{
		Update();
	}
}

void Engine::Update()
{
	//Check for SDL events (Mouse, keyboards, etc)
	SDL_Event e;
	while (SDL_PollEvent(&e) != NULL)
	{
		if (e.type == SDL_QUIT)
		{
			this->quit = true;
		}
	}

	//Update Systems
	renderingSystem->Update();
	movementSystem->Update();
	physicsSystem->Update();
	behaviourSystem->Update();
}

Engine* Engine::getInstance()
{
	static Engine instance;

	return &instance;
}

PhysicsSystem * Engine::getPhysicsSystem()
{
	return physicsSystem;
}

BehaviourSystem * Engine::getBehaviourSystem()
{
	return behaviourSystem;
}