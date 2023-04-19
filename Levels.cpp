#include "Levels.h"

// Game Objects Array

int Levels::getTileContent(int level, int x, int y)
{
	level = level - 1; // start at 0 in array
	return levelMapsNew[level][y][x];
}//---

int Levels::getLevelLength(int level)
{
	return levelMapLength[level-1];
}

/* 
void Levels::setTileContent(int level, int x, int y, int newObject)
{
	levelMaps[level][y][x] = newObject;
}//---
*/