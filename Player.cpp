#include "Player.h"

Player::Player(int mL, float dX, float dY)
{
	if (mL <= 0)
		mL = 3;

	if (dX < 0) 
		dX = 0;

	if (dY < 0 || dY > 1) 
		dY = 0.5f;

	maxLife  = mL;
	life	 = mL;
	ammo = 10;
	ammo_max = 10;
	ammo_maxTime = 1;
	ammo_time = 0;
	isAtk = 0;
	isDmg = 0;

	dirA	 = PLAYER_RIGHT;
	dirY	 = 0;
	dirX	 = 0;
	defX = dX;
	defY = dY;
	x = defX;
	y = defY;
}

Player::Player()
{
	maxLife  = 10;
	life	 = 10;
	vmax	 = 1;
	speed	 = 1;
	ammo	 = 10;
	ammo_max = 10;
	ammo_maxTime = 1;
	ammo_time = 0;
	isAtk = 0;
	isDmg = 0;

	dirA = PLAYER_RIGHT;
	dirY = 0;
	dirX = 0;

	defX = 0.06f;
	defY = 0.05f;
	x = defX;
	y = defY;
}

void Player::setLife(int value)
{
	if (value < 0)
		value = 0;

	life = value;
}

int Player::getLife()
{
	return life;
}