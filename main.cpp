// CI411_Final_Year_1_Liam_Palmqvist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Game.h"

// Variables
Game* game = nullptr;

// ======================================================= 
int main(int argc, char* argv[])
{
	// Frame Limit Variables
	const int frameDelay = 1000 / FPS;
	Uint64 frameStart = 0, frameTime = 0;

	// Create the Game Object
	game = new Game;

	// Start SDL & Create the Game Window
	game->startSDL("CI411 SDL Final - Liam Palmqvist");

	game->createGameObjects();

	// Main Game Loop
	while (game->isRunning())
	{
		frameStart = SDL_GetTicks64();

		game->handleEvents();
		game->update(frameTime);
		game->render();

		// Limit Frame Rate
		frameTime = SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
	}

	game->closeSDL();

	return 0;
}//---