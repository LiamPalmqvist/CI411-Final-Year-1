#include "Game.h"
#include "GameObjects.h"
// ======================================================= 
// SDL Variables
SDL_Renderer* Game::renderer = nullptr;;
SDL_Event Game::playerInputEvent;
GameInput playerInput;

// Game Objects
PlayerCharacter* pc = nullptr;
Projectile* bulletsPC[20];

// ======================================================= 

void Game::createGameObjects()
{
	printf("\nCreating Game Objects");
	
	// Create Game Objects - filename , x and y pos, initial angle
	pc = new PlayerCharacter("assets/images/playerSprite.png", (SCREEN_WIDTH/2-SPRITE_SIZE/2), (SCREEN_HEIGHT/2-SPRITE_SIZE/2), 0);

	//create PC bullets but do not enable
	for (int i = 0; i < sizeof(bulletsPC) / sizeof(bulletsPC[0]); i++)
	{
		bulletsPC[i] = new Projectile("assets/images/bullet_small.png", -100, -100, 0, 8);
		bulletsPC[i]->setBulletSpeed(500);
		bulletsPC[i]->setDamage(35);
	}
}//----

void Game::checkAttacks()
{
	if (pc->isFiring()) // Space 
	{
		std::cout << "Space is being pressed";
		switch (bulletTier)
		{

		case 0:
			// find the first inactive bullet and enable it at the PC Position
			for (Projectile* bullet : bulletsPC)
			{
				if (bullet->getAliveState() == false && coolDown <= 0)
				{	// fire in the direction the pc is facing
					
					bullet->fire(pc->getX(), pc->getY(), 0);
					coolDown = 6; // Set the cooldown
					break; // stop checking the array
				}
			}
			std::cout << "Firing" << coolDown;
			break;

		case 1:
			for (Projectile* bullet : bulletsPC)
			{
				if (bullet->getAliveState() == false && coolDown <= 0)
				{	// fire in the opposite direction the pc is facing

					bullet->fire((pc->getX() + SPRITE_SIZE / 4) + 4, (pc->getY() + SPRITE_SIZE), 180);
					coolDown = 6; // Set the cooldown
					break; // stop checking the array
				}
			}
			break;
		}
	}
	coolDown--;
}

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
	
	for (Projectile* bullet : bulletsPC)
	{
		if (bullet->getAliveState()) { bullet->update(frameTime); }
	}
	
	checkAttacks();
}

void Game::render()
{
	SDL_RenderClear(renderer);
	pc->renderPC();
	
	for (Projectile* bullet : bulletsPC)
	{
		if (bullet->getAliveState()) { bullet->renderProjectile(); }
	}

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

// TODO: Make tiers of bullets
// TODO: Make collisions
// TODO: Make enemies