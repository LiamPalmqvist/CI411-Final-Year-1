#pragma once
// =======================================================
// Libraries / Headers to include 
#include "Game.h"
#include <vector>

class GameObject
{
public:
	GameObject();
	GameObject(const char* spriteFileName, float xPos, float yPos);
	void update();
	void render();
	void setSize(int width, int height);
	bool getAliveState();
	void setAlive(bool state) { isActive = state; }
	void addX(float xAmount) { x += xAmount; }
	void addY(float yAmount) { y += yAmount; }
	void Loadtexture(const char* spriteFileName);
	void screenLimit();
	void disableOffScreen();
	float getX() { return x; }
	float getY() { return y; }
	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }

protected: // derived classes can access
	bool isActive = false;
	float x = -100, y = -100;	 // off the screen by default
	float xVel = 0, yVel = 0;
	float speed = 100;
	float angle = 0;
	float health = 100;
	float maxHealth = 100;
	float oldX = 0, oldY = 0;

	SDL_Rect srcRect = { 0,0,0,0 };
	SDL_Rect destRect = { 0,0,0,0 };
	SDL_Texture* spriteTexture = nullptr;
};


// =======================================================

class PlayerCharacter : GameObject
{
public:
	PlayerCharacter(const char* spriteFileName, float xPos, float yPos, float rotation);
	void updatePC(int keyDown, int keyUp, float frameTime);
	void renderPC();
	
	void smoothMove(int keyDown, int keyUp, float frameTime);
	
	float getX() { return x; }
	float getY() { return y; }
	float getAngle() { return angle; }
	float getHP() { return health; }
	void changeHP(float hpChange) { health += hpChange; }

	float getVelX() { return xVel; }
	float getVelY() { return yVel; }
	void setVelX(float newXVel) { xVel = newXVel; }
	void setVelY(float newYVel) { yVel = newYVel; }
	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }
	float getOldX() { return oldX; }
	float getOldY() { return oldY; }
	void stop();

private:
	int sprinting = 0;
	float wDown = false;
	float sDown = false;
	float aDown = false;
	float dDown = false;
	float drag = 0.9F;
	float acceleration = 50;

};