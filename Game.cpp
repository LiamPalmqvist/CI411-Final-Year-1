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
GameObject* pc_flash;
GameObject* grid;
GameObject* background[3];
GameObject* walls[80];
GameObject* items[100];
GameObject* movingWalls[450];
GameObject* goalPost[18];
Projectile* bulletsPC[1000] = {};
NPC* npcs[20];
Projectile* bulletsNPC[200] = {};
Levels* levelMaps = nullptr;


// Menu Objects
GameObject* menuBackground;
GameObject* levelPreview[3];
GameObject* levelNumber[3];
GameObject* selectionFrame;

// Text
// Gobal Text Variables
TTF_Font* font = nullptr;
TTF_Font* font2 = nullptr;
SDL_Color textColour = { 255, 255, 200 };
SDL_Surface* textSurface = nullptr;
SDL_Texture* textTexture = nullptr;


// ======================================================= 

void Game::createGameObjects()
{
	
	levelMaps = new Levels;

	printf("\nCreating Game Objects");

	for (int i = 0; i < sizeof(background) / sizeof(background[0]); i++)
	{
		background[i] = new GameObject("assets/images/background.jpg", SCREEN_WIDTH / 3, (-1080 * i));
		background[i]->setSize(720, 1080);
		background[i]->setAlive(true); // This will be phased out to be set alive with the other gameObjects once level loading works
		background[i]->setYVel(40);
	}
	
	// This will be the UI later
	grid = new GameObject("assets/images/grid_updated.png", 0, 0);
	grid->setSize(1920, 1080);
	grid->setAlive(true);
	
	// Create Game Objects - filename , x and y pos, initial angle
	pc = new PlayerCharacter("assets/images/playerSprite.png", (SCREEN_WIDTH-SPRITE_SIZE) / 2, (SCREEN_HEIGHT-SPRITE_SIZE) / 2, 0);

	// Create the image that will flash when the player gets hit
	pc_flash = new GameObject("assets/images/playerFlash.png", (SCREEN_WIDTH - SPRITE_SIZE) / 2, (SCREEN_HEIGHT - SPRITE_SIZE) / 2);

	// Create the Array of items
	for (int i = 0; i < sizeof(items) / sizeof(items[0]); i++)
	{
		items[i] = new GameObject("assets/images/Star_Green.png", 0, 0);
	}

	// Create an Array of NPCs
	for (int i = 0; i < sizeof(npcs) / sizeof(npcs[0]); i++)
	{
		npcs[i] = new NPC("assets/images/Circle_Red.png", -100, -100, 180);
		npcs[i]->setSpeed(20);
		npcs[i]->setNextShotTime(rand() % 100); // Set Random shot time upto 1 Sec
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
		movingWalls[i]->setYVel(0);
	}

	for (int i = 0; i < sizeof(goalPost) / sizeof(goalPost[0]); i++)
	{
		goalPost[i] = new GameObject("assets/images/flag.png", 0, 0);
		goalPost[i]->setYVel(0);
	}
	
	printf("\nCreating Menu Objects");

	menuBackground = new GameObject("assets/images/menu_Background.png", 0, 0);
	menuBackground->setSize(1920, 1080);
	menuBackground->setAlive(true);
	menuBackground->setYVel(0);

	levelPreview[0] = new GameObject("assets/images/levelpreviewTemp.png", 80, 280);
	levelPreview[1] = new GameObject("assets/images/levelpreviewTemp.png", 720, 280);
	levelPreview[2] = new GameObject("assets/images/levelpreviewTemp.png", 1360, 280);
	for (int i = 0; i < sizeof(levelPreview) / sizeof(levelPreview[0]); i++)
	{		
		levelPreview[i]->setSize(480, 640);
		levelPreview[i]->setAlive(true);
		levelPreview[i]->setYVel(0);
	}

	levelNumber[0] = new GameObject("assets/images/level1temp.png", 80, 920);
	levelNumber[1] = new GameObject("assets/images/level2temp.png", 720, 920);
	levelNumber[2] = new GameObject("assets/images/level3temp.png", 1360, 920);
	for (int i = 0; i < sizeof(levelNumber) / sizeof(levelNumber[0]); i++)
	{
		levelNumber[i]->setSize(480, 80);
		levelNumber[i]->setAlive(true);
		levelNumber[i]->setYVel(0);
	}

	selectionFrame = new GameObject("assets/images/frame.png", 60, 260);
	selectionFrame->setSize(520, 760);
	selectionFrame->setAlive(true);
	selectionFrame->setYVel(0);

}//----

void Game::loadMap(int levelNumber)
{
	unloadMap();

	for (GameObject* image : background)
	{
		image->setAlive(true);
	}

	grid->setAlive(true);

	levelMaps->getLevelLength(1);
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

	for (int row = levelMaps->getLevelLength(levelNumber); row > -1; row--)
	{
		for (int col = 17; col > 0; col--)
		{

			if (levelMaps->getTileContent(levelNumber, col, row) == 2) // PC
			{
				pc->setX((SCREEN_WIDTH / 3) + (col * SPRITE_SIZE));
				pc->setY((SCREEN_HEIGHT - 32) - (row * SPRITE_SIZE));
			}

			
			if (levelMaps->getTileContent(levelNumber, col, row) == 3) //  NPC
			{
				for (NPC* npc : npcs)
				{
					if (npc->getAliveState() == false)
					{
						npc->setAlive(true);
						npc->setX((SCREEN_WIDTH / 3) + (col * SPRITE_SIZE));
						npc->setY((SCREEN_HEIGHT - 32) - (row * SPRITE_SIZE));
						std::cout
							<< "Alive: " << npc->getAliveState() << std::endl
							<< "HP: " << npc->getHP() << std::endl
							<< "Coords (X,Y): " << npc->getX() << ","
							<< npc->getY() << std::endl;
 						break;
					}
				}
			}

			if (levelMaps->getTileContent(levelNumber, col, row) == 4) //  Items
			{
				for (GameObject* item : items)
				{
					if (item->getAliveState() == false)
					{
						item->setAlive(true);
						item->setX((SCREEN_WIDTH / 3) + (col * SPRITE_SIZE));
						item->setY((SCREEN_HEIGHT - 32) - (row * SPRITE_SIZE));
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
						block->setY((SCREEN_HEIGHT - 32) - (row * SPRITE_SIZE));
						break;
					}
				}
			}

			if (levelMaps->getTileContent(levelNumber, col, row) == 6)
			{
				for (GameObject* post : goalPost)
				{
					if (post->getAliveState() == false)
					{
						post->setAlive(true);
						post->setX((SCREEN_WIDTH / 3) + (col * SPRITE_SIZE));
						post->setY((SCREEN_HEIGHT - 32) - (row * SPRITE_SIZE));
						break;
					}
				}
			}
		}
	}
	
}//---

// Basically just set everything to false
void Game::unloadMap()
{
	pc->setX(0);
	pc->setY(0);
	pc->setHP(100);

	// Unsetting walls for the player
	for (GameObject* wall : walls)
	{
		if (wall->getAliveState() == true)
		{
			wall->setAlive(false);
		}
	}	
	
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState() == true)
		{
			npc->setAlive(false);
		}
	}
	
	for (GameObject* item : items)
	{
		if (item->getAliveState() == true)
		{
			item->setAlive(false);
		}
	}
	
	for (GameObject* block : movingWalls)
	{
		if (block->getAliveState() == true)
		{
			block->setAlive(false);
		}
	}

	for (Projectile* bullet : bulletsPC)
	{
		if (bullet->getAliveState())
		{
			bullet->setAlive(false);
		}
	}

	for (Projectile* bullet : bulletsNPC)
	{
		if (bullet->getAliveState())
		{
			bullet->setAlive(false);
		}
	}

	for (GameObject* post : goalPost)
	{
		if (post->getAliveState())
		{
			post->setAlive(false);
		}
	}

	for (GameObject* image : background)
	{
		image->setAlive(false);
	}

	grid->setAlive(false);
	
	pc_flash->setAlive(false);

}//---

void Game::loadMenu()
{
	menuBackground->setAlive(true);

	for (GameObject* menuItem : levelPreview)
	{
		menuItem->setAlive(true);
	}

	for (GameObject* menuItem : levelNumber)
	{
		menuItem->setAlive(true);
	}

	selectionFrame->setAlive(true);
}

void Game::unloadMenu()
{
	menuBackground->setAlive(false);
	
	for (GameObject* menuItem : levelPreview)
	{
		menuItem->setAlive(false);
	}
	
	for (GameObject* menuItem : levelNumber)
	{
		menuItem->setAlive(false);
	}

	selectionFrame->setAlive(false);
}

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
						npc->setNextShotTime(SDL_GetTicks() + 1000);
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
		if (playerInputEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			isInMenu = false;
			isInLevel = true;
			unloadMenu();
		}
		break;

	case SDL_KEYUP:
		// std::cout << "\n" << playerInputEvent.key.keysym.scancode << " + " << playerInputEvent.key.keysym.mod << " up.";
		playerInput.keyUp = playerInputEvent.key.keysym.scancode;
		break;

	default:
		break;
	}
}//---

void Game::handleMenuEvents()
{
	// Reset Inputs
	playerInput.keyDown = NULL;
	playerInput.keyUp = NULL;
	pc->resetControls();
	points = 0;

	//Check for Events
	SDL_PollEvent(&playerInputEvent);

	switch (playerInputEvent.type)
	{
	case SDL_QUIT:
		gameRunning = false;
		break;

	case SDL_KEYDOWN:
		if (playerInputEvent.key.keysym.scancode == SDL_SCANCODE_LEFT)
		{
			if (mapSelected > 1)
			{
				mapSelected--;
				selectionFrame->setX(selectionFrame->getX() - 640);
			}
			else
			{
				break;
			}
		}
		else if (playerInputEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			if (mapSelected <= 2)
			{
				mapSelected++;
				selectionFrame->setX(selectionFrame->getX() + 640);
				loadMap(mapSelected + 2);
			}
			else
			{
				break;
			}
		}
		else if (playerInputEvent.key.keysym.scancode == SDL_SCANCODE_RETURN)
		{
			loadMap(mapSelected + 2);
			std::cout << "loaded map: " << mapSelected;
			isInMenu = false;
			isInLevel = true;
			unloadMenu();
		}
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

					for (GameObject* block : movingWalls)  //  Moving Terrain ------
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
		}
	}

	for (GameObject* item : items) // Pickups items
	{
		// Only check Alive Items
		if (item->getAliveState() == true)
		{
			objectRect.x = item->getX();
			objectRect.y = item->getY();

			if (SDL_HasIntersection(&pcRect, &objectRect))
			{
				item->setAlive(false); // Disable the item hit
				pc->increaseBulletTier();
				points += 500;
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
				pc->decreaseBulletTier(); // Punish player for getting hit
				hurt = true;
				hurtTimer = 100;
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

			for (GameObject* block : movingWalls)  //  Terrain ------
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
        				npc->changeHP(-bullet->getDamage()); // Apply damage
						if (npc->getHP() < 0) { npc->setAlive(false); npc->setHP(100); }// Disable the NPC 						
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
				// std::cout << "destroyed" << wall;
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
			
		}
	}

	for (GameObject* post : goalPost)
	{
		if (post->getAliveState())
		{
			objectRect.x = post->getX();
			objectRect.y = post->getY();

			if (SDL_HasIntersection(&pcRect, &objectRect))
			{
				printf("You win!");
				unloadMap();
				loadMenu();
				isInMenu = true;
				isInLevel = false;
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
		if (image->getY() > 1080) image->setY(-2160);
	}

	grid->update(frameTime);

	pc->updatePC(playerInput.keyDown, playerInput.keyUp, frameTime);

	if (hurt)
	{
		if (hurtTimer % 10 >= 4)
		{
			pc_flash->setAlive(true);
			hurtTimer--;
		}
		else if (hurtTimer % 10 < 4)
		{
			pc_flash->setAlive(false);
			hurtTimer--;
		}
		else if (hurtTimer <= 0)
		{
			pc_flash->setAlive(false);
			hurt = false;
			hurtTimer = 100;
		}
	}
	pc_flash->setX(pc->getX());
	pc_flash->setY(pc->getY());
	pc_flash->setAngle(pc_flash->getAngle() + 10);
	pc_flash->update(frameTime);

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

	for (GameObject* item : items) // Update Items
	{
		if (item->getAliveState() == true) { item->update(frameTime); item->setYVel(20); }
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
		if (wall->getAliveState()) { wall->update(frameTime); wall->setYVel(20); }
	}

	for (GameObject* post : goalPost)
	{
		if (post->getAliveState()) { post->update(frameTime); post->setYVel(20); }
	}

	checkAttacks();
	checkCollision(frameTime);
}

void Game::render()
{
	SDL_RenderClear(renderer);

	// Menu stuff

	if (menuBackground->getAliveState()) { menuBackground->render(); }

	// Game stuff

	for (GameObject* image : background)
	{
		if (image->getAliveState()) { image->render(); }
	}

	if (grid->getAliveState()) grid->render();

	pc->renderPC();

	if (pc_flash->getAliveState()) pc_flash->render();
	
	for (Projectile* bullet : bulletsPC)
	{
		if (bullet->getAliveState()) { bullet->renderProjectile(); }
	}

	for (Projectile* bullet : bulletsNPC)
	{
		if (bullet->getAliveState()) { bullet->renderProjectile(); }
	}

	for (GameObject* item : items) // Update Items
	{
		if (item->getAliveState() == true) item->render();
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

	for (GameObject* post : goalPost)
	{
		if (post->getAliveState()) { post->render(); }
	}


	updateGUI();
	SDL_RenderPresent(renderer); 	// Update the screen
	checkGameStates();
}//---

void Game::updateMenu(float frameTime)
{
	if (menuBackground->getAliveState()) { menuBackground->update(frameTime); }

	for (GameObject* menuItem : levelNumber)
	{
		if (menuItem->getAliveState())
		{
			menuItem->update(frameTime);
		}
	}

	for (GameObject* menuItem : levelPreview)
	{
		if (menuItem->getAliveState())
		{
			menuItem->update(frameTime);
		}
	}

	selectionFrame->update(frameTime);
}

void Game::renderMenu()
{
	SDL_RenderClear(renderer);

	// Menu stuff

	if (menuBackground->getAliveState()) {
		menuBackground->render();
	}

	for (GameObject* menuItem : levelNumber)
	{
		if (menuItem->getAliveState())
		{
			menuItem->render();
		}
	}

	for (GameObject* menuItem : levelPreview)
	{
		if (menuItem->getAliveState())
		{
			menuItem->render();
		}
	}

	if (selectionFrame->getAliveState()) selectionFrame->render();

	updateMenuGUI();
	SDL_RenderPresent(renderer); 	// Update the screen
}

void Game::updateGUI()
{
	
	std::string  screenText;
	
	/*
	SDL_Rect textRect = { 8,8,0,0 }; // start position of the text

	// text to be on screen Left Side	
	// screenText = "Items: " + std::to_string(activeItems);
	screenText += "  NPCs: " + std::to_string(activeNPCs);
	textColour = { 255, 255, 0 };

	// render the text to screen
	textSurface = TTF_RenderText_Blended_Wrapped(font, screenText.c_str(), textColour, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	*/

	// text to be on screen Right Side
	SDL_Rect textRect = { 1250,100,0,0 }; // start position of the text
	screenText = "HP: " + std::to_string(int(pc->getHP()));
	screenText += "\n\nTime: " + std::to_string(SDL_GetTicks64() / 1000);
	screenText += "\n\nPoints: " + std::to_string(points);
	screenText += "\n\nFiring Level: " + std::to_string(pc->getBulletTier());
	textColour = { 0, 255, 0 };

	// render the text to screen
	textSurface = TTF_RenderText_Blended_Wrapped(font2, screenText.c_str(), textColour, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);


	// Clear the memory
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

}//--

void Game::updateMenuGUI()
{
	/*
	std::string  screenText;
	SDL_Rect textRect = { 1250,100,0,0 }; // start position of the text
	// text to be on screen Right Side
	screenText = "Level selected: " + std::to_string(int(mapSelected));
	screenText += "\n\nPosition ";
	screenText += "\n\nX: " + std::to_string(selectionFrame->getX());
	screenText += "\n\nY: " + std::to_string(selectionFrame->getY());
	textColour = { 0, 255, 0 };

	// render the text to screen
	// This line below is what actually adds the text to the game
	textSurface = TTF_RenderText_Blended_Wrapped(font2, screenText.c_str(), textColour, 0);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);


	// Clear the memory
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	*/
}

void Game::checkGameStates()
{
	activeItems = 0;
	for (GameObject* item : items)
	{
		if (item->getAliveState()) activeItems++;
	}

	activeNPCs = 0;
	for (NPC* npc : npcs)
	{
		if (npc->getAliveState()) activeNPCs++;
	}

	// Check if PC is alive
	if (pc->getHP() < 0)
	{
		//gameRunning = false;
		pc->changeHP(100);
		std::cout << "Player died";
		unloadMap();
		loadMenu();
		isInMenu = true;
		isInLevel = false;
	}
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

		// Initialise Fonts
		TTF_Init();
		font = TTF_OpenFont("assets/fonts/arial.ttf", 22); // size is the last value
		font2 = TTF_OpenFont("assets/fonts/bubble_pixel-7_dark.ttf", 18); // size is the last value
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