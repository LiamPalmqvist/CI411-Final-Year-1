#include "Game.h"
#include "GameObjects.h"
#include "Levels.h"
// ======================================================= 
// SDL Variables
SDL_Renderer* Game::renderer = nullptr;;
SDL_Event Game::playerInputEvent;
GameInput playerInput;

// Game Objects
PlayerCharacter* pc = nullptr;
GameObject* grid;
GameObject* background[2];
GameObject* walls[80];
GameObject* movingWalls[450];
Projectile* bulletsPC[1000] = {};
NPC* npcs[20];
Projectile* bulletsNPC[20] = {};
Levels* levelMaps = nullptr;


// ======================================================= 

void Game::createGameObjects()
{
	levelMaps = new Levels;

	printf("\nCreating Game Objects");
	
	// Create the background first as it is the first layer
	background[0] = new GameObject("assets/images/background.jpg", SCREEN_WIDTH/3, 0);
	background[1] = new GameObject("assets/images/background.jpg", SCREEN_WIDTH/3, -1080);
	background[0]->setSize(720, 1080);
	background[1]->setSize(720, 1080);
	// Set these to be active all the time
	background[0]->setAlive(true); 
	background[1]->setAlive(true);
	background[0]->setYVel(20);
	background[1]->setYVel(20);
	
	// This will be the UI later
	grid = new GameObject("assets/images/grid_updated.png", 0, 0);
	grid->setSize(1920, 1080);
	grid->setAlive(true);
	
	// Create Game Objects - filename , x and y pos, initial angle
	pc = new PlayerCharacter("assets/images/playerSprite.png", (SCREEN_WIDTH/2-SPRITE_SIZE/2), (SCREEN_HEIGHT/2-SPRITE_SIZE/2), 0);

	// Create an Array of NPCs
	for (int i = 0; i < sizeof(npcs) / sizeof(npcs[0]); i++)
	{
		npcs[i] = new NPC("assets/images/Circle_Red.png", -100, -100, 180);
		npcs[i]->setSpeed(20);
		npcs[i]->setNextShotTime(rand() % 10000); // Set Random shot time upto 10 Secs
	}

	//create PC bullets but do not enable
	for (int i = 0; i < sizeof(bulletsPC) / sizeof(bulletsPC[0]); i++)
	{
		bulletsPC[i] = new Projectile("assets/images/bullet_small.png", -100, -100, 0, 8);
		bulletsPC[i]->setBulletSpeed(500);
		bulletsPC[i]->setDamage(35);
	}

	//create NPC bullets but do not enable
	for (int i = 0; i < sizeof(bulletsNPC) / sizeof(bulletsNPC[0]); i++)
	{
		bulletsNPC[i] = new Projectile("assets/images/Circle_Red_8.png", -100, -100, 90, 8);
		bulletsNPC[i]->setBulletSpeed(250);
		bulletsNPC[i]->setDamage(20);
	}

	// Create wall objects but do not enable
	for (int i = 0; i < sizeof(walls) / sizeof(walls[0]); i++)
	{
		walls[i] = new GameObject("assets/images/blank.png", 0, 0);
	}

	for (int i = 0; i < sizeof(movingWalls) / sizeof(movingWalls[0]); i++)
	{
		movingWalls[i] = new GameObject("assets/images/Square_Cross_Blue.png", 0, 0);
		movingWalls[i]->setYVel(20);
	}
	loadMap(5);
}//----

void Game::loadMap(int levelNumber)
{
	for (int row = 0; row < 35; row++)
	{
		for (int col = 0; col < 2; col++)
		{
			// Setting walls for the player
			for (GameObject* wall : walls)
			{
				if (wall->getAliveState() == false)
				{
					wall->setAlive(true);
					if (col == 0)
					{
						wall->setX((SCREEN_WIDTH / 3));
					}
					else if (col == 1)
					{
						wall->setX((2 * (SCREEN_WIDTH / 3)) - 2 * SPRITE_SIZE);
					}
					wall->setY((row * SPRITE_SIZE));
					break;
				}
			}
		}
	}

	for (int row = MAP_HEIGHT; row > -1; row--)
	{
		for (int col = 17; col > 0; col--)
		{

			if (levelMaps->getTileContent(levelNumber, col, row) == 2) // PC
			{
				pc->setX((SCREEN_WIDTH / 3) + (col * SPRITE_SIZE));
				pc->setY((row * SPRITE_SIZE) - SCREEN_HEIGHT / 2);
			}

			
			if (levelMaps->getTileContent(levelNumber, col, row) == 3) //  NPC
			{
				for (NPC* npc : npcs)
				{
					if (npc->getAliveState() == false)
					{
						npc->setAlive(true);
						npc->setX((SCREEN_WIDTH / 3) + (col * SPRITE_SIZE));
						npc->setY((row * SPRITE_SIZE) - SCREEN_HEIGHT / 2);
						break;
					}
				}
			}

			if (levelMaps->getTileContent(levelNumber, col, row) == 5) //  Moving blocks
			{
				for (GameObject* block : movingWalls)
				{
					if (block->getAliveState() == false)
					{
						block->setAlive(true);
						block->setX((SCREEN_WIDTH / 3) + (col * SPRITE_SIZE));
						block->setY((row * SPRITE_SIZE) - SCREEN_HEIGHT/2);
						break;
					}
				}
			}
			/*
			if (levelMaps.getTileContent(levelNumber, col, row) == 4) //  Items
			{
				for (GameObject* item : items)
				{
					if (item->getAliveState() == false)
					{
						item->setAlive(true);
						item->setX(col * SPRITE_SIZE);
						item->setY(row * SPRITE_SIZE);
						break;
					}
				}
			}
			*/
			
	
		}
	}
	
}//---

void Game::checkAttacks()
{
	// This part is for the Player Character
	if (pc->isFiring()) // Space 
	{
		switch (pc->getBulletTier())
		{

		case 0:
			// find the first inactive bullet and enable it at the PC Position
			for (Projectile* bullet : bulletsPC)
			{
				if (bullet->getAliveState() == false && coolDown <= 0)
				{	// fire in the direction the pc is facing
					
					bullet->fire((pc->getX() + SPRITE_SIZE / 2) - 4, pc->getY() + 1, 0);
					coolDown = 6; // Set the cooldown
					break; // stop checking the array
				}
			}
			break;

		case 1:
			for (int i = 0; i < sizeof(bulletsPC) / sizeof(bulletsPC[0]); i += 2)
			{
				if (bulletsPC[i]->getAliveState() == false && coolDown <= 0)
				{	// fire in the opposite direction the pc is facing
					
					bulletsPC[i]->fire((pc->getX() + SPRITE_SIZE / 2) - 4, pc->getY() + 1, 0); // up
					bulletsPC[i+1]->fire((pc->getX() + SPRITE_SIZE / 4) + 4, (pc->getY() + SPRITE_SIZE), 180); // down
					coolDown = 6; // Set the cooldown
					break; // stop checking the array
				}
			}
			break;
		case 2:
			for (int i = 0; i < sizeof(bulletsPC) / sizeof(bulletsPC[0]); i += 4)
			{
				if (bulletsPC[i]->getAliveState() == false && coolDown <= 0)
				{	// fire in the opposite direction the pc is facing

					bulletsPC[i]->fire((pc->getX() + SPRITE_SIZE / 2) - 8, pc->getY() + 1, 0);
					bulletsPC[i + 1]->fire((pc->getX() + SPRITE_SIZE / 2), pc->getY() + 1, 0);
					bulletsPC[i + 2]->fire((pc->getX() + SPRITE_SIZE / 4) + 8, (pc->getY() + SPRITE_SIZE), 180);
					bulletsPC[i + 3]->fire((pc->getX() + SPRITE_SIZE / 4), (pc->getY() + SPRITE_SIZE), 180);
					coolDown = 6; // Set the cooldown
					break; // stop checking the array
				}
			}
			break;
		case 3:
			for (int i = 0; i < sizeof(bulletsPC) / sizeof(bulletsPC[0]); i += 6)
			{
				if (bulletsPC[i]->getAliveState() == false && coolDown <= 0)
				{	// Fire up, down, left, right
					bulletsPC[i]->fire((pc->getX() + SPRITE_SIZE / 2) - 8, pc->getY() + 1, 0); // up 1
					bulletsPC[i + 1]->fire((pc->getX() + SPRITE_SIZE / 2), pc->getY() + 1, 0); // up 2
					bulletsPC[i + 2]->fire((pc->getX() + SPRITE_SIZE / 4) + 8, (pc->getY() + SPRITE_SIZE), 180); // down 1
					bulletsPC[i + 3]->fire((pc->getX() + SPRITE_SIZE / 4), (pc->getY() + SPRITE_SIZE), 180); // down 2
					bulletsPC[i + 4]->fire((pc->getX() + SPRITE_SIZE), (pc->getY() + SPRITE_SIZE / 4) + 4, 90); // left
					bulletsPC[i + 5]->fire((pc->getX() + SPRITE_SIZE / 4), (pc->getY() + SPRITE_SIZE / 4) + 4, 270); // right
					coolDown = 6; // Set the cooldown
					break; // stop checking the array
				}
			}
			break;
		case 4:
			for (int i = 0; i < sizeof(bulletsPC) / sizeof(bulletsPC[0]); i += 6)
			{
				if (bulletsPC[i]->getAliveState() == false && coolDown <= 0)
				{	// Fire up, down, left, right
					bulletsPC[i]->fire((pc->getX() + SPRITE_SIZE / 2) - 4, pc->getY() + 1, 0); // up
					bulletsPC[i + 1]->fire((pc->getX() + SPRITE_SIZE), pc->getY() + 1, 45); // diag up right
					bulletsPC[i + 2]->fire((pc->getX() + SPRITE_SIZE / 4) + 4, (pc->getY() + SPRITE_SIZE), 180); // down
					bulletsPC[i + 3]->fire((pc->getX() + SPRITE_SIZE), (pc->getY() + SPRITE_SIZE / 4) + 4, 90); // left
					bulletsPC[i + 4]->fire((pc->getX() + SPRITE_SIZE / 4), (pc->getY() + SPRITE_SIZE / 4) + 4, 270); // right
					bulletsPC[i + 5]->fire((pc->getX() - SPRITE_SIZE / 4), (pc->getY() + 1), 315); // diag up left

					coolDown = 6; // Set the cooldown
					break; // stop checking the array
				}
			}
			break;
		case 5:
			for (int i = 0; i < sizeof(bulletsPC) / sizeof(bulletsPC[0]); i += 8)
			{
				if (bulletsPC[i]->getAliveState() == false && coolDown <= 0)
				{	// Fire up, down, left, right
					bulletsPC[i]->fire((pc->getX() + SPRITE_SIZE / 2) - 4, pc->getY() + 1, 0); // up
					bulletsPC[i + 1]->fire((pc->getX() + SPRITE_SIZE), pc->getY() + 1, 45); // diag up right
					bulletsPC[i + 2]->fire((pc->getX() + SPRITE_SIZE / 4) + 4, (pc->getY() + SPRITE_SIZE), 180); // down
					bulletsPC[i + 3]->fire((pc->getX() + SPRITE_SIZE), (pc->getY() + SPRITE_SIZE), 135); // diag down right
					bulletsPC[i + 4]->fire((pc->getX() + SPRITE_SIZE), (pc->getY() + SPRITE_SIZE / 4) + 4, 90); // left
					bulletsPC[i + 5]->fire((pc->getX() - SPRITE_SIZE / 4), (pc->getY() + SPRITE_SIZE), 225); // diag down left
					bulletsPC[i + 6]->fire((pc->getX() + SPRITE_SIZE / 4), (pc->getY() + SPRITE_SIZE / 4) + 4, 270); // right
					bulletsPC[i + 7]->fire((pc->getX() - SPRITE_SIZE / 4), (pc->getY() + 1), 315); // diag up left

					coolDown = 6; // Set the cooldown
					break; // stop checking the array
				}
			}
			break;
		}
	}
	coolDown--;

	// NPC attacks
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState() == true)
		{
			if (npc->getNextShotTime() < SDL_GetTicks())
			{
				// find the first inactive bullet and enable it at the PC Position
				for (Projectile* bullet : bulletsNPC)
				{
					if (bullet->getAliveState() == false)
					{
						// Fire at PC
						bullet->fireAtTarget(npc->getX(), npc->getY(), pc->getX(), pc->getY());

						// fire Down
						//bullet->fireAtTarget(npc->getX(), npc->getY(), npc->getX(), SCREEN_HEIGHT);

						// Set Random shot time - Current time + 3s + random upto 7s
						npc->setNextShotTime(SDL_GetTicks() + 3000 + rand() % 7000);
						break; // stop checking the bullet array
					}
				}
			}
		}
	}
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
		// std::cout << "\n" << playerInputEvent.key.keysym.scancode << " + " << playerInputEvent.key.keysym.mod << " up.";
		playerInput.keyUp = playerInputEvent.key.keysym.scancode;
		break;

	default:
		break;
	}
}//---

void Game::checkCollision(float frameTime)
{
	// Create the Rects for checking what collides
	SDL_Rect pcRect = { pc->getX(), pc->getY(), SPRITE_SIZE, SPRITE_SIZE };
	SDL_Rect objectRect = { -100,-100, SPRITE_SIZE, SPRITE_SIZE };
	SDL_Rect npcRect = { -100,-100, SPRITE_SIZE, SPRITE_SIZE };
	SDL_Rect bulletRect = { 0,0,0,0 };

	// Set the PC's next Position by adding velocity
	SDL_Rect pcNextRect = { pc->getX() + pc->getVelX(), pc->getY() + pc->getVelY(), SPRITE_SIZE, SPRITE_SIZE };

	for (GameObject* wall : walls)  //  Terrain -----------------
	{
		if (wall->getAliveState())
		{
			objectRect.x = wall->getX();
			objectRect.y = wall->getY();

			if (SDL_HasIntersection(&pcNextRect, &objectRect))  // PC -----
			{
				pc->stop(); // Stop the PC moving
			}


			for (NPC* npc : npcs) // NPCs ---------
			{
				if (npc->getAliveState() == true)
				{
					npcRect.x = npc->getX(); // Update the rect
					npcRect.y = npc->getY();

					if (SDL_HasIntersection(&npcRect, &objectRect))
					{
						npc->changeDirection();
					}
				}
			}


			for (Projectile* bullet : bulletsPC)  // PC  Bullets -----------------
			{
				if (bullet->getAliveState() == true)
				{
					bulletRect.x = bullet->getX(); // Update the rect
					bulletRect.y = bullet->getY();
					bulletRect.w = bulletRect.h = bullet->getSize();

					if (SDL_HasIntersection(&objectRect, &bulletRect))
					{
						bullet->setAlive(false); // disable bullet
					}
				}
			}
		}
	}

	for (Projectile* bullet : bulletsNPC)  //  NPC Bullets -----------------
	{
		if (bullet->getAliveState())
		{
			bulletRect.x = bullet->getX(); // Update the rect
			bulletRect.y = bullet->getY();
			bulletRect.w = bulletRect.h = bullet->getSize();

			if (SDL_HasIntersection(&pcRect, &bulletRect))  //  PC ------
			{
				pc->changeHP(-bullet->getDamage()); // Apply damage
				bullet->setAlive(false); // disable bullet
			}

			for (GameObject* block : walls)  //  Terrain ------
			{
				if (block->getAliveState())
				{
					objectRect.x = block->getX();
					objectRect.y = block->getY();

					if (SDL_HasIntersection(&objectRect, &bulletRect))
					{
						bullet->setAlive(false); // disable bullet
					}
				}
			}
		}
	}

	// check what alive NPCs hit -
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState())
		{
			npcRect.x = npc->getX(); // Update the rect
			npcRect.y = npc->getY();

			for (Projectile* bullet : bulletsPC)  // PC  Bullets -----------------
			{
				if (bullet->getAliveState() == true)
				{
					bulletRect.x = bullet->getX(); // Update the rect
					bulletRect.y = bullet->getY();
					bulletRect.w = bulletRect.h = bullet->getSize();

					if (SDL_HasIntersection(&npcRect, &bulletRect)) // NPC
					{
						//npc->setAlive(false); // Disable the NPC 						
						npc->changeHP(-bullet->getDamage()); // Apply damage

						bullet->setAlive(false); // disable bullet
					}
				}
			}
		}
	}

	for (GameObject* wall : movingWalls)
	{
		if (wall->getAliveState() == true)
		{
			objectRect.x = wall->getX();
			objectRect.y = wall->getY();

			if (SDL_HasIntersection(&pcRect, &objectRect))
			{
				pc->stop();
				pc->setY(pc->getY() + (20 * frameTime));
			}

			if (wall->getX() > SCREEN_WIDTH || wall->getY() > SCREEN_HEIGHT)
			{
				wall->setAlive(false);
				std::cout << "destroyed" << wall;
			}
		}
	}
}

void Game::update(float frameTime)
{
	// Ensure Frame rate is at the delay speed and convert to deltaTime
	if (frameTime < 1000 * (float)1 / FPS) frameTime = (float)1 / FPS;

	for (GameObject* image : background)
	{
		image->update(frameTime);
	}

	grid->update(frameTime);

	pc->updatePC(playerInput.keyDown, playerInput.keyUp, frameTime);
	
	// Set NPC Behaviours
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState())
		{
			if (npc->getY() > 0)
			{
				npc->roam(frameTime);
			}
			else
			{
				npc->moveDown(frameTime);
			}
			//npc->screenCrawl(frameTime);		
		}
	}

	for (Projectile* bullet : bulletsPC)
	{
		if (bullet->getAliveState()) { bullet->update(frameTime); }
	}

	for (Projectile* bullet : bulletsNPC)
	{
		if (bullet->getAliveState()) { bullet->update(frameTime); }
	}

	for (NPC* npc : npcs)
	{
		if (npc->getAliveState()) { points = npc->updateNPC(points); }
	}

	for (GameObject* wall : walls)
	{
		if (wall->getAliveState()) { wall->update(frameTime); }
	}

	for (GameObject* wall : movingWalls)
	{
		if (wall->getAliveState()) { wall->update(frameTime); }
	}
	
	checkAttacks();
	checkCollision(frameTime);
}

void Game::render()
{
	SDL_RenderClear(renderer);

	for (GameObject* image : background)
	{
		if (image->getAliveState()) { image->render(); }
	}

	grid->render();

	pc->renderPC();
	
	for (Projectile* bullet : bulletsPC)
	{
		if (bullet->getAliveState()) { bullet->renderProjectile(); }
	}

	for (Projectile* bullet : bulletsNPC)
	{
		if (bullet->getAliveState()) { bullet->renderProjectile(); }
	}

	for (NPC* npc : npcs)
	{
		if (npc->getAliveState()) { npc->renderNPC(); }
	}

	for (GameObject* wall : walls)
	{
		if (wall->getAliveState()) { wall->render(); }
	}

	for (GameObject* wall : movingWalls)
	{
		if (wall->getAliveState()) { wall->render(); }
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