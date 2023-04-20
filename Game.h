#pragma once

// ======================================================= 
// Libraries / Headers to include 
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <stdlib.h>


// Constants
#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
#define ROWS = 18
#define COLUMNS = 25
#define SPRITE_SIZE 32
#define FPS 75
#define MAP_HEIGHT 500


// ======================================================= 
class Game
{
public:
	Game(); // constructor
	bool isRunning() { return gameRunning; }
	bool inLevel() { return isInLevel; }
	bool inMenu() { return isInMenu; }
	void startSDL(const char* title);
	void createGameObjects();
	void checkGameStates();
	void loadMap(int levelNumber);
	void unloadMap();
	void loadMenu();
	void unloadMenu();
	void handleEvents();
	void handleMenuEvents();
	void checkAttacks();
	void checkCollision(float frameTime);
	void update(float frametime);
	void updateMenu(float frameTime);
	void updateGUI();
	void updateMenuGUI();
	void render();
	void renderMenu();
	void closeSDL();

	static SDL_Renderer* renderer;
	static SDL_Event playerInputEvent;

	SDL_Window* gameWindow = nullptr;

private:
	bool gameRunning = false;
	bool isInLevel = false;
	bool isInMenu = true;
	int hurtTimer = 100;
	bool hurt = false;
	int mapSelected = 1;

	int coolDown = 0;
	int points = 0;
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