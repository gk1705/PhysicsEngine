#include <iostream>
#include <SDL.h>
#include <chrono>

#include "PhysicsEngine.h"
#include "Rect.h"

#undef main

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Engine", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// create ground
	RigidBody* groundRigidBody = new RigidBody(0.0f, 1.f, 0.0f);
	groundRigidBody->position = { 400.0f, 550.0f };
	Rect* groundRect = new Rect(groundRigidBody, 800.0f, 100.0f);

	// create a dynamic rectangle
	RigidBody* dynamicRigidBody = new RigidBody(10.f, 0.8f, 1.0f);
	dynamicRigidBody->position = { 400.0f, 200.0f };
	Rect* dynamicRect = new Rect(dynamicRigidBody, 100.0f, 100.0f);

	// create the physics engine and add the shapes and rigid bodies
	PhysicsEngine physicsEngine;
	physicsEngine.addShape(groundRect);
	physicsEngine.addShape(dynamicRect);
	physicsEngine.addRigidBody(groundRigidBody);
	physicsEngine.addRigidBody(dynamicRigidBody);

	constexpr int FPS = 60; // default FPS
	constexpr int frameDelay = 1000 / FPS; // delay in ms per frame to achieve the target FPS

	SDL_Event event;
	bool running = true;
	Uint32 frameStart;
	int frameTime = 0;

	float deltaTimeSec = 0;
	while (running)
	{
		frameStart = SDL_GetTicks64();

		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				running = false;
				break;
			}
			default:
				break;
			}
		}

		dynamicRigidBody->force.y += 9.8f; // gravity
		physicsEngine.update(deltaTimeSec);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		// draw the ground rectangle
		SDL_Rect groundSDLRect = { static_cast<int>(groundRigidBody->position.x - 400), static_cast<int>(groundRigidBody->position.y - 50), 800, 100 };
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &groundSDLRect);

		// draw the dynamic rectangle
		SDL_Rect dynamicSDLRect = { static_cast<int>(dynamicRigidBody->position.x - 50), static_cast<int>(dynamicRigidBody->position.y - 50), 100, 100 };
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &dynamicSDLRect);

		SDL_RenderPresent(renderer);

		frameTime = SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
			deltaTimeSec = static_cast<float>(frameTime + (frameDelay - frameTime)) / 1000.f;
		}
		else
		{
			deltaTimeSec = static_cast<float>(frameTime) / 1000.f;
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}