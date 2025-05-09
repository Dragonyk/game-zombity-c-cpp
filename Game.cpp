#include "Game.h"

Game::Game(int value)
{
	if (value <= 0)
		value = 10;
	pointsToWin = value;
	remPoints	= value;
}

Game::Game()
{
	pointsToWin = 10;
	remPoints	= 10;
}

void Game::setPoints(int value)
{
	if (value < 0)
		value = 0;
	if (value > pointsToWin)
		value = pointsToWin;
	remPoints = value;
}

int Game::getPoints()
{
	return remPoints;
}
