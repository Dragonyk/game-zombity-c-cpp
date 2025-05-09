#include "Enemy.h"

//Define construtor do inimigo com a velocidade e poder de ataque como parametro
Enemy::Enemy(float mS, int atk, float dx, float dy)
{
	if (mS < 0)
		mS = 1;
	if (atk < 0)
		atk = 1;
	speed	 = mS;
	atkPower = atk;
	dir		 = ENEMY_LEFT_STAND;

	defX = dx;
	defY = dy;
	x = defX;
	y = defY;
	life = 100;
	dps = 3;
	dps_time = 0;

	isActive = 0;
}

//Construtor padrão do inimigo
Enemy::Enemy()
{
	speed	 = 1;
	atkPower = 1;
	dir		 = ENEMY_LEFT_STAND;
	defX = 0.5f;
	defY = 0.02f;
	x = defX;
	y = defY;
	life = 100;
	dps = 3;
	dps_time = 0;

	isActive = 0;
}

//Define a velocidade de movimento
void Enemy::setSpeed(float value)
{
	if (value < 0)
		value = 0;

	speed = value;
}

//Retorna a velocidade de movimento
float Enemy::getSpeed()
{
	return speed;
}

//Define o poder de ataque
void Enemy::setAttack(float value)
{
	if (value < 0)
		value = 0;

	atkPower = value;
}

//Retorna o poder de ataque
float Enemy::getAttack()
{
	return atkPower;
}