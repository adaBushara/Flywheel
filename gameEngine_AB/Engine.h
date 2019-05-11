#pragma once
#include <SDL.h>
#include "RenderingSystem.h"
#include "PhysicsSystem.h"
#include "BehaviourSystem.h"
#include "MovementSystem.h"

class PhysicsSystem;

class Engine
{
private:
	SDL_Window *window;
	bool quit;
	RenderingSystem *renderingSystem;
	MovementSystem *movementSystem;
	PhysicsSystem *physicsSystem;
	BehaviourSystem *behaviourSystem;

public:
	void Start();
	void Update();
	static Engine *getInstance();
	PhysicsSystem* getPhysicsSystem();
	BehaviourSystem* getBehaviourSystem();

public:
	float physicsScale;

private:
	Engine();
	~Engine();
};