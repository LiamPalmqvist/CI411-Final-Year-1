#include "Game.h"
#include "GameObjects.h"
// ======================================================= 
// SDL Variables
SDL_Renderer* Game::renderer = nullptr;;
SDL_Event Game::playerInputEvent;
GameInput playerInput;

// Game Objects
PlayerCharacter* pc = nullptr;

// ======================================================= 

void Game::createGameObjects()
{
	printf("\nCreating Game Objects");
	
	// Create Game Objects - filename , x and y pos, initial angle
	pc = new PlayerCharacter("assets/images/Pawn_Purple.png", 0, 0, 0);
}//----

void Game::handleEvents()
{
	// Reset Inputs
	playerInput.keyDown = NULL;
	playerInput.keyUp = NULL;
	
	//Check for Events
	SDL_PollEvent(&playerInputEvent);

	switch (playerInputEvent.type)
	{
	case SDL_QUIT:
		gameRunning = false;
		break;

	case SDL_KEYDOWN:
		std::cout << "\n" << playerInputEvent.key.keysym.scancode << " + " << playerInputEvent.key.keysym.mod << " down.";
		playerInput.keyDown = playerInputEvent.key.keysym.scancode;
		break;

	case SDL_KEYUP:
		std::cout << "\n" << playerInputEvent.key.keysym.scancode << " + " << playerInputEvent.key.keysym.mod << " up.";
		playerInput.keyUp = playerInputEvent.key.keysym.scancode;
		break;

	default:
		break;
	}
}//---

void Game::update(float frameTime)
{
	// Ensure Frame rate is at the delay speed and convert to deltaTime
	if (frameTime < 1000 * (float)1 / FPS) frameTime = (float)1 / FPS;

	pc->updatePC(playerInput.keyDown, playerInput.keyUp, frameTime);
}

void Game::render()
{
	SDL_RenderClear(renderer);
	pc->renderPC();
	SDL_RenderPresent(renderer); 	// Update the screen
}//---

void Game::startSDL(const char* title)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "\nSDL Initialised  \n";
		gameWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (gameWindow) printf("\nGame Window Created ");
		renderer = SDL_CreateRenderer(gameWindow, -1, 0);
		if (renderer) printf("\nRenderer Created \n");
		gameRunning = true;
	}
	else
	{
		gameRunning = false;
	}
}//---

void Game::closeSDL() // Clear Memory and exit SDL
{
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "\nSDL Closed \n";
}//---

Game::Game() // Constructor
{
	printf(" \n\n ----- Game Started ------ \n");
}