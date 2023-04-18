#pragma once
// =========================================================================
// Libraries / Headers to include 
#include "Game.h"

class GameObject
{
public:
	GameObject();
	GameObject(const char* spriteFileName, float xPos, float yPos);
	void update(float frameTime);
	void moveDown(float frameTime);
	void render();
	void setSize(int width, int height);
	bool getAliveState();
	void setAlive(bool state) { isActive = state; }
	void addX(float xAmount) { x += xAmount; }
	void addY(float yAmount) { y += yAmount; }
	void Loadtexture(const char* spriteFileName);
	void screenLimit();
	void screenWrap();
	void screenBounce();
	void disableOffScreen();
	float getX() { return x; }
	float getY() { return y; }
	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }
	void setYVel(float vel) { yVel = vel; }

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

// =========================================================================

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
	bool isFiring() { return firing; }
	int getBulletTier() { return bulletTier; }
	void setBulletTier(int tier) { bulletTier = tier; }
	void increaseBulletTier() { if (bulletTier == 5) return; else bulletTier++; }
	void decreaseBulletTier() { if (bulletTier == 0) return; else bulletTier--; }

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
	float wDown = false;
	float sDown = false;
	float aDown = false;
	float dDown = false;
	float firing = false;
	int bulletTier = 0;
	float drag = 0.9F;
	float acceleration = 50;

};

// =========================================================================

class Projectile : GameObject
{
public:
	Projectile(const char* spriteFileName, int xPos, int yPos, float rotation, float spriteSize);

	bool getAliveState() { return isActive; }
	void setAlive(bool state) { isActive = state; }
	void fire(float xSent, float ySent, float angleSent);
	void fireAtTarget(float startX, float startY, float targetX, float targetY);
	void update(float frameTime);
	void renderProjectile();
	void setBulletSpeed(float newSpeed) { speed = newSpeed; }
	float getX() { return x; }
	float getY() { return y; }

	float getDamage() { return damage; }
	void setDamage(float newDamage) { damage = newDamage; }
	Uint32 getSize() { return bulletSize; }


private:
	Uint32 lastTimeActivated = 0;
	Uint32 disableTime = 0;
	Uint32  bulletSize = 0;
	float damage = 30, range = 20;
};

// =========================================================================

class NPC : GameObject {
public:
	NPC(const char* spriteFileName, int xPos, int yPos, float rotation);
	void renderNPC();
	void moveDown(float frameTime);
	int updateNPC(int points);
	void setAlive(bool state) { isActive = state; }
	bool getAliveState() { return isActive; }
	void roam(float frameTime);
	void chasePC(float pcX, float pcY);
	void setSpeed(float newSpeed) { speed = newSpeed; }
	void screenCrawl(float frameTime);
	float getX() { return x; }
	float getY() { return y; }
	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }

	float getHP() { return health; }
	void changeHP(float hpChange) { health += hpChange; }
	int getNextShotTime() { return nextShotTime; }
	void setNextShotTime(int newTime) { nextShotTime = newTime; }
	void changeDirection();

private:
	float drag = 0.99F;
	float acceleration = 50;
	int nextShotTime = 0;
};