#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_LEFT 1
#define PLAYER_RIGHT 2

#define PLAYER_UP 1
#define PLAYER_DOWN 2

#define STATE_FREE 0
#define STATE_ATTACK 1
#define STATE_MELEE 2

#include "Sprite_Animation.h"

class Player
{
public:
	Player(int,float,float);
	Player();

	void setLife(int);
	int getLife();

	int maxLife, state, ammo, ammo_max, isAtk, isDmg;
	float ammo_maxTime, ammo_time;
	float x, y, dirA, dirX, dirY, speed, vmax, vmax2;
	float defX, defY, dx, dy;

	Sprite_Animation animMove;
	Sprite_Animation animFireL, animFireR;
	Sprite_Animation animMeleeL, animMeleeR;
	Sprite_Animation animBlood;
private:
	int life;
};
#endif // !PLAYER_H
