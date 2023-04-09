
// ======================================================= 
// Libraries / Headers to include 
#include "GameObjects.h"
#include "Game.h"

// ======================================================= 
GameObject::GameObject()
{	// default Constructor; 
}

void GameObject::Loadtexture(const char* spriteFileName)
{
	// Load Image from File to create the sprite
	SDL_Surface* tempSurface = IMG_Load(spriteFileName);
	spriteTexture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	if (spriteTexture == NULL) printf(" Image Load Error  - SDL Error: %s\n", SDL_GetError());
	SDL_FreeSurface(tempSurface);

}//---

// ======================================================= 

GameObject::GameObject(const char* spriteFileName, float xPos, float yPos)
{
	Loadtexture(spriteFileName);	// Load Image from File
	x = xPos; 	y = yPos;
	srcRect.h = srcRect.w = SPRITE_SIZE;
	srcRect.x = srcRect.y = 0;
	destRect.h = destRect.w = SPRITE_SIZE;
	destRect.x = (int)x;	destRect.y = (int)y;
}//---

// ======================================================= 
bool GameObject::getAliveState()
{
	return isActive;
}//---


// ======================================================= 
void GameObject::update()
{	// update the display Rectangle Position
	destRect.x = (int)x;
	destRect.y = (int)y;
}//---

// ======================================================= 
void GameObject::setSize(int width, int height) // if sprite is non standard size
{
	// The image source 
	srcRect.w = width;
	srcRect.h = height;
	// Destination Screen display 
	destRect.w = width;
	destRect.h = height;
}//----


// ======================================================= 
void GameObject::render()
{
	// add the Sprite to the Render Image
	SDL_RenderCopy(Game::renderer, spriteTexture, &srcRect, &destRect);
}//---

// ======================================================= 

void GameObject::disableOffScreen()
{
	// disable if sprite leave the screen area
	if (x > SCREEN_WIDTH || x < 0 || y > SCREEN_HEIGHT || y < 0)
	{
		isActive = false;
	}

}//---

void GameObject::screenLimit()
{
	// Limit to edges
	bool stopMoving = false;
	if (x > SCREEN_WIDTH - SPRITE_SIZE)
	{
		stopMoving = true;
		x = SCREEN_WIDTH - SPRITE_SIZE;
	}
	if (x < 0)
	{
		stopMoving = true;
		x = 0;
	}
	if (y > SCREEN_HEIGHT - SPRITE_SIZE)
	{
		stopMoving = true;
		y = SCREEN_HEIGHT - SPRITE_SIZE;
	}
	if (y < 0)
	{
		stopMoving = true;
		y = 0;
	}

	if (stopMoving)
	{
		xVel = 0;
		yVel = 0;
	}
}//---


// ======================================================= 
// PC Object 
// ======================================================= 

PlayerCharacter::PlayerCharacter(const char* spriteFileName, float xPos, float yPos, float rotation)
{
	Loadtexture(spriteFileName);	// Load Image from File
	x = xPos; 	y = yPos;
	angle = rotation;
	srcRect.h = srcRect.w = SPRITE_SIZE;
	srcRect.x = srcRect.y = 0;
	destRect.h = destRect.w = SPRITE_SIZE;
	destRect.x = (int)x; destRect.y = (int)y;
}//----


void PlayerCharacter::renderPC()
{
	// add the Sprite to the Render Image
	SDL_RenderCopyEx(Game::renderer, spriteTexture, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
}//---


// ======================================================= 

void PlayerCharacter::updatePC(int keyDown, int keyUp, float frameTime)
{
	// Set old Position for Collision detection
	oldX = x; oldY = y;

	// Limit Health
	if (health > maxHealth) health = maxHealth;

	// Apply the movement
	smoothMove(keyDown, keyUp, frameTime);
	angle += 10;

	//update Drawing Position Rect
	destRect.x = (int)x;
	destRect.y = (int)y;
}//-----

// ======================================================= 

void PlayerCharacter::smoothMove(int keyDown, int keyUp, float frameTime)
{
	switch (keyDown)
	{
	case 26: // W
		wDown = true;
		keyDown = NULL;
		break;
	case 22: // A
		aDown = true;
		keyDown = NULL;
		break;
	case 4: // S
		sDown = true;
		keyDown = NULL;
		break;
	case 7: // D
		dDown = true;
		keyDown = NULL;
		break;
	}

	switch (keyUp)
	{
	case 26: // W
		wDown = false;
		keyDown = NULL;
		break;
	case 22: // A
		aDown = false;
		keyDown = NULL;
		break;
	case 4: // S
		sDown = false;
		keyDown = NULL;
		break;
	case 7: // D
		dDown = false;
		keyDown = NULL;
		break;
	}

	// WSAD /// Add Acceleration
	if (wDown) yVel -= acceleration * frameTime;
	if (aDown) yVel += acceleration * frameTime;
	if (sDown)  xVel -= acceleration * frameTime;
	if (dDown) xVel += acceleration * frameTime;

	// Limit Speed
	if (xVel > speed) xVel = speed;
	if (yVel > speed) yVel = speed;
	if (xVel < -speed) xVel = -speed;
	if (yVel < -speed) yVel = -speed;

	// apply drag
	if (abs(xVel) > 0.3f) xVel *= drag; else xVel = 0;
	if (abs(yVel) > 0.3f) yVel *= drag; else yVel = 0;


	// Update positions
	x += xVel;
	y += yVel;

	//screenLimit();
	screenLimit();
}//---


void PlayerCharacter::stop()
{
	xVel = yVel = 0;
	x = oldX;
	y = oldY;
}//--

// ======================================================= 