#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_LEFT 0
#define ENEMY_RIGHT 1
#define ENEMY_LEFT_STAND 2
#define ENEMY_RIGHT_STAND 3

#include "Sprite_Animation.h"

class Enemy
{
public:
	Enemy(float,int,float,float);
	Enemy();

	void setSpeed(float);
	float getSpeed();

	void setAttack(float value);

	float getAttack();

	float x, y;
	float defX, defY;
	float dir;

	float dps, dps_time;

	int isActive, life;

	Sprite_Animation anim, animDeath;
private:
	float speed;
	int atkPower;
};
#endif // !ENEMY_H
