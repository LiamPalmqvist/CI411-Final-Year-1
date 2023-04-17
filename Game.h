#pragma once

// ======================================================= 
// Libraries / Headers to include 
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include <string>


// Constants
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define ROWS = 18
#define COLUMNS = 25
#define SPRITE_SIZE 32
#define FPS 75


// ======================================================= 
class Game
{
public:
	Game(); // constructor
	bool isRunning() { return gameRunning; }
	void startSDL(const char* title);
	void createGameObjects();
	void loadMap(int levelNumber);
	void handleEvents();
	void checkAttacks();
	void checkCollision(float frameTime);
	void update(float frametime);
	void render();
	void closeSDL();

	static SDL_Renderer* renderer;
	static SDL_Event playerInputEvent;

private:
	bool gameRunning = false;
	SDL_Window* gameWindow = nullptr;

	int coolDown = 0;
	int bulletTier = 0;
	int activeItems = 0;
	int activeNPCs = 0;
};

// =======================================================
struct GameInput
{
	int mouseX = 0, mouseY = 0;
	bool mouseL = false, mouseR = false;
	bool mouseUpL = false;
	bool mouseWheelUp = false, mouseWheelDown = false;
	int keyDown = NULL;
	int keyUp = NULL;
};