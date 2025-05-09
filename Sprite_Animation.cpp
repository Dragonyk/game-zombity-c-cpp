#include "Sprite_Animation.h"



Sprite_Animation::Sprite_Animation()
{
}

//Define padr�es para anima��o come�ando de um ponto(min) ate outro(max) com um intervalo de tempo(tmax)
void Sprite_Animation::setAnimation(int min, int max, float tmax, int iddle) {
	if (min < 0) minAnim = 0;
	if (max < 0) maxAnim = 0;
	if (min > max) {
		int temp = max;
		max = min;
		min = temp;
	}

	if (tmax <= 0)
		tmax = 1;

	if (iddle < 0)
		iddle = 0;
	iddleFrame = iddle;
	minAnim = min;
	curAnim = min;
	maxAnim = max;

	maxTimeAnim = tmax;
	curTimeAnim = 0;
}

//Avan�a um quadro de anima��o
void Sprite_Animation::nextFrame()
{
	if (curAnim < maxAnim - 1)
		curAnim += 1;
	else
		curAnim = minAnim;
}

//Retorna o quadro de anima��o atual
int Sprite_Animation::getCurFrame()
{
	return curAnim;
}

//Define a posi��o do quadro atual para o quadro inicial
void Sprite_Animation::startAnimation()
{
	curAnim = minAnim;
}

void Sprite_Animation::setFrame(int value)
{
	if (value < 0)
		value = 0;

	curAnim = value;
}

//Avan�a o tempo dos quadros para um valor(value)
void Sprite_Animation::addTime(float value)
{
	if (value <= 0) value = 0.1;
	curTimeAnim += value;
}

//Retorna o tempo atual
float Sprite_Animation::getTime()
{
	return curTimeAnim;
}

//Define o tempo atual para zero
void Sprite_Animation::resetTime()
{
	curTimeAnim = 0;
}